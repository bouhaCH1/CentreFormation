#include "Cours.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDebug>


Cours::Cours()
    : m_id(0), m_duree(0), m_idSalle(0) {}

Cours::Cours(int id, const QString& nom, const QString& description, int duree,
             const QString& niveau, const QString& categorie,
             const QDate& dateDebut, const QDate& dateFin, int idSalle)
    : m_id(id), m_nom(nom), m_description(description), m_duree(duree),
      m_niveau(niveau), m_categorie(categorie),
      m_dateDebut(dateDebut), m_dateFin(dateFin), m_idSalle(idSalle) {}

Cours Cours::fromQuery(const QSqlQuery& query) {
    Cours c;
    c.m_id = query.value("ID_COURS").toInt();
    c.m_nom = query.value("NOM_COURS").toString();
    c.m_description = query.value("DESCRIPTION").toString();
    c.m_duree = query.value("DUREE").toInt();
    c.m_niveau = query.value("NIVEAU").toString();
    c.m_categorie = query.value("CATEGORIE").toString();
    c.m_dateDebut = query.value("DATE_DEBUT").toDate();
    c.m_dateFin = query.value("DATE_FIN").toDate();
    c.m_idSalle = query.value("ID_SALLE").toInt();
    if (query.record().indexOf("NOM_SALLE") != -1) {
        c.m_nomSalle = query.value("NOM_SALLE").toString();
    }
    return c;
}

bool Cours::ajouter() {
    QSqlQuery q;
    q.prepare("INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE) "
              "VALUES (:nom, :desc, :duree, :niveau, :cat, TO_DATE(:debut, 'YYYY-MM-DD'), TO_DATE(:fin, 'YYYY-MM-DD'), :idSalle)");
    q.bindValue(":nom", m_nom);
    q.bindValue(":desc", m_description);
    q.bindValue(":duree", m_duree);
    q.bindValue(":niveau", m_niveau);
    q.bindValue(":cat", m_categorie);
    q.bindValue(":debut", m_dateDebut.toString("yyyy-MM-dd"));
    q.bindValue(":fin", m_dateFin.toString("yyyy-MM-dd"));
    if (m_idSalle > 0)
        q.bindValue(":idSalle", m_idSalle);
    else
        q.bindValue(":idSalle", QVariant(QMetaType::fromType<int>()));

    if (!q.exec()) {
        qCritical() << "Erreur ajout cours:" << q.lastError().text();
        return false;
    }
    return true;
}

bool Cours::modifier() {
    QSqlQuery q;
    q.prepare("UPDATE COURS SET NOM_COURS = :nom, DESCRIPTION = :desc, DUREE = :duree, "
              "NIVEAU = :niveau, CATEGORIE = :cat, DATE_DEBUT = TO_DATE(:debut, 'YYYY-MM-DD'), "
              "DATE_FIN = TO_DATE(:fin, 'YYYY-MM-DD'), ID_SALLE = :idSalle WHERE ID_COURS = :id");
    q.bindValue(":nom", m_nom);
    q.bindValue(":desc", m_description);
    q.bindValue(":duree", m_duree);
    q.bindValue(":niveau", m_niveau);
    q.bindValue(":cat", m_categorie);
    q.bindValue(":debut", m_dateDebut.toString("yyyy-MM-dd"));
    q.bindValue(":fin", m_dateFin.toString("yyyy-MM-dd"));
    if (m_idSalle > 0)
        q.bindValue(":idSalle", m_idSalle);
    else
        q.bindValue(":idSalle", QVariant(QMetaType::fromType<int>()));
    q.bindValue(":id", m_id);

    if (!q.exec()) {
        qCritical() << "Erreur modification cours:" << q.lastError().text();
        return false;
    }
    return true;
}

bool Cours::supprimer() {
    QSqlQuery q;
    q.prepare("DELETE FROM COURS WHERE ID_COURS = :id");
    q.bindValue(":id", m_id);
    return q.exec();
}

