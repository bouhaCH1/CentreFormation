#include "CoursWidget.h"
#include "StatsCoursWidget.h"
#include "../models/Salle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QDateTime>
#include <QDialog>

CoursWidget::CoursWidget(QWidget *parent)
    : QWidget(parent), m_editMode(false), m_editId(0) {
    setupUI();
    loadTableData(Cours::afficherAvecSalle());
}

void CoursWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // Header Title Banner
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setObjectName("headerFrame");
    headerFrame->setStyleSheet("QFrame#headerFrame { background: #1E3A8A; border-radius: 8px; }");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    QLabel *headerTitle = new QLabel("📚 GESTION DES COURS DE FORMATION", headerFrame);
    headerTitle->setStyleSheet("color: #FFFFFF !important; font-size: 18px; font-weight: 800; border: none !important; background: transparent !important;");
    headerLayout->addWidget(headerTitle);
    mainLayout->addWidget(headerFrame);

    m_stackedWidget = new QStackedWidget(this);

    // ==========================================
    // PAGE 0 : LISTE & ACTIONS
    // ==========================================
    QWidget *pageListe = new QWidget(this);
    QVBoxLayout *layoutListe = new QVBoxLayout(pageListe);
    layoutListe->setContentsMargins(0, 0, 0, 0);
    layoutListe->setSpacing(12);

    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(8);

    m_btnAjouter = new QPushButton("➕ Ajouter Cours", this);
    m_btnAjouter->setStyleSheet("background-color: #10B981; color: white;");

    m_btnModifier = new QPushButton("✏️ Modifier", this);
    m_btnModifier->setStyleSheet("background-color: #F59E0B; color: white;");

    m_btnSupprimer = new QPushButton("🗑️ Supprimer", this);
    m_btnSupprimer->setStyleSheet("background-color: #EF4444; color: white;");

    m_btnConflits = new QPushButton("⚠️ Conflits Salles", this);
    m_btnConflits->setStyleSheet("background-color: #DC2626; color: white;");

    m_btnCharge = new QPushButton("⏱️ Charge Horaire", this);
    m_btnCharge->setStyleSheet("background-color: #8B5CF6; color: white;");

    QPushButton *btnSearchToggle = new QPushButton("🔍 Filtres", this);
    btnSearchToggle->setStyleSheet("background-color: #3B82F6; color: white;");

    m_btnPDF = new QPushButton("📄 Exporter PDF", this);
    m_btnPDF->setStyleSheet("background-color: #475569; color: white;");

    m_btnStats = new QPushButton("📊 Statistiques", this);
    m_btnStats->setStyleSheet("background-color: #6366F1; color: white;");

    toolbarLayout->addWidget(m_btnAjouter);
    toolbarLayout->addWidget(m_btnModifier);
    toolbarLayout->addWidget(m_btnSupprimer);
    toolbarLayout->addWidget(m_btnConflits);
    toolbarLayout->addWidget(m_btnCharge);
    toolbarLayout->addWidget(btnSearchToggle);
    toolbarLayout->addWidget(m_btnPDF);
    toolbarLayout->addWidget(m_btnStats);
    toolbarLayout->addStretch();

    layoutListe->addLayout(toolbarLayout);

    // Collapsible Search Panel
    m_searchPanel = new QFrame(this);
    m_searchPanel->setObjectName("searchPanel");
    m_searchPanel->setStyleSheet("QFrame#searchPanel { background-color: #F8FAFC; border: 1px solid #CBD5E1; border-radius: 8px; }");
    m_searchPanel->setVisible(false);
    QHBoxLayout *searchLayout = new QHBoxLayout(m_searchPanel);
    searchLayout->setContentsMargins(12, 12, 12, 12);

    m_searchNom = new QLineEdit(this);
    m_searchNom->setPlaceholderText("Nom cours...");

    m_searchNiveau = new QComboBox(this);
    m_searchNiveau->addItems({"Tous", "Debutant", "Intermediaire", "Avance"});

    m_searchCategorie = new QLineEdit(this);
    m_searchCategorie->setPlaceholderText("Catégorie...");

    m_searchDateDebut = new QDateEdit(this);
    m_searchDateDebut->setCalendarPopup(true);
    m_searchDateDebut->setDate(QDate::currentDate().addMonths(-6));

    m_searchDureeMin = new QSpinBox(this);
    m_searchDureeMin->setRange(0, 500);
    m_searchDureeMin->setPrefix("Min: ");

    m_searchDureeMax = new QSpinBox(this);
    m_searchDureeMax->setRange(0, 500);
    m_searchDureeMax->setPrefix("Max: ");

    QPushButton *btnAppliquerSearch = new QPushButton("Appliquer", this);
    btnAppliquerSearch->setStyleSheet("background-color: #2563EB; color: white;");
    QPushButton *btnResetSearch = new QPushButton("Réinitialiser", this);
    btnResetSearch->setStyleSheet("background-color: #64748B; color: white;");

    searchLayout->addWidget(new QLabel("Nom:"));
    searchLayout->addWidget(m_searchNom);
    searchLayout->addWidget(new QLabel("Niveau:"));
    searchLayout->addWidget(m_searchNiveau);
    searchLayout->addWidget(new QLabel("Catégorie:"));
    searchLayout->addWidget(m_searchCategorie);
    searchLayout->addWidget(new QLabel("Durée:"));
    searchLayout->addWidget(m_searchDureeMin);
    searchLayout->addWidget(m_searchDureeMax);
    searchLayout->addWidget(btnAppliquerSearch);
    searchLayout->addWidget(btnResetSearch);

    layoutListe->addWidget(m_searchPanel);

    // Sort Bar
    QHBoxLayout *sortLayout = new QHBoxLayout();
    QLabel *lblSort = new QLabel("Trier par:", this);
    lblSort->setStyleSheet("font-weight: 700; color: #475569;");
    sortLayout->addWidget(lblSort);

    m_sortByCombo = new QComboBox(this);
    m_sortByCombo->addItems({"ID", "Nom", "Durée", "Niveau", "Catégorie", "Date Début"});
    sortLayout->addWidget(m_sortByCombo);

    m_sortOrderCombo = new QComboBox(this);
    m_sortOrderCombo->addItems({"Croissant", "Décroissant"});
    sortLayout->addWidget(m_sortOrderCombo);

    QPushButton *btnAppliquerSort = new QPushButton("Trier", this);
    btnAppliquerSort->setStyleSheet("background-color: #64748B; color: white;");
    sortLayout->addWidget(btnAppliquerSort);
    sortLayout->addStretch();

    layoutListe->addLayout(sortLayout);

    // Table Widget
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(9);
    m_tableWidget->setHorizontalHeaderLabels({"ID", "Nom Cours", "Durée (h)", "Niveau", "Catégorie", "Date Début", "Date Fin", "ID Salle", "Salle Affectée"});
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setAlternatingRowColors(true);

    layoutListe->addWidget(m_tableWidget);
    m_stackedWidget->addWidget(pageListe);

    // ==========================================
    // PAGE 1 : FORMULAIRE (AJOUT / MODIF)
    // ==========================================
    QWidget *pageForm = new QWidget(this);
    QVBoxLayout *layoutFormContainer = new QVBoxLayout(pageForm);
    layoutFormContainer->setContentsMargins(10, 10, 10, 10);

    QFrame *formCard = new QFrame(pageForm);
    formCard->setObjectName("formCard");
    formCard->setStyleSheet("QFrame#formCard { background-color: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px; }");
    QVBoxLayout *formCardLayout = new QVBoxLayout(formCard);
    formCardLayout->setContentsMargins(24, 24, 24, 24);
    formCardLayout->setSpacing(16);

    m_formTitleLabel = new QLabel("➕ Ajouter un nouveau Cours", formCard);
    m_formTitleLabel->setStyleSheet("font-size: 18px; font-weight: 800; color: #1E3A8A !important; margin-bottom: 10px; border: none !important; background: transparent !important;");
    formCardLayout->addWidget(m_formTitleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(16);

    m_nomEdit = new QLineEdit(formCard);
    m_nomEdit->setPlaceholderText("Ex: Programmmation C++ Avancée");

    m_descEdit = new QTextEdit(formCard);
    m_descEdit->setPlaceholderText("Description du cours...");
    m_descEdit->setMaximumHeight(80);

    m_dureeSpinBox = new QSpinBox(formCard);
    m_dureeSpinBox->setRange(1, 500);
    m_dureeSpinBox->setValue(30);
    m_dureeSpinBox->setSuffix(" heures");

    m_niveauCombo = new QComboBox(formCard);
    m_niveauCombo->addItems({"Debutant", "Intermediaire", "Avance"});

    m_catEdit = new QLineEdit(formCard);
    m_catEdit->setPlaceholderText("Ex: Informatique, Base de données...");

    m_dateDebutEdit = new QDateEdit(formCard);
    m_dateDebutEdit->setCalendarPopup(true);
    m_dateDebutEdit->setDate(QDate::currentDate());

    m_dateFinEdit = new QDateEdit(formCard);
    m_dateFinEdit->setCalendarPopup(true);
    m_dateFinEdit->setDate(QDate::currentDate().addMonths(1));

    m_salleCombo = new QComboBox(formCard);

    formLayout->addRow("Nom du Cours *:", m_nomEdit);
    formLayout->addRow("Description:", m_descEdit);
    formLayout->addRow("Durée Totale *:", m_dureeSpinBox);
    formLayout->addRow("Niveau requis *:", m_niveauCombo);
    formLayout->addRow("Catégorie *:", m_catEdit);
    formLayout->addRow("Date de Début *:", m_dateDebutEdit);
    formLayout->addRow("Date de Fin *:", m_dateFinEdit);
    formLayout->addRow("Salle Affectée:", m_salleCombo);

    formCardLayout->addLayout(formLayout);

    m_errorLabel = new QLabel(formCard);
    m_errorLabel->setStyleSheet("color: #EF4444 !important; font-weight: 700; font-size: 13px; border: none !important; background: transparent !important;");
    formCardLayout->addWidget(m_errorLabel);

    QHBoxLayout *formButtonsLayout = new QHBoxLayout();
    formButtonsLayout->setContentsMargins(0, 10, 0, 0);
    formButtonsLayout->setSpacing(12);

    QPushButton *btnSave = new QPushButton("💾 Enregistrer le Cours", formCard);
    btnSave->setStyleSheet("background-color: #2563EB; color: white; font-weight: 700; font-size: 14px; padding: 10px 20px;");
    
    QPushButton *btnCancel = new QPushButton("❌ Annuler", formCard);
    btnCancel->setStyleSheet("background-color: #64748B; color: white; font-weight: 700; font-size: 14px; padding: 10px 20px;");

    formButtonsLayout->addWidget(btnSave);
    formButtonsLayout->addWidget(btnCancel);
    formButtonsLayout->addStretch();

    formCardLayout->addLayout(formButtonsLayout);

    layoutFormContainer->addWidget(formCard);
    layoutFormContainer->addStretch();

    m_stackedWidget->addWidget(pageForm);

    // ==========================================
    // PAGE 2 : STATISTIQUES
    // ==========================================
    QWidget *pageStats = new QWidget(this);
    QVBoxLayout *layoutStats = new QVBoxLayout(pageStats);

    m_statsWidget = new StatsCoursWidget(pageStats);
    layoutStats->addWidget(m_statsWidget);

    QPushButton *btnBackFromStats = new QPushButton("⬅️ Retour à la liste", pageStats);
    btnBackFromStats->setStyleSheet("background-color: #2563EB; color: white; font-weight: 700; padding: 10px 20px; border-radius: 6px;");
    layoutStats->addWidget(btnBackFromStats, 0, Qt::AlignLeft);

    m_stackedWidget->addWidget(pageStats);

    mainLayout->addWidget(m_stackedWidget);

    // Connections
    connect(m_btnAjouter, &QPushButton::clicked, this, &CoursWidget::onAjouter);
    connect(m_btnModifier, &QPushButton::clicked, this, &CoursWidget::onModifier);
    connect(m_btnSupprimer, &QPushButton::clicked, this, &CoursWidget::onSupprimer);
    connect(m_btnConflits, &QPushButton::clicked, this, &CoursWidget::onDetectConflits);
    connect(m_btnCharge, &QPushButton::clicked, this, &CoursWidget::onCalculChargeHoraire);
    connect(btnSearchToggle, &QPushButton::clicked, this, &CoursWidget::onSearchToggle);
    connect(btnAppliquerSearch, &QPushButton::clicked, this, &CoursWidget::onApplySearch);
    connect(btnResetSearch, &QPushButton::clicked, this, &CoursWidget::onClearSearch);
    connect(btnAppliquerSort, &QPushButton::clicked, this, &CoursWidget::onApplySort);
    connect(m_btnPDF, &QPushButton::clicked, this, &CoursWidget::onGeneratePDF);
    connect(m_btnStats, &QPushButton::clicked, this, &CoursWidget::onShowStats);

    connect(btnSave, &QPushButton::clicked, this, &CoursWidget::onSave);
    connect(btnCancel, &QPushButton::clicked, this, &CoursWidget::onCancel);
    connect(btnBackFromStats, &QPushButton::clicked, this, &CoursWidget::onCancel);

    // Double-click row directly edits that course
    connect(m_tableWidget, &QTableWidget::cellDoubleClicked, this, &CoursWidget::onModifier);
}

