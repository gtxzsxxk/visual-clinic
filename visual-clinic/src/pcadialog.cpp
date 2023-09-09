#include "../include/pcadialog.h"
#include "../lib/pca.hpp"
#include "../ui_pcadialog.h"

#include <set>

PCADialog::PCADialog(QWidget *parent,QTableWidget *tableWidget) :
    QDialog(parent),
    ui(new Ui::PCADialog),tableWidget(tableWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    std::set<int> columns;
    for (const auto &item: tableWidget->selectedItems()) {
        columns.insert(item->column());
    }
    ui->columns_label->clear();
    for (const auto &it: columns) {
        auto col_name = tableWidget->horizontalHeaderItem(it)->text();
        auto prev = ui->columns_label->text();
        prev += col_name + "\r\n";
        ui->columns_label->setText(prev);
        std::vector<float> data;
        for(int i=0;i<tableWidget->rowCount();i++){
            data.emplace_back(tableWidget->item(i,it)->text().toFloat());
        }
        points.emplace_back(data);
    }
    go_PCA();
}

PCADialog::~PCADialog()
{
    delete ui;
}

void PCADialog::go_PCA() {

}
