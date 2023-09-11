#include "../include/pcadialog.h"
#include "../lib/pca.hpp"
#include "../ui_pcadialog.h"

#include <set>
#include <QChartView>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/QScatterDataArray>

PCADialog::PCADialog(QWidget *parent, QTableWidget *tableWidget) :
        QDialog(parent),
        ui(new Ui::PCADialog), tableWidget(tableWidget) {
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
    }
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        std::vector<float> data;
        for (const auto &it: columns) {
            data.emplace_back(tableWidget->item(i, it)->text().toFloat());
        }
        points.emplace_back(data);
    }

    go_PCA();
}

PCADialog::~PCADialog() {
    delete ui;
}

void PCADialog::go_PCA() {
    int dim = ui->spinBox->value();
    /* TODO: manage the memory use */
    if (dim == 3) {
        auto *q3DScatter = new Q3DScatter();
        q3DScatter->setFlags(q3DScatter->flags() ^ Qt::FramelessWindowHint);
        chartWidget = QWidget::createWindowContainer(q3DScatter);
        /* todo: check here */
        ui->horizontalLayout->addWidget(chartWidget);
        ui->horizontalLayout->setStretch(0, 1);
        ui->horizontalLayout->setStretch(1, 3);
        auto *series = new QScatter3DSeries;
        QScatterDataArray data;
        auto res = pca(points, 3);
        for (int i = 0; i < points.size(); i++) {
            float v_0 = res(i, 0);
            float v_1 = res(i, 1);
            float v_2 = res(i, 2);
            data << QVector3D(v_0, v_1, v_2);
        }
        series->dataProxy()->addItems(data);
        q3DScatter->addSeries(series);
    }
}