QList<Cours> Cours::afficher() {
    QList<Cours> list;
    QSqlQuery q("SELECT ID_COURS, NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE FROM COURS ORDER BY ID_COURS ASC");
    while (q.next()) {
        list.append(fromQuery(q));
    }
    return list;
}

QList<Cours> Cours::afficherAvecSalle() {
    QList<Cours> list;
    QSqlQuery q("SELECT c.ID_COURS, c.NOM_COURS, c.DESCRIPTION, c.DUREE, c.NIVEAU, c.CATEGORIE, "
                "c.DATE_DEBUT, c.DATE_FIN, c.ID_SALLE, s.NOM_SALLE "
                "FROM COURS c LEFT JOIN SALLE s ON c.ID_SALLE = s.ID_SALLE ORDER BY c.ID_COURS ASC");
    while (q.next()) {
        list.append(fromQuery(q));
    }
    return list;
}

QList<Cours> Cours::rechercher(const QString& nom, const QString& niveau,
                               const QString& categorie, const QDate& dateDebut,
                               int dureeMin, int dureeMax) {
    QList<Cours> list;
    QString sql = "SELECT c.ID_COURS, c.NOM_COURS, c.DESCRIPTION, c.DUREE, c.NIVEAU, c.CATEGORIE, "
                  "c.DATE_DEBUT, c.DATE_FIN, c.ID_SALLE, s.NOM_SALLE "
                  "FROM COURS c LEFT JOIN SALLE s ON c.ID_SALLE = s.ID_SALLE WHERE 1=1";

    if (!nom.trimmed().isEmpty()) {
        sql += " AND UPPER(c.NOM_COURS) LIKE UPPER(:nom)";
    }
    if (!niveau.trimmed().isEmpty() && niveau != "Tous") {
        sql += " AND c.NIVEAU = :niveau";
    }
    if (!categorie.trimmed().isEmpty() && categorie != "Tous") {
        sql += " AND UPPER(c.CATEGORIE) LIKE UPPER(:cat)";
    }
    if (dateDebut.isValid()) {
        sql += " AND c.DATE_DEBUT >= TO_DATE(:dateDebut, 'YYYY-MM-DD')";
    }
    if (dureeMin > 0) {
        sql += " AND c.DUREE >= :dureeMin";
    }
    if (dureeMax > 0) {
        sql += " AND c.DUREE <= :dureeMax";
    }
    sql += " ORDER BY c.NOM_COURS ASC";

    QSqlQuery q;
    q.prepare(sql);
    if (!nom.trimmed().isEmpty()) {
        q.bindValue(":nom", "%" + nom.trimmed() + "%");
    }
    if (!niveau.trimmed().isEmpty() && niveau != "Tous") {
        q.bindValue(":niveau", niveau);
    }
    if (!categorie.trimmed().isEmpty() && categorie != "Tous") {
        q.bindValue(":cat", "%" + categorie.trimmed() + "%");
    }
    if (dateDebut.isValid()) {
        q.bindValue(":dateDebut", dateDebut.toString("yyyy-MM-dd"));
    }
    if (dureeMin > 0) {
        q.bindValue(":dureeMin", dureeMin);
    }
    if (dureeMax > 0) {
        q.bindValue(":dureeMax", dureeMax);
    }

    if (q.exec()) {
        while (q.next()) {
            list.append(fromQuery(q));
        }
    }
    return list;
}

QList<Cours> Cours::trier(const QString& critere, const QString& ordre) {
    QList<Cours> list;
    QString col = "c.ID_COURS";
    if (critere == "Nom") col = "c.NOM_COURS";
    else if (critere == "Durée") col = "c.DUREE";
    else if (critere == "Niveau") col = "c.NIVEAU";
    else if (critere == "Catégorie") col = "c.CATEGORIE";
    else if (critere == "Date Début") col = "c.DATE_DEBUT";

    QString dir = (ordre.toUpper() == "DESC" || ordre == "Décroissant") ? "DESC" : "ASC";
    QString sql = QString("SELECT c.ID_COURS, c.NOM_COURS, c.DESCRIPTION, c.DUREE, c.NIVEAU, c.CATEGORIE, "
                          "c.DATE_DEBUT, c.DATE_FIN, c.ID_SALLE, s.NOM_SALLE "
                          "FROM COURS c LEFT JOIN SALLE s ON c.ID_SALLE = s.ID_SALLE ORDER BY %1 %2").arg(col, dir);

    QSqlQuery q(sql);
    while (q.next()) {
        list.append(fromQuery(q));
    }
    return list;
}

