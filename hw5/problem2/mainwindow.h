#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datetimepicker.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DateTimePicker* dateTimePicker;
    QString default_date;
    void setDate(int year,int month,int day);
    static bool is_leap_year(int year);
private slots:
    void quit();
    void openDateTimeSettingDialog();
};
#endif // MAINWINDOW_H
