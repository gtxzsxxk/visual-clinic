#include "../include/pcadialog.h"
#include "../lib/math_common.h"
#include "../ui_pcadialog.h"

#include <set>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/QScatterDataArray>
#include <QScatterSeries>
#include <QValueAxis>

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
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(dimValueChanged(int)));

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
    if (dim == 3) {
        init_3d_scatter();
    } else {
        init_2d_scatter();
    }
}

void PCADialog::dimValueChanged(int value) {
    go_PCA();
}

void PCADialog::init_3d_scatter() {
    reset_memory();
    auto *q3DScatter = new Q3DScatter();
    q3DScatter->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);
    q3DScatter->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    q3DScatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
    scatter_3d_widget = QWidget::createWindowContainer(q3DScatter);
    q3DScatter->setFlags(q3DScatter->flags() ^ Qt::FramelessWindowHint);
    scatter_3d_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->horizontalLayout->addWidget(scatter_3d_widget);
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

void PCADialog::init_2d_scatter() {
    reset_memory();

    scatter_2d_widget = new QChartView(this);
    ui->horizontalLayout->addWidget(scatter_2d_widget);
    ui->horizontalLayout->setStretch(0, 1);
    ui->horizontalLayout->setStretch(1, 3);
    auto *chart = new QChart();
    auto *b_series = new QScatterSeries(this);
    b_series->setName("良性肿瘤");
    b_series->setMarkerSize(10);
    b_series->setColor(QColor("LimeGreen"));
    auto *m_series = new QScatterSeries(this);
    m_series->setName("恶性肿瘤");
    m_series->setMarkerSize(10);
    m_series->setColor(QColor("Crimson"));

    auto res = pca(points, 2);
    float v_0_max = -1000, v_1_max = -1000;
    float v_0_min = 1000, v_1_min = 1000;
    for (int i = 0; i < points.size(); i++) {
        float v_0 = res(i, 0);
        float v_1 = res(i, 1);
        if (point_status[i]) {
            m_series->append(QPointF(v_0, v_1));
        } else {
            b_series->append(QPointF(v_0, v_1));
        }
        MAX_MIN_SET(0);
        MAX_MIN_SET(1);
    }

    auto *axisX = new QValueAxis((QObject *) this);
    auto *axisY = new QValueAxis((QObject *) this);
    double h_offset = (v_0_max - v_0_min) * 0.05;
    axisX->setRange(v_0_min - h_offset, v_0_max + h_offset);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%.2f");
    axisX->setTitleText("PC1");
    chart->addAxis(axisX, Qt::AlignBottom);

    double v_offset = (v_1_max - v_1_min) * 0.05;
    axisY->setRange(v_1_min - v_offset, v_1_max + v_offset);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("PC2");
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries(m_series);
    m_series->attachAxis(axisX);
    m_series->attachAxis(axisY);
    chart->addSeries(b_series);
    b_series->attachAxis(axisX);
    b_series->attachAxis(axisY);
    scatter_2d_widget->setChart(chart);
}

void PCADialog::reset_memory() {
    if (scatter_3d_widget && ui->horizontalLayout->count() == 2) {
        ui->horizontalLayout->removeWidget(scatter_3d_widget);
        delete scatter_3d_widget;
        scatter_3d_widget = nullptr;
    }
    if (scatter_2d_widget && ui->horizontalLayout->count() == 2) {
        ui->horizontalLayout->removeWidget(scatter_2d_widget);
        delete scatter_2d_widget;
        scatter_2d_widget = nullptr;
    }
}
