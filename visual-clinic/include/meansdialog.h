#ifndef MEANSDIALOG_H
#define MEANSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QChartView>

namespace Ui {
class MeansDialog;
}

class MeansDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeansDialog(QWidget *parent, QTableWidget* tableWidget);
    ~MeansDialog();

private:
    Ui::MeansDialog *ui;
    /* 聚类算法，0 for K-means */
    int means_flag = 0;
    QTableWidget *tableWidget;
    QWidget *scatter_3d_widget = nullptr;
    QChartView *scatter_2d_widget = nullptr;
    std::vector<std::vector<float>> points;
    std::vector<bool> point_status;

    void go_Means();

    void reset_memory();

    void init_3d_scatter();

    void init_2d_scatter();
};

#endif // MEANSDIALOG_H
