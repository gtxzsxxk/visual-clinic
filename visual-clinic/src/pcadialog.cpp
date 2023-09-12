#include "../include/pcadialog.h"
#include "../lib/pca.hpp"
#include "../ui_pcadialog.h"

#include <set>
#include <QChartView>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/QScatterDataArray>

#define MAX_MIN_SET(id) do { \
    if(v_##id>v_##id##_max){\
    v_##id##_max = v_##id;\
    }\
    if(v_##id<v_##id##_min){\
    v_##id##_min = v_##id;\
    }                            \
} while(0);

#define OFFSET(id) do { \
    v_##id##_offset = (v_##id##_max-v_##id##_min)*0.1;         \
} while(0);

PCADialog::PCADialog(QWidget *parent, QTableWidget *tableWidget) :
        QDialog(parent),
        ui(new Ui::PCADialog), tableWidget(tableWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    std::set<int> columns;
    for (const auto &item: tableWidget->selectedItems()) {
        columns.insert(item->column());
    }
    int diagnosis_column;
    for (int i = 0; i < tableWidget->columnCount(); i++) {
        auto item_data = tableWidget->item(1, i)->text();
        if (item_data[0] == 'B' || item_data[0] == 'M') {
            diagnosis_column = i;
            break;
        }
    }
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        auto item_data = tableWidget->item(i, diagnosis_column)->text();
        if (item_data[0] == 'B') {
            point_status.emplace_back(false);
        }
        if (item_data[0] == 'M') {
            point_status.emplace_back(true);
        }
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
        q3DScatter->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);
        q3DScatter->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
        q3DScatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
        chartWidget = QWidget::createWindowContainer(q3DScatter);
        q3DScatter->setFlags(q3DScatter->flags() ^ Qt::FramelessWindowHint);
//        chartWidget = new QWidget(this);
        chartWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        /* todo: check here */
        ui->horizontalLayout->addWidget(chartWidget);
        ui->horizontalLayout->setStretch(0, 1);
        ui->horizontalLayout->setStretch(1, 3);
        auto *b_series = new QScatter3DSeries;
        auto *m_series = new QScatter3DSeries;
        QScatterDataArray b_data;
        QScatterDataArray m_data;
        auto res = pca(points, 3);
        float v_0_max = -1000, v_1_max = -1000, v_2_max = -1000;
        float v_0_min = 1000, v_1_min = 1000, v_2_min = 1000;
        for (int i = 0; i < points.size(); i++) {
            float v_0 = res(i, 0);
            float v_1 = res(i, 1);
            float v_2 = res(i, 2);
            if (point_status[i]) {
                m_data << QVector3D(v_0, v_1, v_2);
            } else {
                b_data << QVector3D(v_0, v_1, v_2);
            }
            MAX_MIN_SET(0);
            MAX_MIN_SET(1);
            MAX_MIN_SET(2);
        }
        float v_0_offset, v_1_offset, v_2_offset;
        OFFSET(0);
        OFFSET(1);
        OFFSET(2);
        q3DScatter->axisX()->setTitle("PC1");
        q3DScatter->axisY()->setTitle("PC2");
        q3DScatter->axisZ()->setTitle("PC3");
        q3DScatter->axisX()->setRange(v_0_min - v_0_offset, v_0_max + v_0_offset);
        q3DScatter->axisY()->setRange(v_1_min - v_1_offset, v_1_max + v_1_offset);
        q3DScatter->axisZ()->setRange(v_2_min - v_2_offset, v_2_max + v_2_offset);
        b_series->dataProxy()->addItems(b_data);
        b_series->setName("良性肿瘤");
        b_series->setItemSize(0.1f);
        b_series->setBaseColor(QColor("LimeGreen"));
        m_series->dataProxy()->addItems(m_data);
        m_series->setName("恶性肿瘤");
        m_series->setItemSize(0.1f);
        m_series->setBaseColor(QColor("Crimson"));
        q3DScatter->addSeries(b_series);
        q3DScatter->addSeries(m_series);
        q3DScatter->setAspectRatio(1);
        q3DScatter->setHorizontalAspectRatio(1);
    }
}
