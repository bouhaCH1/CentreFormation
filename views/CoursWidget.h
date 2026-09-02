#ifndef COURSWIDGET_H
#define COURSWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include "../models/Cours.h"

class StatsCoursWidget;

class CoursWidget : public QWidget {
    Q_OBJECT

public:
    explicit CoursWidget(QWidget *parent = nullptr);

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onSave();
    void onCancel();
    void onSearchToggle();
    void onApplySearch();
    void onClearSearch();
    void onApplySort();
    void onShowStats();
    void onGeneratePDF();
    void onDetectConflits();
    void onCalculChargeHoraire();
    void onTableSelectionChanged();

private:
    void setupUI();
    void loadTableData(const QList<Cours>& list);
    void clearForm();
    void populateForm(const Cours& c);
    bool validateForm();
    void loadSallesInCombo();

    QStackedWidget *m_stackedWidget;

    // Page 0: Liste
    QTableWidget *m_tableWidget;
    QFrame *m_searchPanel;
    QLineEdit *m_searchNom;
    QComboBox *m_searchNiveau;
    QLineEdit *m_searchCategorie;
    QDateEdit *m_searchDateDebut;
    QSpinBox *m_searchDureeMin;
    QSpinBox *m_searchDureeMax;

    QComboBox *m_sortByCombo;
    QComboBox *m_sortOrderCombo;

    QPushButton *m_btnAjouter;
    QPushButton *m_btnModifier;
    QPushButton *m_btnSupprimer;
    QPushButton *m_btnConflits;
    QPushButton *m_btnCharge;
    QPushButton *m_btnPDF;
    QPushButton *m_btnStats;

    // Page 1: Formulaire
    QLabel *m_formTitleLabel;
    QLineEdit *m_nomEdit;
    QTextEdit *m_descEdit;
    QSpinBox *m_dureeSpinBox;
    QComboBox *m_niveauCombo;
    QLineEdit *m_catEdit;
    QDateEdit *m_dateDebutEdit;
    QDateEdit *m_dateFinEdit;
    QComboBox *m_salleCombo;
    QLabel *m_errorLabel;

    // Page 2: Stats
    StatsCoursWidget *m_statsWidget;

    bool m_editMode;
    int m_editId;
};

#endif // COURSWIDGET_H
