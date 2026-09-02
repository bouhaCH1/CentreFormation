#ifndef SALLE_H
#define SALLE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QPair>
#include <QSqlQuery>

class Salle {
public:
    Salle();
    Salle(int id, const QString& nom, int capacite, const QString& type, const QString& equipement, bool disponible = true);

    // Getters
    int getId() const { return m_id; }
    QString getNom() const { return m_nom; }
    int getCapacite() const { return m_capacite; }
    QString getType() const { return m_type; }
    QString getEquipement() const { return m_equipement; }
    bool isDisponible() const { return m_disponible; }

    // Setters
    void setId(int id) { m_id = id; }
    void setNom(const QString& nom) { m_nom = nom; }
    void setCapacite(int cap) { m_capacite = cap; }
    void setType(const QString& type) { m_type = type; }
    void setEquipement(const QString& eq) { m_equipement = eq; }
    void setDisponible(bool dispo) { m_disponible = dispo; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer();
    static QList<Salle> afficher();

    // Métiers
    static QList<Salle> rechercher(const QString& nom, const QString& type, int capMin, int capMax, int disponible);
    static QList<Salle> trier(const QString& critere, const QString& ordre);
    static QMap<QString, int> statsParType();
    static int countDisponibles();
    static int countIndisponibles();
    static QMap<QString, int> statsCapaciteMoyenneParType();
    static bool toggleDisponibilite(int id);
    static QList<QPair<QString, QString>> getCoursEnSalle(int idSalle);

private:
    int m_id;
    QString m_nom;
    int m_capacite;
    QString m_type;
    QString m_equipement;
    bool m_disponible;

    static Salle fromQuery(const QSqlQuery& query);
};

#endif // SALLE_H
