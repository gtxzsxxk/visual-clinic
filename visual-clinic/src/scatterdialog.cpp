#include "../include/scatterdialog.h"
#include "../lib/leastsquare.hpp"
#include "../ui_scatterdialog.h"

#include <algorithm>

ScatterDialog::ScatterDialog(QWidget *parent, QTableWidget *tableWidget) :
        QDialog(parent), tableWidget(tableWidget),
        ui(new Ui::ScatterDialog) {
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    int col = -1;
    int col_2;
    for (const auto &it: tableWidget->selectedItems()) {
        if (col == -1) {
            col = it->column();
            if (it->text()[0] >= 'A') {
                QMessageBox::warning(parent, "错误", "无法拟合离散的一列");
                close();
            }
        }
        if (it->column() == col) {
            data_col_ind.push_back(it->text().toFloat());
        } else {
            data_col_dep.push_back(it->text().toFloat());
            col_2 = it->column();
        }
    }
    ind_max = *std::max_element(data_col_ind.begin(), data_col_ind.end());
    ind_min = *std::min_element(data_col_ind.begin(), data_col_ind.end());
    dep_max = *std::max_element(data_col_dep.begin(), data_col_dep.end());
    dep_min = *std::min_element(data_col_dep.begin(), data_col_dep.end());
    name_col_ind = tableWidget->horizontalHeaderItem(col)->text();
    name_col_dep = tableWidget->horizontalHeaderItem(col_2)->text();
    ui->var_ind_label->setText(name_col_ind);
    ui->var_dep_label->setText(name_col_dep);
    connect(ui->swap_var_btn, SIGNAL(clicked()), this, SLOT(swap_variables()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onDegreeChange(int)));
    draw_chart();
    fit();
}

ScatterDialog::~ScatterDialog() {
    delete ui;
}

void ScatterDialog::swap_variables() {
    std::swap(data_col_ind, data_col_dep);
    std::swap(name_col_ind, name_col_dep);
    ui->var_ind_label->setText(name_col_ind);
    ui->var_dep_label->setText(name_col_dep);
    std::swap(ind_max, dep_max);
    std::swap(ind_min, dep_min);
    draw_chart();
    fit();
}

void ScatterDialog::draw_chart() {
    if (chart == nullptr) {
        chart = new QChart();
    }
    if (scatter_series == nullptr) {
        scatter_series = new QScatterSeries(this);
    } else {
        chart->removeSeries(scatter_series);
    }
    scatter_series->setName("散点图");
    scatter_series->clear();
    for (int i = 0; i < data_col_ind.size(); i++) {
        scatter_series->append(QPointF(data_col_ind[i], data_col_dep[i]));
    }

    if (axisX == nullptr) {
        axisX = new QValueAxis((QObject *) this);
    } else {
        chart->removeAxis(axisX);
    }
    if (axisY == nullptr) {
        axisY = new QValueAxis((QObject *) this);
    } else {
        chart->removeAxis(axisY);
    }
    double h_offset = (ind_max - ind_min) * 0.05;
    axisX->setRange(ind_min - h_offset, ind_max + h_offset);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%d");
    axisX->setTitleText(name_col_ind);
    chart->addAxis(axisX, Qt::AlignBottom);

    double v_offset = (dep_max - dep_min) * 0.05;
    axisY->setRange(dep_min - v_offset, dep_max + v_offset);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%d");
    axisY->setTitleText(name_col_dep);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries(scatter_series);
    scatter_series->attachAxis(axisX);
    scatter_series->attachAxis(axisY);
    ui->chart->setChart(chart);
}

void ScatterDialog::fit() {
    int deg = ui->spinBox->value();
    auto results = fitLeastSquareAndPR(data_col_ind, data_col_dep, deg);
    if (spline_series == nullptr) {
        spline_series = new QSplineSeries(this);
    } else {
        chart->removeSeries(spline_series);
    }
    spline_series->setName("拟合曲线");
    spline_series->clear();
    auto poly = std::get<0>(results);
    auto func = [&](float x) {
        float value = 0;
        for (int i = 0; i < deg + 1; i++) {
            float tmp = poly(i, 0);
            for (int j = 0; j < i; j++) {
                tmp *= x;
            }
            value += tmp;
        }
        return value;
    };
    int dots = 10;
    float h_offset = (float) ((ind_max - ind_min) * 0.05);
    float h_dist = h_offset * 2 + (ind_max - ind_min);
    float current = ind_min - h_offset;
    float step = h_dist / (float) dots;
    for (int i = 0; i < dots; i++) {
        auto value = func(current);
        spline_series->append(QPointF(current, value));
        current += step;
    }
    chart->addSeries(spline_series);
    spline_series->attachAxis(axisX);
    spline_series->attachAxis(axisY);
    auto p = std::get<1>(results);
    auto r_2 = std::get<2>(results);
    ui->p_label->setText(QString::number(p, 'g', 3));
    ui->r_square_label->setText(QString::number(r_2, 'g', 3));
}

void ScatterDialog::onDegreeChange(int deg) {
    fit();
}
