#include "Cours.h"
#include "../database/DatabaseManager.h"
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
    Cours c(
        query.value("ID_COURS").toInt(),
        query.value("NOM_COURS").toString(),
        query.value("DESCRIPTION").toString(),
        query.value("DUREE").toInt(),
        query.value("NIVEAU").toString(),
        query.value("CATEGORIE").toString(),
        query.value("DATE_DEBUT").toDate(),
        query.value("DATE_FIN").toDate(),
        query.value("ID_SALLE").toInt()
    );
    if (query.record().indexOf("NOM_SALLE") >= 0) {
        c.setNomSalle(query.value("NOM_SALLE").toString());
    }
    return c;
}

bool Cours::ajouter() {
    QSqlQuery query;
    if (m_idSalle > 0) {
        query.prepare("INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE) "
                      "VALUES (:nom, :desc, :duree, :niv, :cat, :dd, :df, :idSalle)");
        query.bindValue(":idSalle", m_idSalle);
    } else {
        query.prepare("INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE) "
                      "VALUES (:nom, :desc, :duree, :niv, :cat, :dd, :df, NULL)");
    }
    query.bindValue(":nom", m_nom);
    query.bindValue(":desc", m_description);
    query.bindValue(":duree", m_duree);
    query.bindValue(":niv", m_niveau);
    query.bindValue(":cat", m_categorie);
    query.bindValue(":dd", m_dateDebut);
    query.bindValue(":df", m_dateFin);

    return query.exec();
}

bool Cours::modifier() {
    QSqlQuery query;
    if (m_idSalle > 0) {
        query.prepare("UPDATE COURS SET NOM_COURS = :nom, DESCRIPTION = :desc, DUREE = :duree, "
                      "NIVEAU = :niv, CATEGORIE = :cat, DATE_DEBUT = :dd, DATE_FIN = :df, ID_SALLE = :idSalle "
                      "WHERE ID_COURS = :id");
        query.bindValue(":idSalle", m_idSalle);
    } else {
        query.prepare("UPDATE COURS SET NOM_COURS = :nom, DESCRIPTION = :desc, DUREE = :duree, "
                      "NIVEAU = :niv, CATEGORIE = :cat, DATE_DEBUT = :dd, DATE_FIN = :df, ID_SALLE = NULL "
                      "WHERE ID_COURS = :id");
    }
    query.bindValue(":nom", m_nom);
    query.bindValue(":desc", m_description);
    query.bindValue(":duree", m_duree);
    query.bindValue(":niv", m_niveau);
    query.bindValue(":cat", m_categorie);
    query.bindValue(":dd", m_dateDebut);
    query.bindValue(":df", m_dateFin);
    query.bindValue(":id", m_id);

    return query.exec();
}

bool Cours::supprimer() {
    QSqlQuery query;
    query.prepare("DELETE FROM COURS WHERE ID_COURS = :id");
    query.bindValue(":id", m_id);
    return query.exec();
}

