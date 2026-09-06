#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QDate>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QSqlQuery>

class Cours {
public:
    Cours();
    Cours(int id, const QString& nom, const QString& description, int duree,
          const QString& niveau, const QString& categorie,
          const QDate& dateDebut, const QDate& dateFin, int idSalle = 0);

    // Getters
    int getId() const { return m_id; }
    QString getNom() const { return m_nom; }
    QString getDescription() const { return m_description; }
    int getDuree() const { return m_duree; }
    QString getNiveau() const { return m_niveau; }
    QString getCategorie() const { return m_categorie; }
    QDate getDateDebut() const { return m_dateDebut; }
    QDate getDateFin() const { return m_dateFin; }
    int getIdSalle() const { return m_idSalle; }
    QString getNomSalle() const { return m_nomSalle; }

    // Setters
    void setId(int id) { m_id = id; }
    void setNom(const QString& nom) { m_nom = nom; }
    void setDescription(const QString& desc) { m_description = desc; }
    void setDuree(int duree) { m_duree = duree; }
    void setNiveau(const QString& niveau) { m_niveau = niveau; }
    void setCategorie(const QString& cat) { m_categorie = cat; }
    void setDateDebut(const QDate& d) { m_dateDebut = d; }
    void setDateFin(const QDate& d) { m_dateFin = d; }
    void setIdSalle(int idSalle) { m_idSalle = idSalle; }
    void setNomSalle(const QString& nomSalle) { m_nomSalle = nomSalle; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer();
    static QList<Cours> afficher();
    static QList<Cours> afficherAvecSalle();

    // Métiers
    static QList<Cours> rechercher(const QString& nom, const QString& niveau,
                                    const QString& categorie, const QDate& dateDebut,
                                    int dureeMin, int dureeMax);
    static QList<Cours> trier(const QString& critere, const QString& ordre);
    static QMap<QString, int> statsParCategorie();
    static QMap<QString, int> statsParNiveau();
    static QMap<int, int> statsParMois(int annee);
    static QList<QStringList> detecterConflits();
    static QMap<QString, int> chargeHoraireParCategorie();
    static int totalHeures();
    static QString nomSallePourId(int idSalle);

    // Advanced AI Métier
    static QString analyseIAPlanning();

private:
    int m_id;
    QString m_nom;
    QString m_description;
    int m_duree;
    QString m_niveau;
    QString m_categorie;
    QDate m_dateDebut;
    QDate m_dateFin;
    int m_idSalle;
    QString m_nomSalle;

    static Cours fromQuery(const QSqlQuery& query);
};

#endif // COURS_H
