#include "LoginWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
}

void LoginWidget::setupUI() {
    // Outer centered layout
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // Background Container Frame
    QFrame *bgFrame = new QFrame(this);
    bgFrame->setObjectName("loginBgFrame");
    bgFrame->setStyleSheet("QFrame#loginBgFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0F172A, stop:1 #1E3A8A); }");
    QVBoxLayout *bgLayout = new QVBoxLayout(bgFrame);

    // Centered Login Card
    QFrame *card = new QFrame(bgFrame);
    card->setObjectName("loginCard");
    card->setFixedSize(440, 520);
    card->setStyleSheet("QFrame#loginCard { background-color: #FFFFFF; border-radius: 12px; }");

    // Card shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 8);
    card->setGraphicsEffect(shadow);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(36, 36, 36, 36);
    cardLayout->setSpacing(16);

    // Header Title
    QLabel *iconLabel = new QLabel("🏢", card);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 42px; border: none; background: transparent;");
    cardLayout->addWidget(iconLabel);

    QLabel *titleLabel = new QLabel("CENTRE DE FORMATION", card);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: 800; color: #1E3A8A !important; border: none; background: transparent;");
    cardLayout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel("Gestion des Salles & Cours (Qt 6 & Oracle XE)", card);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 12px; font-weight: 600; color: #64748B !important; border: none; background: transparent;");
    cardLayout->addWidget(subtitleLabel);

    cardLayout->addSpacing(10);

    // Form
    m_usernameEdit = new QLineEdit(card);
    m_usernameEdit->setPlaceholderText("Nom d'utilisateur (ex: admin)");
    m_usernameEdit->setText("admin");

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    m_passwordEdit = new QLineEdit(card);
    m_passwordEdit->setPlaceholderText("Mot de passe");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setText("admin");

    m_togglePasswordBtn = new QPushButton("👁️", card);
    m_togglePasswordBtn->setCursor(Qt::PointingHandCursor);
    m_togglePasswordBtn->setFixedWidth(36);
    m_togglePasswordBtn->setToolTip("Afficher/Masquer le mot de passe");
    m_togglePasswordBtn->setStyleSheet("background-color: #E2E8F0; color: #1E293B; border-radius: 6px; padding: 4px;");

    passwordLayout->addWidget(m_passwordEdit);
    passwordLayout->addWidget(m_togglePasswordBtn);

    m_roleCombo = new QComboBox(card);
    m_roleCombo->addItems({"Administrateur", "Gestionnaire Salles", "Formateur"});

    m_rememberMeCheck = new QCheckBox("Se souvenir de moi", card);
    m_rememberMeCheck->setChecked(true);

    cardLayout->addWidget(new QLabel("👤 Identifiant :", card));
    cardLayout->addWidget(m_usernameEdit);
    cardLayout->addWidget(new QLabel("🔒 Mot de passe :", card));
    cardLayout->addLayout(passwordLayout);
    cardLayout->addWidget(new QLabel("🎭 Rôle d'accès :", card));
    cardLayout->addWidget(m_roleCombo);
    cardLayout->addWidget(m_rememberMeCheck);

    // Error Label
    m_errorLabel = new QLabel(card);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet("color: #EF4444 !important; font-weight: 700; font-size: 12px; border: none; background: transparent;");
    cardLayout->addWidget(m_errorLabel);

    // Login Button
    m_loginBtn = new QPushButton("🔑 SE CONNECTER", card);
    m_loginBtn->setCursor(Qt::PointingHandCursor);
    m_loginBtn->setStyleSheet("background-color: #2563EB; color: #FFFFFF; font-size: 14px; font-weight: 800; padding: 12px; border-radius: 8px;");
    cardLayout->addWidget(m_loginBtn);

    // Hint footer label for jury demo
    QLabel *hintLabel = new QLabel("💡 Compte Démo : admin / admin", card);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet("font-size: 11px; color: #94A3B8 !important; border: none; background: transparent;");
    cardLayout->addWidget(hintLabel);

    bgLayout->addWidget(card, 0, Qt::AlignCenter);
    outerLayout->addWidget(bgFrame);

    // Connections
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &LoginWidget::onLoginClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::onLoginClicked);
    connect(m_togglePasswordBtn, &QPushButton::clicked, this, &LoginWidget::onTogglePasswordVisibility);
}

void LoginWidget::onLoginClicked() {
    QString user = m_usernameEdit->text().trimmed();
    QString pass = m_passwordEdit->text().trimmed();
    QString role = m_roleCombo->currentText();

    if (user.isEmpty() || pass.isEmpty()) {
        m_errorLabel->setText("❌ Veuillez saisir l'identifiant et le mot de passe.");
        return;
    }

    // Demo / System authentication check (admin/admin or SYSTEM/180702 or any non-empty demo login)
    if ((user.toLower() == "admin" && pass == "admin") || 
        (user.toUpper() == "SYSTEM" && pass == "180702") ||
        (!user.isEmpty() && pass == "admin") ||
        (!user.isEmpty() && pass.length() >= 4)) {
        
        m_errorLabel->clear();
        emit loginSuccess(user, role);
    } else {
        m_errorLabel->setText("❌ Identifiant ou mot de passe incorrect.");
    }
}

void LoginWidget::onTogglePasswordVisibility() {
    if (m_passwordEdit->echoMode() == QLineEdit::Password) {
        m_passwordEdit->setEchoMode(QLineEdit::Normal);
        m_togglePasswordBtn->setText("🙈");
    } else {
        m_passwordEdit->setEchoMode(QLineEdit::Password);
        m_togglePasswordBtn->setText("👁️");
    }
}

void LoginWidget::clearFields() {
    m_usernameEdit->setText("admin");
    m_passwordEdit->setText("admin");
    m_errorLabel->clear();
}
