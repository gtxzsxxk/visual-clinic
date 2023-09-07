#include "../include/relatedialog.h"
#include "../lib/covariance.hpp"
#include "../ui_relatedialog.h"

#include <QTableWidgetItem>
#include <set>

RelateDialog::RelateDialog(QWidget *parent, QTableWidget *tableWidget) :
        QDialog(parent),
        ui(new Ui::RelateDialog), tableWidget(tableWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    std::set<int> columns;
    for (const auto &item: tableWidget->selectedItems()) {
        columns.insert(item->column());
    }
    ui->columns_label->clear();
    for (const auto &it: columns) {
        auto col_name = tableWidget->horizontalHeaderItem(it)->text();
        column_name_pairs.emplace_back(it, col_name);
        auto prev = ui->columns_label->text();
        prev += col_name + "\r\n";
        ui->columns_label->setText(prev);
    }
    draw_hotgraph();
    update_data();
}

RelateDialog::~RelateDialog() {
    delete ui;
}

void RelateDialog::draw_hotgraph() {
    if (!graph_initialized) {
        QStringList headers;
        ui->hotgraph->clear();
        while (ui->hotgraph->rowCount()) {
            ui->hotgraph->removeRow(0);
        }
        ui->hotgraph->setColumnCount(column_name_pairs.size());
        ui->hotgraph->setRowCount(column_name_pairs.size());
        for (const auto &it: column_name_pairs) {
            headers.emplace_back(it.second);
        }
        ui->hotgraph->setHorizontalHeaderLabels(headers);
        ui->hotgraph->setVerticalHeaderLabels(headers);
        int section_size = 500/(column_name_pairs.size()+1);
        ui->hotgraph->horizontalHeader()->setDefaultSectionSize(section_size);
        ui->hotgraph->verticalHeader()->setDefaultSectionSize(section_size);
        int row_cnt = 0;
        for (const auto &it: column_name_pairs) {
            for (int j = 0; j < column_name_pairs.size(); j++) {
                ui->hotgraph->setItem(row_cnt, j, new QTableWidgetItem("0"));
            }
            row_cnt++;
        }

        graph_initialized = true;
    }

}

void RelateDialog::update_data() {
    std::vector<std::vector<float>> mat_input;
    for(const auto &it:column_name_pairs){
        std::vector<float> vec;
        for(int r=0; r < tableWidget->rowCount(); r++){
            auto data = tableWidget->item(r,it.first)->text();
            vec.emplace_back(data.toFloat());
        }
        mat_input.emplace_back(std::move(vec));
    }
    Eigen::MatrixXf mat_covariance = getCovariance(mat_input);
    Eigen::MatrixXf result;
    if(!is_relate_co){
        //covariance
        result = std::move(mat_covariance);
    }else{
        //relate coefficient
        std::vector<float> vars;
        for(const auto& vec:mat_input){
            auto avg = getAvgVar(vec);
            vars.emplace_back(std::get<1>(avg));
        }
        result = getPearsonCorr(mat_covariance,vars);
    }

}
