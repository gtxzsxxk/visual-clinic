#include "../include/scatterdialog.h"
#include "../ui_scatterdialog.h"

ScatterDialog::ScatterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScatterDialog)
{
    ui->setupUi(this);
}

ScatterDialog::~ScatterDialog()
{
    delete ui;
}
