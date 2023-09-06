#include "../include/avgdialog.h"
#include "../ui_avgdialog.h"

#include <QLineSeries>

AvgDialog::AvgDialog(QWidget *parent,const std::vector<double>&& data,bool discrete) :
    QDialog(parent),
    ui(new Ui::AvgDialog)
{
    ui->setupUi(this);
    /* find the range */

}

AvgDialog::~AvgDialog()
{
    delete ui;
}