QMap<QString, int> Cours::statsParCategorie() {
    QMap<QString, int> res;
    QSqlQuery q("SELECT CATEGORIE, COUNT(*) FROM COURS GROUP BY CATEGORIE");
    while (q.next()) {
        res.insert(q.value(0).toString(), q.value(1).toInt());
    }
    return res;
}

QMap<QString, int> Cours::statsParNiveau() {
    QMap<QString, int> res;
    QSqlQuery q("SELECT NIVEAU, COUNT(*) FROM COURS GROUP BY NIVEAU");
    while (q.next()) {
        res.insert(q.value(0).toString(), q.value(1).toInt());
    }
    return res;
}

QMap<int, int> Cours::statsParMois(int annee) {
    QMap<int, int> res;
    for (int m = 1; m <= 12; ++m) res.insert(m, 0);

    QSqlQuery q;
    q.prepare("SELECT EXTRACT(MONTH FROM DATE_DEBUT), COUNT(*) FROM COURS "
              "WHERE EXTRACT(YEAR FROM DATE_DEBUT) = :annee GROUP BY EXTRACT(MONTH FROM DATE_DEBUT)");
    q.bindValue(":annee", annee);
    if (q.exec()) {
        while (q.next()) {
            res.insert(q.value(0).toInt(), q.value(1).toInt());
        }
    }
    return res;
}

QList<QStringList> Cours::detecterConflits() {
    QList<QStringList> conflits;
    // Détection de chevauchement de dates pour une même salle
    QSqlQuery q("SELECT c1.ID_COURS, c1.NOM_COURS, c2.ID_COURS, c2.NOM_COURS, s.NOM_SALLE, "
                "TO_CHAR(c1.DATE_DEBUT,'DD/MM/YYYY') || ' - ' || TO_CHAR(c1.DATE_FIN,'DD/MM/YYYY'), "
                "TO_CHAR(c2.DATE_DEBUT,'DD/MM/YYYY') || ' - ' || TO_CHAR(c2.DATE_FIN,'DD/MM/YYYY') "
                "FROM COURS c1 "
                "JOIN COURS c2 ON c1.ID_SALLE = c2.ID_SALLE AND c1.ID_COURS < c2.ID_COURS "
                "JOIN SALLE s ON c1.ID_SALLE = s.ID_SALLE "
                "WHERE c1.DATE_DEBUT <= c2.DATE_FIN AND c2.DATE_DEBUT <= c1.DATE_FIN");
    while (q.next()) {
        QStringList item;
        item << q.value(0).toString() << q.value(1).toString()
             << q.value(2).toString() << q.value(3).toString()
             << q.value(4).toString() << q.value(5).toString() << q.value(6).toString();
        conflits.append(item);
    }
    return conflits;
}

QMap<QString, int> Cours::chargeHoraireParCategorie() {
    QMap<QString, int> res;
    QSqlQuery q("SELECT CATEGORIE, SUM(DUREE) FROM COURS GROUP BY CATEGORIE");
    while (q.next()) {
        res.insert(q.value(0).toString(), q.value(1).toInt());
    }
    return res;
}

int Cours::totalHeures() {
    QSqlQuery q("SELECT SUM(DUREE) FROM COURS");
    if (q.next()) return q.value(0).toInt();
    return 0;
}

QString Cours::nomSallePourId(int idSalle) {
    QSqlQuery q;
    q.prepare("SELECT NOM_SALLE FROM SALLE WHERE ID_SALLE = :id");
    q.bindValue(":id", idSalle);
    if (q.exec() && q.next()) {
        return q.value(0).toString();
    }
    return "Aucune";
}
