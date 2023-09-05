#ifndef DATETIMEPICKER_H
#define DATETIMEPICKER_H

#include <QDialog>

namespace Ui {
class DateTimePicker;
}

class DateTimePicker : public QDialog
{
    Q_OBJECT

public:
    explicit DateTimePicker(QWidget *parent = nullptr);
    ~DateTimePicker();

private:
    Ui::DateTimePicker *ui;
};

#endif // DATETIMEPICKER_H
