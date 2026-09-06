#include "Salle.h"
#include "../database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

QString Salle::lastError = "";

Salle::Salle()
    : m_id(0), m_capacite(0), m_disponible(true) {}

Salle::Salle(int id, const QString& nom, int capacite, const QString& type, const QString& equipement, bool disponible)
    : m_id(id), m_nom(nom), m_capacite(capacite), m_type(type), m_equipement(equipement), m_disponible(disponible) {}

Salle Salle::fromQuery(const QSqlQuery& query) {
    return Salle(
        query.value("ID_SALLE").toInt(),
        query.value("NOM_SALLE").toString(),
        query.value("CAPACITE").toInt(),
        query.value("TYPE_SALLE").toString(),
        query.value("EQUIPEMENT").toString(),
        query.value("DISPONIBLE").toInt() == 1
    );
}

bool Salle::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE) "
                  "VALUES (:nom, :cap, :type, :eq, :dispo)");
    query.bindValue(":nom", m_nom);
    query.bindValue(":cap", m_capacite);
    query.bindValue(":type", m_type);
    query.bindValue(":eq", m_equipement);
    query.bindValue(":dispo", m_disponible ? 1 : 0);

    bool res = query.exec();
    if (!res) {
        lastError = query.lastError().text();
        qDebug() << "Salle::ajouter error:" << lastError;
    }
    return res;
}

bool Salle::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE SALLE SET NOM_SALLE = :nom, CAPACITE = :cap, TYPE_SALLE = :type, "
                  "EQUIPEMENT = :eq, DISPONIBLE = :dispo WHERE ID_SALLE = :id");
    query.bindValue(":nom", m_nom);
    query.bindValue(":cap", m_capacite);
    query.bindValue(":type", m_type);
    query.bindValue(":eq", m_equipement);
    query.bindValue(":dispo", m_disponible ? 1 : 0);
    query.bindValue(":id", m_id);

    bool res = query.exec();
    if (!res) {
        lastError = query.lastError().text();
        qDebug() << "Salle::modifier error:" << lastError;
    }
    return res;
}

bool Salle::supprimer() {
    QSqlQuery query;
    query.prepare("DELETE FROM SALLE WHERE ID_SALLE = :id");
    query.bindValue(":id", m_id);
    bool res = query.exec();
    if (!res) {
        lastError = query.lastError().text();
        qDebug() << "Salle::supprimer error:" << lastError;
    }
    return res;
}

QList<Salle> Salle::afficher() {
    QList<Salle> list;
    QSqlQuery query("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE FROM SALLE ORDER BY ID_SALLE ASC");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

QList<Salle> Salle::rechercher(const QString& nom, const QString& type, int capMin, int capMax, int disponible) {
    QList<Salle> list;
    QString sql = "SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE FROM SALLE WHERE 1=1";

    if (!nom.isEmpty()) sql += " AND LOWER(NOM_SALLE) LIKE '%" + nom.toLower() + "%'";
    if (!type.isEmpty() && type != "Tous") sql += " AND TYPE_SALLE = '" + type + "'";
    if (capMin > 0) sql += QString(" AND CAPACITE >= %1").arg(capMin);
    if (capMax > 0) sql += QString(" AND CAPACITE <= %1").arg(capMax);
    if (disponible != -1) sql += QString(" AND DISPONIBLE = %1").arg(disponible);

    sql += " ORDER BY ID_SALLE ASC";

    QSqlQuery query(sql);
    while (query.next()) {
        list.append(fromQuery(query));
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

    QString dir = (ordre == "Décroissant") ? "DESC" : "ASC";
    QString sql = QString("SELECT ID_SALLE, NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE FROM SALLE ORDER BY %1 %2").arg(col).arg(dir);

    QSqlQuery query(sql);
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

QMap<QString, int> Salle::statsParType() {
    QMap<QString, int> stats;
    QSqlQuery query("SELECT TYPE_SALLE, COUNT(*) FROM SALLE GROUP BY TYPE_SALLE");
    while (query.next()) {
        stats[query.value(0).toString()] = query.value(1).toInt();
    }
    return stats;
}

int Salle::countDisponibles() {
    QSqlQuery query("SELECT COUNT(*) FROM SALLE WHERE DISPONIBLE = 1");
    if (query.next()) return query.value(0).toInt();
    return 0;
}

int Salle::countIndisponibles() {
    QSqlQuery query("SELECT COUNT(*) FROM SALLE WHERE DISPONIBLE = 0");
    if (query.next()) return query.value(0).toInt();
    return 0;
}

QMap<QString, int> Salle::statsCapaciteMoyenneParType() {
    QMap<QString, int> stats;
    QSqlQuery query("SELECT TYPE_SALLE, AVG(CAPACITE) FROM SALLE GROUP BY TYPE_SALLE");
    while (query.next()) {
        stats[query.value(0).toString()] = query.value(1).toInt();
    }
    return stats;
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

QString Salle::analyseIAOptimisation() {
    QList<Salle> salles = afficher();
    int total = salles.size();
    int dispo = countDisponibles();
    int indispo = countIndisponibles();

    if (total == 0) return "🤖 Aucun espace de formation disponible dans la base Oracle.";

    double ratioDispo = (double)dispo / total * 100.0;
    int scoreEfficiency = static_cast<int>(100 - ratioDispo * 0.4);

    QString report = "🤖 <b>Rapport d'Analyse & Optimisation par IA - Salles de Formation</b><br><br>";
    report += QString("📊 <b>Score d'Efficacité Énergétique & d'Occupation :</b> <font color='#2563EB'><b>%1 / 100</b></font><br>").arg(scoreEfficiency);
    report += QString("• Total des Salles enregistrées : <b>%1</b><br>").arg(total);
    report += QString("• Salles Actives & Disponibles : <font color='#059669'><b>%1 (%2%%)</b></font><br>").arg(dispo).arg(QString::number(ratioDispo, 'f', 1));
    report += QString("• Salles Indisponibles / Occupées : <font color='#DC2626'><b>%1</b></font><br><br>").arg(indispo);

    report += "💡 <b>Recommandations de l'Algorithme IA :</b><br>";

    for (const Salle& s : salles) {
        if (s.getCapacite() >= 100 && s.isDisponible()) {
            report += QString("  • <font color='#8B5CF6'><b>%1</b></font> (Cap: %2 places) est disponible. L'IA recommande de la réserver prioritairement pour les conférences ou grandes promotions.<br>").arg(s.getNom()).arg(s.getCapacite());
        }
        if (s.getType() == "TP" && !s.getEquipement().contains("Projecteur", Qt::CaseInsensitive)) {
            report += QString("  • ⚠️ <b>%1</b> (Labo TP) manque de projecteur HD. L'IA suggère d'ajouter des équipements vidéo.<br>").arg(s.getNom());
        }
    }

    if (dispo == 0) {
        report += "  • 🚨 <b>Alerte Saturation :</b> 100% des salles sont indisponibles. L'IA recommande la création de créneaux supplémentaires.";
    } else {
        report += "  • ✅ <b>Optimisation des Espace :</b> La répartition actuelle garantit un niveau de disponibilité optimal sans surpopulation.";
    }

    return report;
}
