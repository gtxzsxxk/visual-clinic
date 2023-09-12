#include "../include/meansdialog.h"
#include "../lib/pca.hpp"
#include "../ui_meansdialog.h"

#include <Q3DScatter>
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

MeansDialog::MeansDialog(QWidget *parent, QTableWidget* tableWidget) :
    QDialog(parent),
    ui(new Ui::MeansDialog),tableWidget(tableWidget)
{
    ui->setupUi(this);
}

MeansDialog::~MeansDialog()
{
    delete ui;
}

void MeansDialog::init_3d_scatter() {
    reset_memory();
    auto *q3DScatter = new Q3DScatter();
    q3DScatter->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);
    q3DScatter->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    q3DScatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
    scatter_3d_widget = QWidget::createWindowContainer(q3DScatter);
    q3DScatter->setFlags(q3DScatter->flags() ^ Qt::FramelessWindowHint);
    scatter_3d_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->horizontalLayout_3->addWidget(scatter_3d_widget);
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

void MeansDialog::init_2d_scatter() {
    reset_memory();

    scatter_2d_widget = new QChartView(this);
    ui->horizontalLayout_3->addWidget(scatter_2d_widget);
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

void MeansDialog::reset_memory() {
    if (scatter_3d_widget && ui->horizontalLayout_3->count() == 2) {
        ui->horizontalLayout_3->removeWidget(scatter_3d_widget);
        delete scatter_3d_widget;
        scatter_3d_widget = nullptr;
    }
    if (scatter_2d_widget && ui->horizontalLayout_3->count() == 2) {
        ui->horizontalLayout_3->removeWidget(scatter_2d_widget);
        delete scatter_2d_widget;
        scatter_2d_widget = nullptr;
    }
}
