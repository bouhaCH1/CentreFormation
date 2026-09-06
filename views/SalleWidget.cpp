#include "SalleWidget.h"
#include "StatsSalleWidget.h"
#include "../models/QRCodeGenerator.h"
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

SalleWidget::SalleWidget(QWidget *parent)
    : QWidget(parent), m_editMode(false), m_editId(0) {
    setupUI();
    loadTableData(Salle::afficher());
}

void SalleWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // Header Title Banner
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setObjectName("headerFrame");
    headerFrame->setStyleSheet("QFrame#headerFrame { background: #1E3A8A; border-radius: 8px; }");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    QLabel *headerTitle = new QLabel("🏫 GESTION DES SALLES DE FORMATION", headerFrame);
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

    m_btnAjouter = new QPushButton("➕ Ajouter Salle", this);
    m_btnAjouter->setStyleSheet("background-color: #10B981; color: white;");

    m_btnModifier = new QPushButton("✏️ Modifier", this);
    m_btnModifier->setStyleSheet("background-color: #F59E0B; color: white;");

    m_btnSupprimer = new QPushButton("🗑️ Supprimer", this);
    m_btnSupprimer->setStyleSheet("background-color: #EF4444; color: white;");

    m_btnDispo = new QPushButton("🔄 Basculer Dispo", this);
    m_btnDispo->setStyleSheet("background-color: #06B6D4; color: white;");

    QPushButton *btnSearchToggle = new QPushButton("🔍 Filtres", this);
    btnSearchToggle->setStyleSheet("background-color: #3B82F6; color: white;");

    m_btnPDF = new QPushButton("📄 Exporter PDF", this);
    m_btnPDF->setStyleSheet("background-color: #475569; color: white;");

    m_btnStats = new QPushButton("📊 Statistiques", this);
    m_btnStats->setStyleSheet("background-color: #8B5CF6; color: white;");

    m_btnIA = new QPushButton("🤖 Recommandation IA", this);
    m_btnIA->setStyleSheet("background-color: #7C3AED; color: white;");

    m_btnQRCode = new QPushButton("📱 QR Code", this);
    m_btnQRCode->setStyleSheet("background-color: #0284C7; color: white;");

    toolbarLayout->addWidget(m_btnAjouter);
    toolbarLayout->addWidget(m_btnModifier);
    toolbarLayout->addWidget(m_btnSupprimer);
    toolbarLayout->addWidget(m_btnDispo);
    toolbarLayout->addWidget(btnSearchToggle);
    toolbarLayout->addWidget(m_btnPDF);
    toolbarLayout->addWidget(m_btnStats);
    toolbarLayout->addWidget(m_btnIA);
    toolbarLayout->addWidget(m_btnQRCode);
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
    m_searchNom->setPlaceholderText("Nom salle...");

    m_searchType = new QComboBox(this);
    m_searchType->addItems({"Tous", "Cours", "TP", "Amphitheatre", "Conference"});

    m_searchCapMin = new QSpinBox(this);
    m_searchCapMin->setRange(0, 9999);
    m_searchCapMin->setPrefix("Min: ");

    m_searchCapMax = new QSpinBox(this);
    m_searchCapMax->setRange(0, 9999);
    m_searchCapMax->setPrefix("Max: ");

    m_searchDispo = new QComboBox(this);
    m_searchDispo->addItem("Tous", -1);
    m_searchDispo->addItem("Disponible", 1);
    m_searchDispo->addItem("Indisponible", 0);

    QPushButton *btnAppliquerSearch = new QPushButton("Appliquer", this);
    btnAppliquerSearch->setStyleSheet("background-color: #2563EB; color: white;");
    QPushButton *btnResetSearch = new QPushButton("Réinitialiser", this);
    btnResetSearch->setStyleSheet("background-color: #64748B; color: white;");

    searchLayout->addWidget(new QLabel("Nom:"));
    searchLayout->addWidget(m_searchNom);
    searchLayout->addWidget(new QLabel("Type:"));
    searchLayout->addWidget(m_searchType);
    searchLayout->addWidget(m_searchCapMin);
    searchLayout->addWidget(m_searchCapMax);
    searchLayout->addWidget(new QLabel("État:"));
    searchLayout->addWidget(m_searchDispo);
    searchLayout->addWidget(btnAppliquerSearch);
    searchLayout->addWidget(btnResetSearch);

    layoutListe->addWidget(m_searchPanel);

    // Sort Bar
    QHBoxLayout *sortLayout = new QHBoxLayout();
    QLabel *lblSort = new QLabel("Trier par:", this);
    lblSort->setStyleSheet("font-weight: 700; color: #475569;");
    sortLayout->addWidget(lblSort);

    m_sortByCombo = new QComboBox(this);
    m_sortByCombo->addItems({"ID", "Nom", "Capacite", "Type", "Disponibilite"});
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
    m_tableWidget->setColumnCount(6);
    m_tableWidget->setHorizontalHeaderLabels({"ID", "Nom Salle", "Capacité (places)", "Type de Salle", "Équipements", "Disponibilité"});
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

    m_formTitleLabel = new QLabel("➕ Ajouter une nouvelle Salle", formCard);
    m_formTitleLabel->setStyleSheet("font-size: 18px; font-weight: 800; color: #1E3A8A !important; margin-bottom: 10px; border: none !important; background: transparent !important;");
    formCardLayout->addWidget(m_formTitleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(16);

    m_nomEdit = new QLineEdit(formCard);
    m_nomEdit->setPlaceholderText("Ex: Salle A101");
    m_nomEdit->setMaxLength(80);

    m_capSpinBox = new QSpinBox(formCard);
    m_capSpinBox->setRange(1, 1000);
    m_capSpinBox->setValue(30);
    m_capSpinBox->setSuffix(" places");

    m_typeCombo = new QComboBox(formCard);
    m_typeCombo->addItems({"Cours", "TP", "Amphitheatre", "Conference"});

    m_equipEdit = new QLineEdit(formCard);
    m_equipEdit->setPlaceholderText("Ex: Projecteur HD, Tableau blanc...");
    m_equipEdit->setMaxLength(150);

    m_dispoCheck = new QCheckBox("Salle disponible pour affectation de cours", formCard);
    m_dispoCheck->setChecked(true);

    formLayout->addRow("Nom de la Salle *:", m_nomEdit);
    formLayout->addRow("Capacité (places) *:", m_capSpinBox);
    formLayout->addRow("Type de Salle *:", m_typeCombo);
    formLayout->addRow("Équipements:", m_equipEdit);
    formLayout->addRow("Statut Initial:", m_dispoCheck);

    formCardLayout->addLayout(formLayout);

    m_errorLabel = new QLabel(formCard);
    m_errorLabel->setStyleSheet("color: #EF4444 !important; font-weight: 700; font-size: 13px; border: none !important; background: transparent !important;");
    formCardLayout->addWidget(m_errorLabel);

    QHBoxLayout *formButtonsLayout = new QHBoxLayout();
    formButtonsLayout->setContentsMargins(0, 10, 0, 0);
    formButtonsLayout->setSpacing(12);

    QPushButton *btnSave = new QPushButton("💾 Enregistrer la Salle", formCard);
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

    m_statsWidget = new StatsSalleWidget(pageStats);
    layoutStats->addWidget(m_statsWidget);

    QPushButton *btnBackFromStats = new QPushButton("⬅️ Retour à la liste", pageStats);
    btnBackFromStats->setStyleSheet("background-color: #2563EB; color: white; font-weight: 700; padding: 10px 20px; border-radius: 6px;");
    layoutStats->addWidget(btnBackFromStats, 0, Qt::AlignLeft);

    m_stackedWidget->addWidget(pageStats);

    mainLayout->addWidget(m_stackedWidget);

    // Connections
    connect(m_btnAjouter, &QPushButton::clicked, this, &SalleWidget::onAjouter);
    connect(m_btnModifier, &QPushButton::clicked, this, &SalleWidget::onModifier);
    connect(m_btnSupprimer, &QPushButton::clicked, this, &SalleWidget::onSupprimer);
    connect(m_btnDispo, &QPushButton::clicked, this, &SalleWidget::onToggleDisponibilite);
    connect(btnSearchToggle, &QPushButton::clicked, this, &SalleWidget::onSearchToggle);
    connect(btnAppliquerSearch, &QPushButton::clicked, this, &SalleWidget::onApplySearch);
    connect(btnResetSearch, &QPushButton::clicked, this, &SalleWidget::onClearSearch);
    connect(btnAppliquerSort, &QPushButton::clicked, this, &SalleWidget::onApplySort);
    connect(m_btnPDF, &QPushButton::clicked, this, &SalleWidget::onGeneratePDF);
    connect(m_btnStats, &QPushButton::clicked, this, &SalleWidget::onShowStats);
    connect(m_btnIA, &QPushButton::clicked, this, &SalleWidget::onIAAssistant);
    connect(m_btnQRCode, &QPushButton::clicked, this, &SalleWidget::onQRCode);

    connect(btnSave, &QPushButton::clicked, this, &SalleWidget::onSave);
    connect(btnCancel, &QPushButton::clicked, this, &SalleWidget::onCancel);
    connect(btnBackFromStats, &QPushButton::clicked, this, &SalleWidget::onCancel);

    // Double-click row directly edits that salle
    connect(m_tableWidget, &QTableWidget::cellDoubleClicked, this, &SalleWidget::onModifier);
}

