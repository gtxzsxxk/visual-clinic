#include "../include/relatedialog.h"
#include "../ui_relatedialog.h"

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

}

RelateDialog::~RelateDialog() {
    delete ui;
}

void RelateDialog::draw_hotgraph() {
    if(!graph_initialized){

    }
}
