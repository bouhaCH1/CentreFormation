#include "Salle.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Salle::Salle()
    : m_id(0), m_capacite(0), m_disponible(true) {}

Salle::Salle(int id, const QString& nom, int capacite, const QString& type, const QString& equipement, bool disponible)
    : m_id(id), m_nom(nom), m_capacite(capacite), m_type(type), m_equipement(equipement), m_disponible(disponible) {}

Salle Salle::fromQuery(const QSqlQuery& query) {
    Salle s;
    s.m_id = query.value("ID_SALLE").toInt();
    s.m_nom = query.value("NOM_SALLE").toString();
    s.m_capacite = query.value("CAPACITE").toInt();
    s.m_type = query.value("TYPE_SALLE").toString();
    s.m_equipement = query.value("EQUIPEMENT").toString();
    s.m_disponible = (query.value("DISPONIBLE").toInt() == 1);
    return s;
}

bool Salle::ajouter() {
    QSqlQuery q;
    q.prepare("INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE) "
              "VALUES (:nom, :cap, :type, :eq, :dispo)");
    q.bindValue(":nom", m_nom);
    q.bindValue(":cap", m_capacite);
    q.bindValue(":type", m_type);
    q.bindValue(":eq", m_equipement);
    q.bindValue(":dispo", m_disponible ? 1 : 0);

    if (!q.exec()) {
        qCritical() << "Erreur ajout salle:" << q.lastError().text();
        return false;
    }
    return true;
}

bool Salle::modifier() {
    QSqlQuery q;
    q.prepare("UPDATE SALLE SET NOM_SALLE = :nom, CAPACITE = :cap, TYPE_SALLE = :type, "
              "EQUIPEMENT = :eq, DISPONIBLE = :dispo WHERE ID_SALLE = :id");
    q.bindValue(":nom", m_nom);
    q.bindValue(":cap", m_capacite);
    q.bindValue(":type", m_type);
    q.bindValue(":eq", m_equipement);
    q.bindValue(":dispo", m_disponible ? 1 : 0);
    q.bindValue(":id", m_id);

    if (!q.exec()) {
        qCritical() << "Erreur modification salle:" << q.lastError().text();
        return false;
    }
    return true;
}

bool Salle::supprimer() {
    // Vérification de la contrainte d'intégrité référentielle avec la table COURS
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM COURS WHERE ID_SALLE = :id");
    checkQuery.bindValue(":id", m_id);
    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() > 0) {
            qWarning() << "Impossible de supprimer la salle ID" << m_id << ": des cours y sont affectés.";
            return false;
        }
    }

    QSqlQuery q;
    q.prepare("DELETE FROM SALLE WHERE ID_SALLE = :id");
    q.bindValue(":id", m_id);
    return q.exec();
}

QList<Salle> Salle::afficher() {
    QList<Salle> list;
    QSqlQuery q("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE FROM SALLE ORDER BY ID_SALLE ASC");
    while (q.next()) {
        list.append(fromQuery(q));
    }
    return list;
}

QList<Salle> Salle::rechercher(const QString& nom, const QString& type, int capMin, int capMax, int disponible) {
    QList<Salle> list;
    QString sql = "SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE FROM SALLE WHERE 1=1";

    if (!nom.trimmed().isEmpty()) {
        sql += " AND UPPER(NOM_SALLE) LIKE UPPER(:nom)";
    }
    if (!type.trimmed().isEmpty() && type != "Tous") {
        sql += " AND TYPE_SALLE = :type";
    }
    if (capMin > 0) {
        sql += " AND CAPACITE >= :capMin";
    }
    if (capMax > 0) {
        sql += " AND CAPACITE <= :capMax";
    }
    if (disponible != -1) {
        sql += " AND DISPONIBLE = :dispo";
    }
    sql += " ORDER BY NOM_SALLE ASC";

    QSqlQuery q;
    q.prepare(sql);
    if (!nom.trimmed().isEmpty()) {
        q.bindValue(":nom", "%" + nom.trimmed() + "%");
    }
    if (!type.trimmed().isEmpty() && type != "Tous") {
        q.bindValue(":type", type);
    }
    if (capMin > 0) {
        q.bindValue(":capMin", capMin);
    }
    if (capMax > 0) {
        q.bindValue(":capMax", capMax);
    }
    if (disponible != -1) {
        q.bindValue(":dispo", disponible);
    }

    if (q.exec()) {
        while (q.next()) {
            list.append(fromQuery(q));
        }
    }
    return list;
}

QList<Salle> Salle::trier(const QString& critere, const QString& ordre) {
    QList<Salle> list;
    QString col = "ID_SALLE";
    if (critere == "Nom") col = "NOM_SALLE";
    else if (critere == "Capacite") col = "CAPACITE";
    else if (critere == "Type") col = "TYPE_SALLE";
    else if (critere == "Disponibilite") col = "DISPONIBLE";

    QString dir = (ordre.toUpper() == "DESC" || ordre == "Décroissant") ? "DESC" : "ASC";
    QString sql = QString("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE FROM SALLE ORDER BY %1 %2").arg(col, dir);

    QSqlQuery q(sql);
    while (q.next()) {
        list.append(fromQuery(q));
    }
    return list;
}

QMap<QString, int> Salle::statsParType() {
    QMap<QString, int> res;
    QSqlQuery q("SELECT TYPE_SALLE, COUNT(*) FROM SALLE GROUP BY TYPE_SALLE");
    while (q.next()) {
        res.insert(q.value(0).toString(), q.value(1).toInt());
    }
    return res;
}

int Salle::countDisponibles() {
    QSqlQuery q("SELECT COUNT(*) FROM SALLE WHERE DISPONIBLE = 1");
    if (q.next()) return q.value(0).toInt();
    return 0;
}

int Salle::countIndisponibles() {
    QSqlQuery q("SELECT COUNT(*) FROM SALLE WHERE DISPONIBLE = 0");
    if (q.next()) return q.value(0).toInt();
    return 0;
}

QMap<QString, int> Salle::statsCapaciteMoyenneParType() {
    QMap<QString, int> res;
    QSqlQuery q("SELECT TYPE_SALLE, AVG(CAPACITE) FROM SALLE GROUP BY TYPE_SALLE");
    while (q.next()) {
        res.insert(q.value(0).toString(), q.value(1).toInt());
    }
    return res;
}

bool Salle::toggleDisponibilite(int id) {
    QSqlQuery q;
    q.prepare("UPDATE SALLE SET DISPONIBLE = CASE WHEN DISPONIBLE = 1 THEN 0 ELSE 1 END WHERE ID_SALLE = :id");
    q.bindValue(":id", id);
    return q.exec();
}

QList<QPair<QString, QString>> Salle::getCoursEnSalle(int idSalle) {
    QList<QPair<QString, QString>> list;
    QSqlQuery q;
    q.prepare("SELECT NOM_COURS, TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') || ' - ' || TO_CHAR(DATE_FIN, 'DD/MM/YYYY') FROM COURS WHERE ID_SALLE = :id");
    q.bindValue(":id", idSalle);
    if (q.exec()) {
        while (q.next()) {
            list.append(qMakePair(q.value(0).toString(), q.value(1).toString()));
        }
    }
    return list;
}
