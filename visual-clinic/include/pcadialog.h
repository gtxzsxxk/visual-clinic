#ifndef PCADIALOG_H
#define PCADIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <vector>
#include <QWidget>
#include <QtDataVisualization/Q3DScatter>

namespace Ui {
class PCADialog;
}

class PCADialog : public QDialog
{
    Q_OBJECT

public:
    explicit PCADialog(QWidget *parent,QTableWidget *tableWidget);
    ~PCADialog();

private:
    Ui::PCADialog *ui;
    QTableWidget *tableWidget;
    Q3DScatter *q3DScatter;
    QWidget *chartWidget;
    std::vector<std::vector<float>> points;
    void go_PCA();
};

#endif // PCADIALOG_H
