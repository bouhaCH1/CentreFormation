#include "MainWindow.h"
#include "LoginWidget.h"
#include "SalleWidget.h"
#include "CoursWidget.h"
#include <QStatusBar>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    applyGlobalStylesheet();
}

void MainWindow::setupUI() {
    setWindowTitle("Centre de Formation - Management System (Qt 6 & Oracle XE)");
    resize(1300, 850);
    setMinimumSize(1100, 720);

    m_rootStackedWidget = new QStackedWidget(this);

    // ==========================================
    // PAGE 0 : LOGIN SCREEN
    // ==========================================
    m_loginWidget = new LoginWidget(this);
    m_rootStackedWidget->addWidget(m_loginWidget);

    // ==========================================
    // PAGE 1 : MAIN DASHBOARD APP
    // ==========================================
    m_appContainerWidget = new QWidget(this);
    QVBoxLayout *appLayout = new QVBoxLayout(m_appContainerWidget);
    appLayout->setContentsMargins(0, 0, 0, 0);

    m_tabWidget = new QTabWidget(m_appContainerWidget);
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->setMovable(false);

    m_salleWidget = new SalleWidget(m_tabWidget);
    m_coursWidget = new CoursWidget(m_tabWidget);

    m_tabWidget->addTab(m_salleWidget, "🏫  Gestion des Salles");
    m_tabWidget->addTab(m_coursWidget, "📚  Gestion des Cours");

    appLayout->addWidget(m_tabWidget);
    m_rootStackedWidget->addWidget(m_appContainerWidget);

    setCentralWidget(m_rootStackedWidget);
    m_rootStackedWidget->setCurrentIndex(0); // Start on Login Screen

    // Status Bar Setup
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("color: #059669; font-weight: 700; font-size: 13px; padding-left: 10px;");
    statusBar()->addWidget(m_statusLabel);

    m_logoutBtn = new QPushButton("🚪 Déconnexion", this);
    m_logoutBtn->setCursor(Qt::PointingHandCursor);
    m_logoutBtn->setStyleSheet("background-color: #EF4444; color: white; font-weight: 700; font-size: 12px; padding: 4px 12px; border-radius: 4px; margin-right: 10px;");
    m_logoutBtn->setVisible(false);
    statusBar()->addPermanentWidget(m_logoutBtn);

    // Connections
    connect(m_loginWidget, &LoginWidget::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(m_logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
}

void MainWindow::onLoginSuccess(const QString &username, const QString &role) {
    m_currentUser = username;
    m_currentRole = role;

    m_statusLabel->setText(QString("🟢 Connecté : %1 (%2) | Oracle XE: ACTIVE (localhost:1521/XE)").arg(username).arg(role));
    m_logoutBtn->setVisible(true);

    m_rootStackedWidget->setCurrentIndex(1);
}

void MainWindow::onLogoutClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Déconnexion", "Voulez-vous vraiment vous déconnecter ?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_rootStackedWidget->setCurrentIndex(0);
        m_logoutBtn->setVisible(false);
        m_statusLabel->setText("🔒 Non connecté");
    }
}

void MainWindow::applyGlobalStylesheet() {
    QString qss = R"(
        /* Global Window */
        QMainWindow {
            background-color: #F8FAFC;
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        /* Tab Widget Styling */
        QTabWidget::pane {
            border: 1px solid #E2E8F0;
            background: #FFFFFF;
            border-bottom-left-radius: 12px;
            border-bottom-right-radius: 12px;
            top: -1px;
        }
        QTabBar::tab {
            background: #F1F5F9;
            color: #475569;
            padding: 12px 24px;
            font-weight: 700;
            font-size: 14px;
            border: 1px solid #E2E8F0;
            border-bottom: none;
            border-top-left-radius: 10px;
            border-top-right-radius: 10px;
            margin-right: 4px;
        }
        QTabBar::tab:selected {
            background: #2563EB;
            color: #FFFFFF;
            border-color: #2563EB;
        }
        QTabBar::tab:hover:!selected {
            background: #E2E8F0;
            color: #1E293B;
        }

        /* General Labels */
        QLabel {
            color: #0F172A !important;
            font-size: 13px !important;
            font-weight: 600 !important;
            background: transparent !important;
            border: none !important;
            padding: 0px !important;
            margin: 0px !important;
        }

        /* Inputs */
        QLineEdit, QComboBox, QSpinBox, QDateEdit, QTextEdit {
            background-color: #FFFFFF !important;
            color: #0F172A !important;
            border: 1px solid #94A3B8 !important;
            border-radius: 6px !important;
            padding: 6px 10px !important;
            font-size: 13px !important;
            font-weight: 600 !important;
            min-height: 24px !important;
        }

        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDateEdit:focus, QTextEdit:focus {
            border: 2px solid #2563EB !important;
            background-color: #FFFFFF !important;
        }

        QLineEdit::placeholder, QTextEdit::placeholder {
            color: #64748B !important;
            font-style: italic;
            font-weight: 400;
        }

        /* Dropdowns & Popups */
        QComboBox {
            padding-right: 25px !important;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 25px;
            border-left-width: 0px;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF !important;
            color: #0F172A !important;
            border: 1px solid #94A3B8;
            selection-background-color: #2563EB !important;
            selection-color: #FFFFFF !important;
            outline: none;
            padding: 4px;
        }

        /* Buttons */
        QPushButton {
            border: none !important;
            border-radius: 6px !important;
            font-size: 13px !important;
            font-weight: 700 !important;
            padding: 8px 16px !important;
            min-height: 24px !important;
        }
        QPushButton:hover {
            opacity: 0.92;
        }
        QPushButton:pressed {
            opacity: 0.82;
        }
        QPushButton:disabled {
            background-color: #E2E8F0 !important;
            color: #94A3B8 !important;
        }

        /* Table View */
        QTableWidget {
            background-color: #FFFFFF !important;
            gridline-color: #E2E8F0;
            border: 1px solid #CBD5E1;
            border-radius: 8px;
            color: #0F172A !important;
            font-size: 13px;
            selection-background-color: #DBEAFE !important;
            selection-color: #1E40AF !important;
        }
        QHeaderView::section {
            background-color: #0F172A !important;
            color: #F8FAFC !important;
            padding: 10px 12px;
            font-weight: 700;
            font-size: 13px;
            border: none;
            border-right: 1px solid #1E293B;
        }
        QTableWidget::item {
            padding: 6px;
            border-bottom: 1px solid #F1F5F9;
        }

        /* Checkbox */
        QCheckBox {
            color: #1E293B !important;
            font-size: 13px !important;
            font-weight: 600 !important;
            spacing: 8px;
            background: transparent !important;
            border: none !important;
        }

        /* MessageBox & Dialog High Contrast Styling */
        QDialog, QMessageBox {
            background-color: #FFFFFF !important;
            color: #0F172A !important;
        }
        QMessageBox QLabel {
            color: #0F172A !important;
            font-size: 13px !important;
            font-weight: 600 !important;
        }
        QMessageBox QPushButton {
            background-color: #2563EB !important;
            color: #FFFFFF !important;
            font-weight: 700 !important;
            min-width: 80px !important;
            padding: 6px 16px !important;
        }

        /* Status Bar */
        QStatusBar {
            background: #FFFFFF;
            border-top: 1px solid #E2E8F0;
            min-height: 28px;
        }
    )";
    setStyleSheet(qss);
}
