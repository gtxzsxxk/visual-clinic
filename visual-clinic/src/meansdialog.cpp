#include "../include/meansdialog.h"
#include "../lib/kmeans.hpp"
#include "../ui_meansdialog.h"

#include <set>
#include <random>
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

MeansDialog::MeansDialog(QWidget *parent, QTableWidget *tableWidget) :
        QDialog(parent),
        ui(new Ui::MeansDialog), tableWidget(tableWidget) {
    ui->setupUi(this);
    std::set<int> columns, rows;
    for (const auto &item: tableWidget->selectedItems()) {
        columns.insert(item->column());
        rows.insert(item->row());
    }
    /* 把数据加到自己的小表格里 */
    ui->tableWidget->setRowCount(rows.size());
    ui->tableWidget->setColumnCount(columns.size());
    for (const auto &item: tableWidget->selectedItems()) {
        int col = get_set_index(columns, item->column());
        int ro = get_set_index(rows, item->row());
        ui->tableWidget->setItem(ro, col, new QTableWidgetItem(item->text()));
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
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        std::vector<float> data;
        for (const auto &it: columns) {
            data.emplace_back(tableWidget->item(i, it)->text().toFloat());
        }
        points.emplace_back(data);
    }

    connect(ui->showcolor_btn, SIGNAL(clicked()), this, SLOT(onColorSwitchClicked()));

    go_Means();
}

MeansDialog::~MeansDialog() {
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
    ui->horizontalLayout_3->setStretch(0,3);
    ui->horizontalLayout_3->setStretch(1,5);

    auto *b_series = new QScatter3DSeries;
    auto *m_series = new QScatter3DSeries;
    QScatterDataArray b_data;
    QScatterDataArray m_data;
    auto res = kpca(points, 3);
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
    ui->horizontalLayout_3->setStretch(0,3);
    ui->horizontalLayout_3->setStretch(1,5);
    auto *chart = new QChart();
    if (real_categories) {
        auto *b_series = new QScatterSeries(this);
        b_series->setName("良性肿瘤");
        b_series->setMarkerSize(10);
        b_series->setColor(QColor("LimeGreen"));
        auto *m_series = new QScatterSeries(this);
        m_series->setName("恶性肿瘤");
        m_series->setMarkerSize(10);
        m_series->setColor(QColor("Crimson"));

        auto res = kpca(points, 2);
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
    } else {
        auto res = kpca(points, 2);
        float v_0_max = -1000, v_1_max = -1000;
        float v_0_min = 1000, v_1_min = 1000;
        for (int i = 0; i < points.size(); i++) {
            float v_0 = res(i, 0);
            float v_1 = res(i, 1);
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
        for (int i = 0; i < point_colors.size(); i++) {
            auto *series = new QScatterSeries(this);
            series->setName("聚类" + QString::number(i));
            series->setMarkerSize(10);
            series->setColor(point_colors[i]);
            for (int j = 0; j < points.size(); j++) {
                if (point_categories[j] == i) {
                    series->append(QPointF(res(j, 0), res(j, 1)));
                }
            }
            chart->addSeries(series);
            series->attachAxis(axisX);
            series->attachAxis(axisY);
        }
    }

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

void MeansDialog::go_Means() {
    if (means_flag == 0) {
        int k = ui->point_spinbox->text().toInt();
        int iter = ui->iterate_spinbox->text().toInt();
        auto res = clusterKMeans(points, k, iter);
        point_categories = std::get<1>(res);
        int number = 0;
        for (const auto &it: point_categories) {
            if (it > number) {
                number = it;
            }
        }
        std::random_device r_dev;
        std::mt19937 gen(r_dev());
        std::uniform_int_distribution<> distribution(100, 255);
        point_colors.clear();
        for (int i = 0; i <= number; i++) {
            point_colors.emplace_back(distribution(gen), distribution(gen), distribution(gen));
        }
    }
    if (show_color) {
        set_table_colors();
    } else {
        reset_table_colors();
    }
    int dim = ui->dim_spinbox->value();
    if (dim == 3) {
        init_3d_scatter();
    } else {
        init_2d_scatter();
    }
}

void MeansDialog::set_table_colors() {
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            int category = point_categories[i];
            ui->tableWidget->item(i, j)->setBackground(
                    QBrush(point_colors[category])
            );
        }
    }
}

void MeansDialog::reset_table_colors() {
    QTableWidgetItem deflt;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            ui->tableWidget->item(i, j)->setBackground(deflt.background());
        }
    }
}

int MeansDialog::get_set_index(const std::set<int> &data, int value) {
    int cnt = 0;
    for (const auto &it: data) {
        if (it == value) {
            return cnt;
        }
        cnt++;
    }
    return cnt;
}

void MeansDialog::onColorSwitchClicked() {
    if (show_color) {
        reset_table_colors();
        show_color = false;
    } else {
        set_table_colors();
        show_color = true;
    }
}