void SalleWidget::loadTableData(const QList<Salle>& list) {
    m_tableWidget->setRowCount(0);
    for (int i = 0; i < list.size(); ++i) {
        const Salle& s = list[i];
        m_tableWidget->insertRow(i);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(s.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *nomItem = new QTableWidgetItem(s.getNom());
        nomItem->setFont(QFont("Segoe UI", 10, QFont::Bold));

        QTableWidgetItem *capItem = new QTableWidgetItem(QString("%1 places").arg(s.getCapacite()));
        capItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *typeItem = new QTableWidgetItem(s.getType());
        typeItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *eqItem = new QTableWidgetItem(s.getEquipement().isEmpty() ? "-" : s.getEquipement());

        QTableWidgetItem *dispoItem = new QTableWidgetItem(s.isDisponible() ? "🟢 Disponible" : "🔴 Indisponible");
        dispoItem->setTextAlignment(Qt::AlignCenter);
        if (s.isDisponible()) {
            dispoItem->setForeground(QColor("#059669"));
            dispoItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        } else {
            dispoItem->setForeground(QColor("#DC2626"));
            dispoItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        }

        m_tableWidget->setItem(i, 0, idItem);
        m_tableWidget->setItem(i, 1, nomItem);
        m_tableWidget->setItem(i, 2, capItem);
        m_tableWidget->setItem(i, 3, typeItem);
        m_tableWidget->setItem(i, 4, eqItem);
        m_tableWidget->setItem(i, 5, dispoItem);
    }
}

void SalleWidget::onTableSelectionChanged() {
    // Keep buttons active
}

void SalleWidget::onAjouter() {
    m_editMode = false;
    m_editId = 0;
    m_formTitleLabel->setText("➕ Ajouter une nouvelle Salle");
    clearForm();
    m_stackedWidget->setCurrentIndex(1);
}

void SalleWidget::onModifier() {
    int row = m_tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord cliquer sur une salle dans le tableau pour la modifier.");
        return;
    }

    m_editMode = true;
    m_editId = m_tableWidget->item(row, 0)->text().toInt();

    Salle s;
    s.setId(m_editId);
    s.setNom(m_tableWidget->item(row, 1)->text());

    QString capText = m_tableWidget->item(row, 2)->text();
    capText.replace(" places", "");
    s.setCapacite(capText.toInt());

    s.setType(m_tableWidget->item(row, 3)->text());
    s.setEquipement(m_tableWidget->item(row, 4)->text() == "-" ? "" : m_tableWidget->item(row, 4)->text());
    s.setDisponible(m_tableWidget->item(row, 5)->text().contains("Disponible"));

    m_formTitleLabel->setText(QString("✏️ Modifier la Salle (ID: %1)").arg(m_editId));
    populateForm(s);
    m_stackedWidget->setCurrentIndex(1);
}

