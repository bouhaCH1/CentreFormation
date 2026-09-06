#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>

class SalleWidget;
class CoursWidget;
class LoginWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onLoginSuccess(const QString &username, const QString &role);
    void onLogoutClicked();

private:
    void setupUI();
    void applyGlobalStylesheet();

    QStackedWidget *m_rootStackedWidget;
    LoginWidget *m_loginWidget;
    QWidget *m_appContainerWidget;
    QTabWidget *m_tabWidget;
    SalleWidget *m_salleWidget;
    CoursWidget *m_coursWidget;

    QLabel *m_statusLabel;
    QPushButton *m_logoutBtn;
    QString m_currentUser;
    QString m_currentRole;
};

#endif // MAINWINDOW_H