void CoursWidget::loadSallesInCombo() {
    m_salleCombo->clear();
    m_salleCombo->addItem("Non affecté", 0);

    QList<Salle> salles = Salle::afficher();
    for (const Salle& s : salles) {
        if (s.isDisponible()) {
            m_salleCombo->addItem(QString("%1 (Cap: %2 places)").arg(s.getNom()).arg(s.getCapacite()), s.getId());
        }
    }
}

void CoursWidget::loadTableData(const QList<Cours>& list) {
    m_tableWidget->setRowCount(0);
    for (int i = 0; i < list.size(); ++i) {
        const Cours& c = list[i];
        m_tableWidget->insertRow(i);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(c.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *nomItem = new QTableWidgetItem(c.getNom());
        nomItem->setFont(QFont("Segoe UI", 10, QFont::Bold));

        QTableWidgetItem *dureeItem = new QTableWidgetItem(QString("%1h").arg(c.getDuree()));
        dureeItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *niveauItem = new QTableWidgetItem(c.getNiveau());
        niveauItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *catItem = new QTableWidgetItem(c.getCategorie());
        catItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *debutItem = new QTableWidgetItem(c.getDateDebut().toString("dd/MM/yyyy"));
        debutItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *finItem = new QTableWidgetItem(c.getDateFin().toString("dd/MM/yyyy"));
        finItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *idSalleItem = new QTableWidgetItem(c.getIdSalle() > 0 ? QString::number(c.getIdSalle()) : "-");
        idSalleItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *salleItem = new QTableWidgetItem(c.getNomSalle().isEmpty() ? "Non affecté" : c.getNomSalle());
        salleItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        if (!c.getNomSalle().isEmpty()) {
            salleItem->setForeground(QColor("#2563EB"));
        } else {
            salleItem->setForeground(QColor("#94A3B8"));
        }

        m_tableWidget->setItem(i, 0, idItem);
        m_tableWidget->setItem(i, 1, nomItem);
        m_tableWidget->setItem(i, 2, dureeItem);
        m_tableWidget->setItem(i, 3, niveauItem);
        m_tableWidget->setItem(i, 4, catItem);
        m_tableWidget->setItem(i, 5, debutItem);
        m_tableWidget->setItem(i, 6, finItem);
        m_tableWidget->setItem(i, 7, idSalleItem);
        m_tableWidget->setItem(i, 8, salleItem);
    }
}

void CoursWidget::onTableSelectionChanged() {
    // Keep buttons active
}

void CoursWidget::onAjouter() {
    m_editMode = false;
    m_editId = 0;
    m_formTitleLabel->setText("➕ Ajouter un nouveau Cours");
    loadSallesInCombo();
    clearForm();
    m_stackedWidget->setCurrentIndex(1);
}

void CoursWidget::onModifier() {
    int row = m_tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord cliquer sur un cours dans le tableau pour le modifier.");
        return;
    }

    m_editMode = true;
    m_editId = m_tableWidget->item(row, 0)->text().toInt();

    loadSallesInCombo();

    Cours c;
    c.setId(m_editId);
    c.setNom(m_tableWidget->item(row, 1)->text());

    QString dureeText = m_tableWidget->item(row, 2)->text();
    dureeText.replace("h", "");
    c.setDuree(dureeText.toInt());

    c.setNiveau(m_tableWidget->item(row, 3)->text());
    c.setCategorie(m_tableWidget->item(row, 4)->text());
    c.setDateDebut(QDate::fromString(m_tableWidget->item(row, 5)->text(), "dd/MM/yyyy"));
    c.setDateFin(QDate::fromString(m_tableWidget->item(row, 6)->text(), "dd/MM/yyyy"));
    c.setIdSalle(m_tableWidget->item(row, 7)->text() == "-" ? 0 : m_tableWidget->item(row, 7)->text().toInt());

    m_formTitleLabel->setText(QString("✏️ Modifier le Cours (ID: %1)").arg(m_editId));
    populateForm(c);
    m_stackedWidget->setCurrentIndex(1);
}

