#include "datetimepicker.h"
#include "ui_datetimepicker.h"

DateTimePicker::DateTimePicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateTimePicker)
{
    ui->setupUi(this);
}

DateTimePicker::~DateTimePicker()
{
    delete ui;
}
