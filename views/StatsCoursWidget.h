#ifndef STATSCOURSWIDGET_H
#define STATSCOURSWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

class StatsCoursWidget : public QWidget {
    Q_OBJECT

public:
    explicit StatsCoursWidget(QWidget *parent = nullptr);

public slots:
    void refreshStats();

private:
    void setupUI();
    QChart* createCategorieChart();
    QChart* createNiveauChart();
    QChart* createMoisChart();

    QChartView *m_chartViewCat;
    QChartView *m_chartViewNiveau;
    QChartView *m_chartViewMois;
};

#endif // STATSCOURSWIDGET_H