void SalleWidget::onSupprimer() {
    int row = m_tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord cliquer sur une salle dans le tableau pour la supprimer.");
        return;
    }

    int id = m_tableWidget->item(row, 0)->text().toInt();
    QString nom = m_tableWidget->item(row, 1)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation de suppression",
        QString("Voulez-vous vraiment supprimer la salle '%1' (ID: %2) ?").arg(nom).arg(id),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        Salle s;
        s.setId(id);
        if (s.supprimer()) {
            QMessageBox::information(this, "Succès", "Salle supprimée avec succès.");
            loadTableData(Salle::afficher());
        } else {
            QMessageBox::critical(this, "Erreur d'intégrité", "Impossible de supprimer cette salle : des cours y sont actuellement affectés.");
        }
    }
}

void SalleWidget::onToggleDisponibilite() {
    int row = m_tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord cliquer sur une salle dans le tableau.");
        return;
    }

    int id = m_tableWidget->item(row, 0)->text().toInt();
    if (Salle::toggleDisponibilite(id)) {
        loadTableData(Salle::afficher());
    }
}

void SalleWidget::onSave() {
    if (!validateForm()) return;

    Salle s(
        m_editId,
        m_nomEdit->text().trimmed(),
        m_capSpinBox->value(),
        m_typeCombo->currentText(),
        m_equipEdit->text().trimmed(),
        m_dispoCheck->isChecked()
    );

    bool ok = m_editMode ? s.modifier() : s.ajouter();

    if (ok) {
        QMessageBox::information(this, "Succès", m_editMode ? "Salle modifiée avec succès dans la base Oracle!" : "Salle ajoutée avec succès dans la base Oracle!");
        m_stackedWidget->setCurrentIndex(0);
        loadTableData(Salle::afficher());
    } else {
        m_errorLabel->setText(QString("Erreur Oracle : %1").arg(Salle::lastError.isEmpty() ? "Vérifiez vos données." : Salle::lastError));
    }
}

