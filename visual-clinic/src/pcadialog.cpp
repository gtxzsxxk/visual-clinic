#include "../include/pcadialog.h"
#include "../lib/pca.hpp"
#include "../ui_pcadialog.h"

#include <set>
#include <QChartView>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/QScatterDataArray>

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

    q3DScatter = new Q3DScatter();
    q3DScatter->setFlags(q3DScatter->flags() ^ Qt::FramelessWindowHint);
    QScatter3DSeries *series = new QScatter3DSeries;
    QScatterDataArray data;
    data << QVector3D(0.5f, 0.5f, 0.5f) << QVector3D(-0.3f, -0.5f, -0.4f) << QVector3D(0.0f, -0.3f, 0.2f);
    series->dataProxy()->addItems(data);
    q3DScatter->addSeries(series);

    chartWidget = QWidget::createWindowContainer(q3DScatter);

    ui->horizontalLayout->addWidget(chartWidget);

    ui->horizontalLayout->setStretch(0,1);
    ui->horizontalLayout->setStretch(1,3);

    go_PCA();
}

PCADialog::~PCADialog()
{
    delete ui;
    if(q3DScatter != nullptr){
        delete q3DScatter;
    }
}

void PCADialog::go_PCA() {

}
