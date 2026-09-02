#include "StatsSalleWidget.h"
#include "../models/Salle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

StatsSalleWidget::StatsSalleWidget(QWidget *parent)
    : QWidget(parent), m_chartViewType(nullptr), m_chartViewDispo(nullptr), m_chartViewCapacite(nullptr) {
    setupUI();
    refreshStats();
}

void StatsSalleWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("📊 Statistiques - Module Salles", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #1E3A8A; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *chartsLayout = new QHBoxLayout();

    m_chartViewType = new QChartView(this);
    m_chartViewType->setRenderHint(QPainter::Antialiasing);

    m_chartViewDispo = new QChartView(this);
    m_chartViewDispo->setRenderHint(QPainter::Antialiasing);

    m_chartViewCapacite = new QChartView(this);
    m_chartViewCapacite->setRenderHint(QPainter::Antialiasing);

    chartsLayout->addWidget(m_chartViewType);
    chartsLayout->addWidget(m_chartViewDispo);
    chartsLayout->addWidget(m_chartViewCapacite);

    mainLayout->addLayout(chartsLayout);
}

QChart* StatsSalleWidget::createTypeChart() {
    QChart *chart = new QChart();
    chart->setTitle("Nombre de salles par type");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *set = new QBarSet("Salles");
    set->setColor(QColor("#2563EB"));

    QStringList categories;
    QMap<QString, int> stats = Salle::statsParType();
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        categories << it.key();
        *set << it.value();
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

QChart* StatsSalleWidget::createDispoChart() {
    QChart *chart = new QChart();
    chart->setTitle("Disponibilité des salles");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();
    int dispo = Salle::countDisponibles();
    int indispo = Salle::countIndisponibles();

    QPieSlice *slice1 = series->append(QString("Disponibles (%1)").arg(dispo), dispo);
    slice1->setColor(QColor("#10B981"));
    slice1->setExploded();
    slice1->setLabelVisible();

    QPieSlice *slice2 = series->append(QString("Occupées / Indisponibles (%1)").arg(indispo), indispo);
    slice2->setColor(QColor("#EF4444"));
    slice2->setLabelVisible();

    chart->addSeries(series);
    return chart;
}

QChart* StatsSalleWidget::createCapaciteChart() {
    QChart *chart = new QChart();
    chart->setTitle("Capacité moyenne par type");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *set = new QBarSet("Capacité moyenne");
    set->setColor(QColor("#8B5CF6"));

    QStringList categories;
    QMap<QString, int> stats = Salle::statsCapaciteMoyenneParType();
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        categories << it.key();
        *set << it.value();
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d places");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

void StatsSalleWidget::refreshStats() {
    m_chartViewType->setChart(createTypeChart());
    m_chartViewDispo->setChart(createDispoChart());
    m_chartViewCapacite->setChart(createCapaciteChart());
}
