#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &username, const QString &role);

private slots:
    void onLoginClicked();
    void onTogglePasswordVisibility();

private:
    void setupUI();
    void clearFields();

    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_togglePasswordBtn;
    QComboBox *m_roleCombo;
    QCheckBox *m_rememberMeCheck;
    QLabel *m_errorLabel;
    QPushButton *m_loginBtn;
};

#endif // LOGINWIDGET_H