void CoursWidget::onSupprimer() {
    int row = m_tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord cliquer sur un cours dans le tableau pour le supprimer.");
        return;
    }

    int id = m_tableWidget->item(row, 0)->text().toInt();
    QString nom = m_tableWidget->item(row, 1)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation de suppression",
        QString("Voulez-vous vraiment supprimer le cours '%1' (ID: %2) ?").arg(nom).arg(id),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        Cours c;
        c.setId(id);
        if (c.supprimer()) {
            QMessageBox::information(this, "Succès", "Cours supprimé avec succès.");
            loadTableData(Cours::afficherAvecSalle());
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de supprimer ce cours.");
        }
    }
}

void CoursWidget::onSave() {
    if (!validateForm()) return;

    int idSalle = m_salleCombo->currentData().toInt();

    Cours c(
        m_editId,
        m_nomEdit->text().trimmed(),
        m_descEdit->toPlainText().trimmed(),
        m_dureeSpinBox->value(),
        m_niveauCombo->currentText(),
        m_catEdit->text().trimmed(),
        m_dateDebutEdit->date(),
        m_dateFinEdit->date(),
        idSalle
    );

    bool ok = m_editMode ? c.modifier() : c.ajouter();

    if (ok) {
        QMessageBox::information(this, "Succès", m_editMode ? "Cours modifié avec succès dans Oracle BD!" : "Cours ajouté avec succès dans Oracle BD!");
        m_stackedWidget->setCurrentIndex(0);
        loadTableData(Cours::afficherAvecSalle());
    } else {
        m_errorLabel->setText("Erreur lors de l'enregistrement dans la base de données Oracle.");
    }
}

