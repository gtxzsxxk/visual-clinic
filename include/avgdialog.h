#ifndef AVGDIALOG_H
#define AVGDIALOG_H

#include <QDialog>
#include <vector>
#include <map>
#include <QChart>
#include <QPoint>
#include <QSplineSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>

namespace Ui {
    class AvgDialog;
}

class AvgDialog : public QDialog {
Q_OBJECT

public:
    explicit AvgDialog(QWidget *parent, std::vector<float> data, const QString &&name, bool discrete = false,
                       int discrete_categories = 2);

    ~AvgDialog();

private:
    /* 选中的列 */
    const QString name;
    float average;
    float variance;
    /* 本列是否为离散数据的一列 */
    bool discrete_flag;
    /* 是否启用了正态分布的显示 */
    bool normal_distribution_enabled;
    QSplineSeries *lineseries = nullptr;
    QValueAxis *axisY_n = nullptr;
    Ui::AvgDialog *ui;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    /* chart的上界 */
    int upper_bound = 0;
    std::vector<float> data;
    std::map<float, int> coordinates;

    void data_initialize(double groups = 0);

    void chart_initialize();

private slots:

    void onSpinBoxValueChanged(int value);

    void onNormalDistributionSet();
};

#endif // AVGDIALOG_H
