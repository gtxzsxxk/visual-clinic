#include "../include/meansdialog.h"
#include "../ui_meansdialog.h"

MeansDialog::MeansDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeansDialog)
{
    ui->setupUi(this);
}

MeansDialog::~MeansDialog()
{
    delete ui;
}