void SalleWidget::onCancel() {
    m_stackedWidget->setCurrentIndex(0);
}

void SalleWidget::onSearchToggle() {
    m_searchPanel->setVisible(!m_searchPanel->isVisible());
}

void SalleWidget::onApplySearch() {
    QString nom = m_searchNom->text();
    QString type = m_searchType->currentText();
    int capMin = m_searchCapMin->value();
    int capMax = m_searchCapMax->value();
    int dispo = m_searchDispo->currentData().toInt();

    loadTableData(Salle::rechercher(nom, type, capMin, capMax, dispo));
}

void SalleWidget::onClearSearch() {
    m_searchNom->clear();
    m_searchType->setCurrentIndex(0);
    m_searchCapMin->setValue(0);
    m_searchCapMax->setValue(0);
    m_searchDispo->setCurrentIndex(0);

    loadTableData(Salle::afficher());
}

void SalleWidget::onApplySort() {
    QString critere = m_sortByCombo->currentText();
    QString ordre = m_sortOrderCombo->currentText();
    loadTableData(Salle::trier(critere, ordre));
}

void SalleWidget::onShowStats() {
    m_statsWidget->refreshStats();
    m_stackedWidget->setCurrentIndex(2);
}

void SalleWidget::onIAAssistant() {
    QString report = Salle::analyseIAOptimisation();
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("🤖 Assistant IA - Optimisation des Salles");
    msgBox.setText(report);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void SalleWidget::onQRCode() {
    int row = m_tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord cliquer sur une salle dans le tableau pour générer son QR Code.");
        return;
    }

    QString idStr = m_tableWidget->item(row, 0)->text();
    QString nomStr = m_tableWidget->item(row, 1)->text();
    QString capStr = m_tableWidget->item(row, 2)->text();
    QString typeStr = m_tableWidget->item(row, 3)->text();
    QString eqStr = m_tableWidget->item(row, 4)->text();
    QString dispoStr = m_tableWidget->item(row, 5)->text();

    QString qrPayload = QString(
        "CENTRE DE FORMATION ESPRIT\n"
        "--- SALLE DE FORMATION ---\n"
        "ID: #%1\n"
        "Nom: %2\n"
        "Capacité: %3\n"
        "Type: %4\n"
        "Équipements: %5\n"
        "Statut: %6\n"
        "Oracle XE verified"
    ).arg(idStr).arg(nomStr).arg(capStr).arg(typeStr).arg(eqStr).arg(dispoStr);

    QPixmap pixmap = QRCodeGenerator::generateQRCodePixmap(qrPayload, 260);

    QDialog dlg(this);
    dlg.setWindowTitle(QString("📱 Flashcode / QR Code — %1").arg(nomStr));
    dlg.resize(400, 480);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel(QString("📱 QR Code Officiel - %1").arg(nomStr), &dlg);
    lblTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #1E3A8A;");
    lblTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(lblTitle);

    QLabel *lblImg = new QLabel(&dlg);
    lblImg->setPixmap(pixmap);
    lblImg->setAlignment(Qt::AlignCenter);
    layout->addWidget(lblImg);

    QLabel *lblInfo = new QLabel(QString("Scannez ce QR Code pour accéder à la fiche d'information de la salle #%1 (%2).").arg(idStr).arg(nomStr), &dlg);
    lblInfo->setWordWrap(true);
    lblInfo->setAlignment(Qt::AlignCenter);
    lblInfo->setStyleSheet("color: #64748B; font-size: 12px;");
    layout->addWidget(lblInfo);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("💾 Enregistrer Image (.png)", &dlg);
    btnSave->setStyleSheet("background-color: #2563EB; color: white; font-weight: bold; padding: 8px 16px; border-radius: 6px;");

    QPushButton *btnClose = new QPushButton("Fermer", &dlg);
    btnClose->setStyleSheet("background-color: #64748B; color: white; font-weight: bold; padding: 8px 16px; border-radius: 6px;");

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnClose);
    layout->addLayout(btnLayout);

    connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(btnSave, &QPushButton::clicked, [this, pixmap, nomStr]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer QR Code", QString("QRCode_Salle_%1.png").arg(nomStr), "Images PNG (*.png)");
        if (!fileName.isEmpty()) {
            pixmap.save(fileName, "PNG");
            QMessageBox::information(this, "Succès", QString("QR Code enregistré avec succès dans :\n%1").arg(fileName));
        }
    });

    dlg.exec();
}

