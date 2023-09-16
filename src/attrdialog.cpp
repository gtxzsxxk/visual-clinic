#include "../include/attrdialog.h"
#include "../ui_attrdialog.h"

AttrDialog::AttrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttrDialog)
{
    ui->setupUi(this);
}

AttrDialog::~AttrDialog()
{
    delete ui;
}
