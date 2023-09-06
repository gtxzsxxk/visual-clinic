#include "../include/scatterdialog.h"
#include "../ui_scatterdialog.h"

#include <QMessageBox>
#include <QScatterSeries>
#include <QValueAxis>
#include <QSplineSeries>
#include <algorithm>

ScatterDialog::ScatterDialog(QWidget *parent,QTableWidget* tableWidget) :
        QDialog(parent), tableWidget(tableWidget),
        ui(new Ui::ScatterDialog) {
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    int col=-1;
    int col_2;
    for(const auto &it:tableWidget->selectedItems()){
        if(col==-1){
            col = it->column();
            if(it->text()[0]>='A'){
                QMessageBox::warning(parent,"错误","无法拟合离散的一列");
                close();
            }
        }
        if(it->column()==col){
            data_col_ind.push_back(it->text().toFloat());
        }else{
            data_col_dep.push_back(it->text().toFloat());
            col_2 = it->column();
        }
    }
    ind_max = *std::max_element(data_col_ind.begin(),data_col_ind.end());
    ind_min = *std::min_element(data_col_ind.begin(),data_col_ind.end());
    dep_max = *std::max_element(data_col_dep.begin(),data_col_dep.end());
    dep_min = *std::min_element(data_col_dep.begin(),data_col_dep.end());
    name_col_ind = tableWidget->horizontalHeaderItem(col)->text();
    name_col_dep = tableWidget->horizontalHeaderItem(col_2)->text();
    ui->var_ind_label->setText(name_col_ind);
    ui->var_dep_label->setText(name_col_dep);
    connect(ui->swap_var_btn, SIGNAL(clicked()),this, SLOT(swap_variables()));
    draw_chart();
}

ScatterDialog::~ScatterDialog() {
    delete ui;
}

void ScatterDialog::swap_variables() {
    std::swap(data_col_ind, data_col_dep);
    std::swap(name_col_ind, name_col_dep);
    ui->var_ind_label->setText(name_col_ind);
    ui->var_dep_label->setText(name_col_dep);
    std::swap(ind_max,dep_max);
    std::swap(ind_min,dep_min);
    draw_chart();
}

void ScatterDialog::draw_chart() {
    auto *scatter_series = new QScatterSeries(this);
    scatter_series->setName(name_col_dep);
    for(int i=0;i<data_col_ind.size();i++){
        scatter_series->append(QPointF(data_col_ind[i],data_col_dep[i]));
    }
    auto *chart = new QChart();

    auto axisX = new QValueAxis((QObject *) this);
    double h_offset = (ind_max-ind_min)*0.05;
    axisX->setRange(ind_min-h_offset, ind_max+h_offset);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%d");
    chart->addAxis(axisX, Qt::AlignBottom);

    double v_offset = (dep_max-dep_min)*0.05;
    auto axisY = new QValueAxis((QObject *) this);
    axisY->setRange(dep_min-v_offset, dep_max+v_offset);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);


    chart->addSeries(scatter_series);
    scatter_series->attachAxis(axisX);
    scatter_series->attachAxis(axisY);
    ui->chart->setChart(chart);
}