QList<Cours> Cours::afficher() {
    QList<Cours> list;
    QSqlQuery query("SELECT ID_COURS, NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE FROM COURS ORDER BY ID_COURS ASC");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

QList<Cours> Cours::afficherAvecSalle() {
    QList<Cours> list;
    QSqlQuery query("SELECT c.ID_COURS, c.NOM_COURS, c.DESCRIPTION, c.DUREE, c.NIVEAU, c.CATEGORIE, c.DATE_DEBUT, c.DATE_FIN, c.ID_SALLE, s.NOM_SALLE "
                    "FROM COURS c LEFT JOIN SALLE s ON c.ID_SALLE = s.ID_SALLE ORDER BY c.ID_COURS ASC");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

QList<Cours> Cours::rechercher(const QString& nom, const QString& niveau, const QString& categorie, const QDate& dateDebut, int dureeMin, int dureeMax) {
    QList<Cours> list;
    QString sql = "SELECT c.ID_COURS, c.NOM_COURS, c.DESCRIPTION, c.DUREE, c.NIVEAU, c.CATEGORIE, c.DATE_DEBUT, c.DATE_FIN, c.ID_SALLE, s.NOM_SALLE "
                  "FROM COURS c LEFT JOIN SALLE s ON c.ID_SALLE = s.ID_SALLE WHERE 1=1";

    if (!nom.isEmpty()) sql += " AND LOWER(c.NOM_COURS) LIKE '%" + nom.toLower() + "%'";
    if (!niveau.isEmpty() && niveau != "Tous") sql += " AND c.NIVEAU = '" + niveau + "'";
    if (!categorie.isEmpty()) sql += " AND LOWER(c.CATEGORIE) LIKE '%" + categorie.toLower() + "%'";
    if (dureeMin > 0) sql += QString(" AND c.DUREE >= %1").arg(dureeMin);
    if (dureeMax > 0) sql += QString(" AND c.DUREE <= %1").arg(dureeMax);

    sql += " ORDER BY c.ID_COURS ASC";

    QSqlQuery query(sql);
    while (query.next()) {
        list.append(fromQuery(query));
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

    QString dir = (ordre == "Décroissant") ? "DESC" : "ASC";
    QString sql = QString("SELECT c.ID_COURS, c.NOM_COURS, c.DESCRIPTION, c.DUREE, c.NIVEAU, c.CATEGORIE, c.DATE_DEBUT, c.DATE_FIN, c.ID_SALLE, s.NOM_SALLE "
                          "FROM COURS c LEFT JOIN SALLE s ON c.ID_SALLE = s.ID_SALLE ORDER BY %1 %2").arg(col).arg(dir);

    QSqlQuery query(sql);
    while (query.next()) {
        list.append(fromQuery(query));
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
    if (idSalle <= 0) return "Non affecté";
    QSqlQuery q;
    q.prepare("SELECT NOM_SALLE FROM SALLE WHERE ID_SALLE = :id");
    q.bindValue(":id", idSalle);
    if (q.exec() && q.next()) return q.value(0).toString();
    return "Inconnue";
}

QString Cours::analyseIAPlanning() {
    QList<Cours> list = afficherAvecSalle();
    QList<QStringList> conflits = detecterConflits();
    int total = list.size();
    int nonAffectes = 0;

    for (const Cours& c : list) {
        if (c.getIdSalle() <= 0) nonAffectes++;
    }

    QString report = "🤖 <b>Rapport d'Intelligence Artificielle - Planification des Cours</b><br><br>";
    report += QString("📊 <b>Diagnostic Prédictif du Catalogue :</b><br>");
    report += QString("• Total des Cours au catalogue : <b>%1</b><br>").arg(total);
    report += QString("• Volume Horaire Cumulé : <font color='#8B5CF6'><b>%1 Heures</b></font><br>").arg(totalHeures());
    report += QString("• Cours sans salle affectée : <font color='#F59E0B'><b>%1</b></font><br>").arg(nonAffectes);
    report += QString("• Conflits d'occupation détectés : <font color='%1'><b>%2</b></font><br><br>")
              .arg(conflits.isEmpty() ? "#059669" : "#DC2626")
              .arg(conflits.size());

    report += "💡 <b>Recommandations de l'Assistant IA :</b><br>";

    if (!conflits.isEmpty()) {
        report += "  • 🚨 <b>Alerte Conflit :</b> " + QString::number(conflits.size()) + " chevauchement(s) détecté(s). L'IA préconise de réaffecter l'un des deux cours vers une salle disponible.<br>";
    } else {
        report += "  • ✅ <b>Planning Sécurisé :</b> Aucun conflit temporel de salle n'est à déplorer.<br>";
    }

    if (nonAffectes > 0) {
        report += QString("  • ⚡ <b>Optimisation d'Affectation :</b> %1 cours n'ont pas encore de salle attribuée. L'IA suggère d'utiliser l'option Modifier pour les affecter à des salles 'Cours' ou 'TP'.<br>").arg(nonAffectes);
    }

    for (const Cours& c : list) {
        if (c.getDuree() >= 50) {
            report += QString("  • ⏱️ <b>%1</b> (%2h) est une formation longue. L'IA recommande un suivi pédagogique hebdomadaire.<br>").arg(c.getNom()).arg(c.getDuree());
        }
    }

    return report;
}
