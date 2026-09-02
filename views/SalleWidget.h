#ifndef SALLEWIDGET_H
#define SALLEWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include "../models/Salle.h"

class StatsSalleWidget;

class SalleWidget : public QWidget {
    Q_OBJECT

public:
    explicit SalleWidget(QWidget *parent = nullptr);

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onToggleDisponibilite();
    void onSave();
    void onCancel();
    void onSearchToggle();
    void onApplySearch();
    void onClearSearch();
    void onApplySort();
    void onShowStats();
    void onGeneratePDF();
    void onTableSelectionChanged();

private:
    void setupUI();
    void loadTableData(const QList<Salle>& list);
    void clearForm();
    void populateForm(const Salle& s);
    bool validateForm();

    QStackedWidget *m_stackedWidget;

    // Page 0: Liste
    QTableWidget *m_tableWidget;
    QFrame *m_searchPanel;
    QLineEdit *m_searchNom;
    QComboBox *m_searchType;
    QSpinBox *m_searchCapMin;
    QSpinBox *m_searchCapMax;
    QComboBox *m_searchDispo;

    QComboBox *m_sortByCombo;
    QComboBox *m_sortOrderCombo;

    QPushButton *m_btnAjouter;
    QPushButton *m_btnModifier;
    QPushButton *m_btnSupprimer;
    QPushButton *m_btnDispo;
    QPushButton *m_btnPDF;
    QPushButton *m_btnStats;

    // Page 1: Formulaire
    QLabel *m_formTitleLabel;
    QLineEdit *m_nomEdit;
    QSpinBox *m_capSpinBox;
    QComboBox *m_typeCombo;
    QLineEdit *m_equipEdit;
    QCheckBox *m_dispoCheck;
    QLabel *m_errorLabel;

    // Page 2: Stats
    StatsSalleWidget *m_statsWidget;

    bool m_editMode;
    int m_editId;
};

#endif // SALLEWIDGET_H
