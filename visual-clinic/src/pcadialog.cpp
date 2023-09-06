#include "../include/pcadialog.h"
#include "../ui_pcadialog.h"

PCADialog::PCADialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PCADialog)
{
    ui->setupUi(this);
}

PCADialog::~PCADialog()
{
    delete ui;
}
