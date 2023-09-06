#include "../include/avgdialog.h"
#include "../lib/rowfeature.hpp"
#include "../ui_avgdialog.h"

#include <cmath>
#include <algorithm>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QBarCategoryAxis>
#include <QValueAxis>

AvgDialog::AvgDialog(QWidget *parent, std::vector<float> data, const QString &&name, bool discrete,
                     int discrete_categories) :
        QDialog(parent), name(name),
        ui(new Ui::AvgDialog), data(data), normal_distribution_enabled(false) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    if (discrete) {
        data_initialize(discrete_categories);
        ui->spinBox->setEnabled(false);
    } else {
        data_initialize();
        connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    }
    chart_initialize();
    auto tuple_avg_var = getAvgVar(data);
    average = std::get<0>(tuple_avg_var);
    variance = std::get<1>(tuple_avg_var);
    ui->average_label->setText(QString::number(average, 'g', 3));
    ui->variance_label->setText(QString::number(variance, 'g', 3));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNormalDistributionSet()));
}

AvgDialog::~AvgDialog() {
    delete ui;
}

void AvgDialog::data_initialize(double groups) {
    int groups_n = 0;
    if (groups == 0) {
        groups = 1 + log2(data.size());
        groups_n = static_cast<int>(floor(groups));
        ui->spinBox->setValue(groups_n);
    } else {
        groups_n = static_cast<int>(floor(groups));
    }
    std::sort(data.begin(), data.end());
    double step = (data[data.size() - 1] - data[0]) / groups;
    double last = data[0];
    int step_n = data.size() / groups;
    for (int i = 0; i < data.size(); i += step_n) {
        double avg = 0;
        int n = 0;
        double r_l = last;
        double r_r = last + step;
        for (const auto &t: data) {
            if (t >= r_l && t < r_r) {
                n++;
                avg += t;
            }
        }
        avg /= n;
        last = r_r;
        if (n > 0) {
            coordinates[avg] = n;
        }
    }
}

void AvgDialog::chart_initialize() {
    auto *series = new QBarSeries((QObject *) this);
    chart = new QChart;
    QStringList categories;
    int upper = 0;
    QString start, end;
    auto *st = new QBarSet("组内平均值", (QObject *) this);
    for (const auto &it: coordinates) {
        auto cat = QString::number(it.first, 'g', 2);
        if (start.isEmpty()) {
            start = cat;
        }
        end = cat;
        *st << it.second;
        categories.push_back(cat);
        if (it.second > upper) {
            upper = it.second;
        }
    }
    series->append(st);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible(true);
    auto axisX = new QBarCategoryAxis((QObject *) this);
    axisX->append(categories);
    axisX->setRange(start, end);
    axisX->setLabelsAngle(45);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto axisY = new QValueAxis((QObject *) this);
    axisY->setRange(0, upper + 10);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->addSeries(series);
    chart->setTitle(name);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    ui->chart->setChart(chart);
}

void AvgDialog::onSpinBoxValueChanged(int value) {
    coordinates.clear();
    data_initialize(value);
    chart_initialize();
    normal_distribution_enabled = !normal_distribution_enabled;
    onNormalDistributionSet();
}

void AvgDialog::onNormalDistributionSet() {
    const float PI = 3.141592653;
    if (lineseries == nullptr) {
        lineseries = new QSplineSeries;
        lineseries->setName("正态分布");
        for (const auto &it: coordinates) {
            auto value = static_cast<float>(1 / std::sqrt(2 * PI * variance) *
                                            std::exp(-(it.first - average) * (it.first - average) / 2 / variance));
            lineseries->append(QPointF(it.first, value));
        }
    }
    if (!normal_distribution_enabled) {
        chart->addSeries(lineseries);
        normal_distribution_enabled = true;
    } else {
        chart->removeSeries(lineseries);
        normal_distribution_enabled = false;
    }
}
