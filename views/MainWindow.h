#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>

class SalleWidget;
class CoursWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void applyGlobalStylesheet();

    QTabWidget *m_tabWidget;
    SalleWidget *m_salleWidget;
    CoursWidget *m_coursWidget;
    QLabel *m_statusLabel;
};

#endif // MAINWINDOW_H
