#include "../include/relatedialog.h"
#include "../ui_relatedialog.h"

RelateDialog::RelateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RelateDialog)
{
    ui->setupUi(this);
}

RelateDialog::~RelateDialog()
{
    delete ui;
}