void SalleWidget::onGeneratePDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF Salles", "Rapport_Salles.pdf", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);

    QList<Salle> list = Salle::afficher();
    int dispoCount = Salle::countDisponibles();
    int indispoCount = Salle::countIndisponibles();

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
            <h1>CENTRE DE FORMATION — RAPPORT DES SALLES</h1>
            <p class="sub">Document officiel d'inventaire généré le %1</p>

            <table class="summary">
                <tr>
                    <td>Total Salles : <font color="#2563EB">%2</font></td>
                    <td>Disponibles : <font color="#059669">%3</font></td>
                    <td>Occupées / Indisponibles : <font color="#DC2626">%4</font></td>
                </tr>
            </table>

            <table class="grid">
                <thead>
                    <tr>
                        <th width="10%%">ID</th>
                        <th width="32%%">Nom de la Salle</th>
                        <th width="18%%">Capacité</th>
                        <th width="20%%">Type</th>
                        <th width="20%%">Statut</th>
                    </tr>
                </thead>
                <tbody>
    )").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"))
       .arg(list.size())
       .arg(dispoCount)
       .arg(indispoCount);

    for (int i = 0; i < list.size(); ++i) {
        const Salle& s = list[i];
        QString bg = (i % 2 == 0) ? "#FFFFFF" : "#F8FAFC";
        QString statusBadge = s.isDisponible()
            ? "<font color='#059669'><b>[ DISPONIBLE ]</b></font>"
            : "<font color='#DC2626'><b>[ INDISPONIBLE ]</b></font>";

        html += QString(R"(
            <tr bgcolor="%1">
                <td align="center"><b>#%2</b></td>
                <td><b>%3</b></td>
                <td align="center">%4 places</td>
                <td align="center">%5</td>
                <td align="center">%6</td>
            </tr>
        )").arg(bg)
           .arg(s.getId())
           .arg(s.getNom())
           .arg(s.getCapacite())
           .arg(s.getType())
           .arg(statusBadge);
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

void SalleWidget::clearForm() {
    m_nomEdit->clear();
    m_capSpinBox->setValue(30);
    m_typeCombo->setCurrentIndex(0);
    m_equipEdit->clear();
    m_dispoCheck->setChecked(true);
    m_errorLabel->clear();
}

void SalleWidget::populateForm(const Salle& s) {
    m_nomEdit->setText(s.getNom());
    m_capSpinBox->setValue(s.getCapacite());
    m_typeCombo->setCurrentText(s.getType());
    m_equipEdit->setText(s.getEquipement());
    m_dispoCheck->setChecked(s.isDisponible());
    m_errorLabel->clear();
}

bool SalleWidget::validateForm() {
    if (m_nomEdit->text().trimmed().isEmpty()) {
        m_errorLabel->setText("❌ Le nom de la salle est obligatoire.");
        return false;
    }
    if (m_capSpinBox->value() <= 0) {
        m_errorLabel->setText("❌ La capacité doit être supérieure à 0.");
        return false;
    }
    m_errorLabel->clear();
    return true;
}
