#include "../include/avgdialog.h"
#include "../ui_avgdialog.h"

#include <cmath>
#include <algorithm>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QBarCategoryAxis>
#include <QValueAxis>

AvgDialog::AvgDialog(QWidget *parent, std::vector<double> &&data, const QString &&name, bool discrete) :
        QDialog(parent), name(name),
        ui(new Ui::AvgDialog), data(data) {
    ui->setupUi(this);
    data_initialize();
    chart_initialize();
}

AvgDialog::~AvgDialog() {
    delete ui;
}

void AvgDialog::data_initialize() {
    double groups = 1 + log2(data.size());
    int groups_n = static_cast<int>(floor(groups));
    std::sort(data.begin(), data.end());
    double step = (data[data.size() - 1] - data[0]) / groups;
    double last = data[0];
    for (int i = 0; i < data.size(); i += groups_n) {
        double avg = 0;
        int n = 0;
        double r_l = last;
        double r_r = last + step;
        for (const auto &t: data) {
            if (t > r_l && t < r_r) {
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

void AvgDialog::chart_initialize() const {
    auto *series = new QBarSeries;
    auto chart = new QChart;
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
    auto axisX = new QBarCategoryAxis;
    axisX->append(categories);
    axisX->setRange(start, end);
    axisX->setLabelsAngle(45);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto axisY = new QValueAxis;
    axisY->setRange(0, upper);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->addSeries(series);
    chart->setTitle(name);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    ui->chart->setChart(chart);
}