void CoursWidget::onCancel() {
    m_stackedWidget->setCurrentIndex(0);
}

void CoursWidget::onSearchToggle() {
    m_searchPanel->setVisible(!m_searchPanel->isVisible());
}

void CoursWidget::onApplySearch() {
    QString nom = m_searchNom->text();
    QString niveau = m_searchNiveau->currentText();
    QString cat = m_searchCategorie->text();
    QDate dateDebut = m_searchDateDebut->date();
    int dureeMin = m_searchDureeMin->value();
    int dureeMax = m_searchDureeMax->value();

    loadTableData(Cours::rechercher(nom, niveau, cat, dateDebut, dureeMin, dureeMax));
}

void CoursWidget::onClearSearch() {
    m_searchNom->clear();
    m_searchNiveau->setCurrentIndex(0);
    m_searchCategorie->clear();
    m_searchDureeMin->setValue(0);
    m_searchDureeMax->setValue(0);

    loadTableData(Cours::afficherAvecSalle());
}

void CoursWidget::onApplySort() {
    QString critere = m_sortByCombo->currentText();
    QString ordre = m_sortOrderCombo->currentText();
    loadTableData(Cours::trier(critere, ordre));
}

void CoursWidget::onShowStats() {
    m_statsWidget->refreshStats();
    m_stackedWidget->setCurrentIndex(2);
}

