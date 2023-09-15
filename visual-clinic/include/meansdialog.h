#ifndef MEANSDIALOG_H
#define MEANSDIALOG_H

#include <vector>
#include <set>
#include <QColor>
#include <QDialog>
#include <QTableWidget>
#include <QChartView>

namespace Ui {
    class MeansDialog;
}

class MeansDialog : public QDialog {
Q_OBJECT

public:
    explicit MeansDialog(QWidget *parent, QTableWidget *tableWidget);

    ~MeansDialog();

private:
    Ui::MeansDialog *ui;
    int columns_number;
    bool show_color = true;
    bool real_categories = false;
    /* 聚类算法，0 for K-means */
    int means_flag = 0;
    int categories_number = 0;
    QTableWidget *tableWidget;
    QWidget *scatter_3d_widget = nullptr;
    QChartView *scatter_2d_widget = nullptr;
    std::vector<std::vector<float>> points;
    std::vector<bool> point_status;
    std::vector<int> point_categories;
    std::vector<QColor> point_colors;

    void go_Means();

    int get_set_index(const std::set<int> &data, int value);

    void set_table_colors();

    void set_algorithm_name(const QString &&name);

    void set_table_column_categories();

    void reset_table_colors();

    void reset_memory();

    void init_3d_scatter();

    void init_2d_scatter();

private slots:

    void onColorSwitchClicked();

    void onDisplayTypeChanged();

    void onSpinBoxValueChanged(int value);

    void onSetKmeans();

    void onSetDBSCAN();

    void onSetMeanShift();
};

#endif // MEANSDIALOG_H
