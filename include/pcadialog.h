#ifndef PCADIALOG_H
#define PCADIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QLabel>
#include <QWidget>
#include <QChartView>
#include <QtDataVisualization/Q3DScatter>
#include <vector>

namespace Ui {
    class PCADialog;
}

class PCADialog : public QDialog {
Q_OBJECT

public:
    explicit PCADialog(QWidget *parent, QTableWidget *tableWidget);

    ~PCADialog();

private:
    Ui::PCADialog *ui;
    QLabel *tipLabel = nullptr;
    QTableWidget *tableWidget;
    QWidget *scatter_3d_widget = nullptr;
    QChartView *scatter_2d_widget = nullptr;
    std::vector<std::vector<float>> points;
    std::vector<bool> point_status;

    void go_PCA();

    void reset_memory();

    void init_3d_scatter();

    void init_2d_scatter();

private slots:

    void dimValueChanged(int value);

    void hoverTip_2d(const QPointF &point, bool status);
};

#endif // PCADIALOG_H
