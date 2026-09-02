#ifndef STATSSALLEWIDGET_H
#define STATSSALLEWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

class StatsSalleWidget : public QWidget {
    Q_OBJECT

public:
    explicit StatsSalleWidget(QWidget *parent = nullptr);

public slots:
    void refreshStats();

private:
    void setupUI();
    QChart* createTypeChart();
    QChart* createDispoChart();
    QChart* createCapaciteChart();

    QChartView *m_chartViewType;
    QChartView *m_chartViewDispo;
    QChartView *m_chartViewCapacite;
};

#endif // STATSSALLEWIDGET_H
