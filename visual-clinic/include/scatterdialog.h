#ifndef SCATTERDIALOG_H
#define SCATTERDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <vector>
#include <QMessageBox>
#include <QScatterSeries>
#include <QValueAxis>
#include <QSplineSeries>
#include <QChart>

namespace Ui {
class ScatterDialog;
}

class ScatterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScatterDialog(QWidget *parent,QTableWidget* tableWidget);
    ~ScatterDialog();

private:
    Ui::ScatterDialog *ui;
    float ind_max;
    float ind_min;
    float dep_max;
    float dep_min;
    QScatterSeries *scatter_series= nullptr;
    QValueAxis *axisX= nullptr;
    QValueAxis *axisY= nullptr;
    QChart *chart= nullptr;
    std::vector<float> data_col_ind;
    std::vector<float> data_col_dep;
    QString name_col_ind;
    QString name_col_dep;
    QTableWidget *tableWidget;
    void draw_chart();
private slots:
    void swap_variables();
};

#endif // SCATTERDIALOG_H
