#include "../include/relatedialog.h"
#include "../lib/math_common.h"
#include "../ui_relatedialog.h"

#include <QTableWidgetItem>
#include <set>

RelateDialog::RelateDialog(QWidget *parent, QTableWidget *tableWidget) :
        QDialog(parent),
        ui(new Ui::RelateDialog), tableWidget(tableWidget) {
    ui->setupUi(this);
    setWindowIcon(QIcon("resources/logo128.png"));
    /* 设置内存策略 */
    setAttribute(Qt::WA_DeleteOnClose);

    /* 初始化表头 */
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

    connect(ui->toggle_btn, SIGNAL(clicked()), this, SLOT(onSwitchButtonClicked()));
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
        auto *vertical_headers = new QHeaderView(Qt::Vertical);
        ui->hotgraph->setVerticalHeader(vertical_headers);
        ui->hotgraph->setHorizontalHeaderLabels(headers);
        ui->hotgraph->setVerticalHeaderLabels(headers);
        int section_size = 400 / column_name_pairs.size();
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
    for (const auto &it: column_name_pairs) {
        std::vector<float> vec;
        for (int r = 0; r < tableWidget->rowCount(); r++) {
            auto data = tableWidget->item(r, it.first)->text();
            vec.emplace_back(data.toFloat());
        }
        mat_input.emplace_back(std::move(vec));
    }
    Eigen::MatrixXf mat_covariance = getCovariance(mat_input);
    Eigen::MatrixXf result;
    if (!is_relate_co) {
        //covariance
        result = std::move(mat_covariance);
        ui->mode_label->setText("协方差模式");
        ui->toggle_btn->setText("切换相关系数矩阵");
    } else {
        //relate coefficient
        std::vector<float> vars;
        for (const auto &vec: mat_input) {
            auto avg = getAvgVar(vec);
            vars.emplace_back(std::get<1>(avg));
        }
        result = getPearsonCorr(mat_covariance, vars);
        ui->mode_label->setText("Pearson相关系数模式");
        ui->toggle_btn->setText("切换协方差矩阵");
    }

    float max_value = -1000, min_value = 1000;
    for (int i = 0; i < column_name_pairs.size(); i++) {
        for (int j = 0; j < column_name_pairs.size(); j++) {
            float value = result(i, j);
            if (value > max_value) {
                max_value = value;
            }
            if (value < min_value) {
                min_value = value;
            }
        }
    }
    min_value = is_relate_co ? 0 : min_value;

    /* 线性颜色 */
    auto color = [&](float x) {
        float r_max = 160;
        int offset = 20;
        float k = (510 - offset - r_max) / (min_value - max_value);
        float b = r_max + (510 - offset - r_max) * max_value / (max_value - min_value);
        return static_cast<int>(k * x + b);
    };

    /* 二次曲线 */
    auto color_ploy = [&](float x) {
        float r_max = 140;
        float offset = 15;
        float top = 510 - offset;
        float A = (top - r_max) / (max_value - min_value) / (max_value - min_value);
        return static_cast<int>(-A * (x - min_value) * (x - min_value) + top);
    };

    /* 选择颜色模式 */
    auto color_func = color_ploy;

    for (int i = 0; i < column_name_pairs.size(); i++) {
        for (int j = 0; j < column_name_pairs.size(); j++) {
            float value = result(i, j);
            ui->hotgraph->item(i, j)->setText(QString::number(value, 'g', 3));
            ui->hotgraph->item(i, j)->setFlags(
                    ui->hotgraph->item(i, j)->flags() & ~Qt::ItemIsSelectable
            );
            ui->hotgraph->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            int color_value = color_func(value);
            QColor qColor;
            if (color_value < 255) {
                qColor = QColor(color_value, 0, 0);
                ui->hotgraph->item(i, j)->setBackground(QBrush(
                        qColor
                ));
                ui->hotgraph->item(i, j)->setForeground(QBrush(QColor(255, 255, 255)));
            } else {
                color_value -= 255;
                qColor = QColor(255, color_value, color_value);
                ui->hotgraph->item(i, j)->setBackground(QBrush(
                        qColor
                ));
                ui->hotgraph->item(i, j)->setForeground(QBrush(QColor(0, 0, 0)));
            }
        }
    }
    ui->hotgraph->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->hotgraph->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gradient_frame->setPoints(color_func, max_value, min_value);
}

void RelateDialog::onSwitchButtonClicked() {
    is_relate_co = !is_relate_co;
    update_data();
}
