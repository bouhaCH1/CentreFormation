#include "StatsCoursWidget.h"
#include "../models/Cours.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDate>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

StatsCoursWidget::StatsCoursWidget(QWidget *parent)
    : QWidget(parent), m_chartViewCat(nullptr), m_chartViewNiveau(nullptr), m_chartViewMois(nullptr) {
    setupUI();
    refreshStats();
}

void StatsCoursWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    QLabel *titleLabel = new QLabel("📊 Statistiques - Module Cours", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #1E3A8A; margin-bottom: 5px;");
    mainLayout->addWidget(titleLabel);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(12);

    m_chartViewCat = new QChartView(this);
    m_chartViewCat->setRenderHint(QPainter::Antialiasing);

    m_chartViewNiveau = new QChartView(this);
    m_chartViewNiveau->setRenderHint(QPainter::Antialiasing);

    m_chartViewMois = new QChartView(this);
    m_chartViewMois->setRenderHint(QPainter::Antialiasing);

    // Row 0: Category bar chart & Level pie chart side-by-side
    gridLayout->addWidget(m_chartViewCat, 0, 0);
    gridLayout->addWidget(m_chartViewNiveau, 0, 1);

    // Row 1: Monthly timeline spanning full width (gives 12 months ample horizontal space)
    gridLayout->addWidget(m_chartViewMois, 1, 0, 1, 2);

    mainLayout->addLayout(gridLayout);
}

QChart* StatsCoursWidget::createCategorieChart() {
    QChart *chart = new QChart();
    chart->setTitle("Cours par Catégorie");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *set = new QBarSet("Nombre de cours");
    set->setColor(QColor("#0284C7"));

    QStringList categories;
    QMap<QString, int> stats = Cours::statsParCategorie();
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        categories << it.key();
        *set << it.value();
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsFont(QFont("Segoe UI", 9, QFont::Bold));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

QChart* StatsCoursWidget::createNiveauChart() {
    QChart *chart = new QChart();
    chart->setTitle("Répartition par Niveau");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();
    QMap<QString, int> stats = Cours::statsParNiveau();
    QMap<QString, QColor> colors = {
        {"Debutant", QColor("#34D399")},
        {"Intermediaire", QColor("#FBBF24")},
        {"Avance", QColor("#F87171")}
    };

    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QPieSlice *slice = series->append(QString("%1 (%2)").arg(it.key()).arg(it.value()), it.value());
        if (colors.contains(it.key())) slice->setColor(colors[it.key()]);
        slice->setLabelVisible();
    }

    chart->addSeries(series);
    return chart;
}

QChart* StatsCoursWidget::createMoisChart() {
    QChart *chart = new QChart();
    chart->setTitle(QString("Planification mensuelle %1").arg(QDate::currentDate().year()));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *set = new QBarSet("Cours prévus");
    set->setColor(QColor("#6366F1"));

    QStringList mois = {"Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"};
    QMap<int, int> stats = Cours::statsParMois(QDate::currentDate().year());

    for (int m = 1; m <= 12; ++m) {
        *set << stats.value(m, 0);
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(mois);
    axisX->setLabelsFont(QFont("Segoe UI", 9, QFont::Bold));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

void StatsCoursWidget::refreshStats() {
    m_chartViewCat->setChart(createCategorieChart());
    m_chartViewNiveau->setChart(createNiveauChart());
    m_chartViewMois->setChart(createMoisChart());
}