void CoursWidget::onDetectConflits() {
    QList<QStringList> conflits = Cours::detecterConflits();
    if (conflits.isEmpty()) {
        QMessageBox::information(this, "Conflits Salles", "✅ Aucun conflit de salle détecté ! Toutes les salles occupées ont des plannings distincts.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("⚠️ Conflits d'occupation des Salles");
    dialog.resize(700, 350);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *lbl = new QLabel(QString("⚠️ %1 conflit(s) de chevauchement de dates détecté(s) :").arg(conflits.size()), &dialog);
    lbl->setStyleSheet("font-weight: bold; color: #DC2626; font-size: 14px;");
    layout->addWidget(lbl);

    QTableWidget *tw = new QTableWidget(&dialog);
    tw->setColumnCount(5);
    tw->setHorizontalHeaderLabels({"Cours 1", "Cours 2", "Salle", "Période 1", "Période 2"});
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < conflits.size(); ++i) {
        tw->insertRow(i);
        const QStringList& conf = conflits[i];
        tw->setItem(i, 0, new QTableWidgetItem(conf[1]));
        tw->setItem(i, 1, new QTableWidgetItem(conf[3]));
        tw->setItem(i, 2, new QTableWidgetItem(conf[4]));
        tw->setItem(i, 3, new QTableWidgetItem(conf[5]));
        tw->setItem(i, 4, new QTableWidgetItem(conf[6]));
    }
    layout->addWidget(tw);

    QPushButton *btnClose = new QPushButton("Fermer", &dialog);
    connect(btnClose, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(btnClose, 0, Qt::AlignRight);

    dialog.exec();
}

void CoursWidget::onCalculChargeHoraire() {
    QMap<QString, int> charge = Cours::chargeHoraireParCategorie();
    int total = Cours::totalHeures();

    QString msg = "📊 Charge horaire globale par catégorie :\n\n";
    for (auto it = charge.begin(); it != charge.end(); ++it) {
        msg += QString("  • %1 : %2 heures\n").arg(it.key()).arg(it.value());
    }
    msg += QString("\n⏱️ TOTAL GLOBAL : %1 heures de formation").arg(total);

    QMessageBox::information(this, "Calcul de Charge Horaire", msg);
}

void CoursWidget::onGeneratePDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Catalogue des Cours PDF", "Catalogue_Cours.pdf", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);

    QList<Cours> list = Cours::afficherAvecSalle();
    int totalHours = Cours::totalHeures();

    QString html = QString(R"(
        <!DOCTYPE html>
        <html>
        <head>
        <style>
            body { font-family: 'Segoe UI', Arial, sans-serif; color: #0F172A; margin: 15px; }
            h1 { color: #1E3A8A; text-align: center; margin-bottom: 4px; font-size: 20px; font-weight: bold; }
            p.sub { text-align: center; color: #64748B; font-size: 11px; margin-top: 0px; margin-bottom: 16px; }
            table.summary { width: 100%%; border: 1px solid #CBD5E1; background-color: #F8FAFC; margin-bottom: 20px; border-collapse: collapse; }
            table.summary td { padding: 10px; font-size: 12px; text-align: center; font-weight: bold; }
            table.grid { width: 100%%; border-collapse: collapse; margin-top: 10px; }
            table.grid th { background-color: #0F172A; color: #FFFFFF; font-size: 12px; font-weight: bold; padding: 10px; text-align: center; border: 1px solid #0F172A; }
            table.grid td { padding: 10px; font-size: 12px; border: 1px solid #CBD5E1; }
            .footer { margin-top: 30px; text-align: center; font-size: 10px; color: #64748B; border-top: 1px solid #CBD5E1; padding-top: 8px; }
        </style>
        </head>
        <body>
            <h1>CENTRE DE FORMATION — CATALOGUE DES COURS</h1>
            <p class="sub">Document officiel de planification généré le %1</p>

            <table class="summary">
                <tr>
                    <td>Total Cours : <font color="#2563EB">%2</font></td>
                    <td>Volume Horaire Total : <font color="#8B5CF6">%3 heures</font></td>
                </tr>
            </table>

            <table class="grid">
                <thead>
                    <tr>
                        <th width="8%%">ID</th>
                        <th width="26%%">Nom du Cours</th>
                        <th width="10%%">Durée</th>
                        <th width="14%%">Niveau</th>
                        <th width="16%%">Catégorie</th>
                        <th width="16%%">Période</th>
                        <th width="10%%">Salle</th>
                    </tr>
                </thead>
                <tbody>
    )").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"))
       .arg(list.size())
       .arg(totalHours);

    for (int i = 0; i < list.size(); ++i) {
        const Cours& c = list[i];
        QString bg = (i % 2 == 0) ? "#FFFFFF" : "#F8FAFC";
        QString salleBadge = c.getNomSalle().isEmpty()
            ? "<font color='#64748B'><i>Non affecté</i></font>"
            : QString("<font color='#2563EB'><b>%1</b></font>").arg(c.getNomSalle());

        html += QString(R"(
            <tr bgcolor="%1">
                <td align="center"><b>#%2</b></td>
                <td><b>%3</b></td>
                <td align="center">%4h</td>
                <td align="center">%5</td>
                <td align="center">%6</td>
                <td align="center">%7 - %8</td>
                <td align="center">%9</td>
            </tr>
        )").arg(bg)
           .arg(c.getId())
           .arg(c.getNom())
           .arg(c.getDuree())
           .arg(c.getNiveau())
           .arg(c.getCategorie())
           .arg(c.getDateDebut().toString("dd/MM/yy"))
           .arg(c.getDateFin().toString("dd/MM/yy"))
           .arg(salleBadge);
    }

    html += R"(
                </tbody>
            </table>

            <div class="footer">
                Centre de Formation ESPRIT • Module Salles & Cours (Qt 6 & Oracle XE)
            </div>
        </body>
        </html>
    )";

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Exportation Réussie");
    msgBox.setText("✅ Fichier PDF généré avec succès !");
    msgBox.setInformativeText(QString("Emplacement :\n%1").arg(fileName));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void CoursWidget::clearForm() {
    m_nomEdit->clear();
    m_descEdit->clear();
    m_dureeSpinBox->setValue(30);
    m_niveauCombo->setCurrentIndex(0);
    m_catEdit->clear();
    m_dateDebutEdit->setDate(QDate::currentDate());
    m_dateFinEdit->setDate(QDate::currentDate().addMonths(1));
    m_salleCombo->setCurrentIndex(0);
    m_errorLabel->clear();
}

void CoursWidget::populateForm(const Cours& c) {
    m_nomEdit->setText(c.getNom());
    m_descEdit->setText(c.getDescription());
    m_dureeSpinBox->setValue(c.getDuree());
    m_niveauCombo->setCurrentText(c.getNiveau());
    m_catEdit->setText(c.getCategorie());
    m_dateDebutEdit->setDate(c.getDateDebut());
    m_dateFinEdit->setDate(c.getDateFin());

    int index = m_salleCombo->findData(c.getIdSalle());
    if (index >= 0) m_salleCombo->setCurrentIndex(index);
    else m_salleCombo->setCurrentIndex(0);

    m_errorLabel->clear();
}

bool CoursWidget::validateForm() {
    if (m_nomEdit->text().trimmed().isEmpty()) {
        m_errorLabel->setText("❌ Le nom du cours est obligatoire.");
        return false;
    }
    if (m_catEdit->text().trimmed().isEmpty()) {
        m_errorLabel->setText("❌ La catégorie est obligatoire.");
        return false;
    }
    if (m_dateFinEdit->date() < m_dateDebutEdit->date()) {
        m_errorLabel->setText("❌ La date de fin ne peut pas être antérieure à la date de début.");
        return false;
    }
    m_errorLabel->clear();
    return true;
}
