#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    dateTimePicker = new DateTimePicker(this);
    dateTimePicker->setModal(true);
    connect(ui->action_3, SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->actionshezhiriqi, SIGNAL(triggered()), this, SLOT(openDateTimeSettingDialog()));
    auto date_now = QDate::currentDate();
    setDate(date_now.year(), date_now.month(), date_now.day());
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << date_now.year() << "-" << std::setw(2) << date_now.month()
       << "-" << std::setw(2) << date_now.day();
    default_date = QString(ss.str().c_str());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::openDateTimeSettingDialog() {
    bool ok = false;
    auto date = QInputDialog::getText(this, "Date Picker",
                                      "请输入日期(格式为：yyyy-mm-dd)",
                                      QLineEdit::Normal, default_date, &ok);
    if (ok) {
        QString pattern =
                QString("(([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})-(((0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|") +
                QString("((0[469]|11)-(0[1-9]|[12][0-9]|30))|(02-(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|") +
                QString("((0[48]|[2468][048]|[3579][26])00))-02-29)$");
        QRegularExpression re(pattern);
        if (re.match(date).hasMatch()) {
            auto splits = date.split('-');
            auto year = splits[0].toInt();
            auto month = splits[1].toInt();
            auto day = splits[2].toInt();
            if (!is_leap_year(year)) {
                if (month == 2 && day == 29) {
                    QMessageBox::warning(this, "错误", "不是闰年");
                    return;
                }
            }
            if (day >= 1 && day <= 31) {
                setDate(year, month, day);
                QString info = "已将日期设置为" + QString::number(year) + "年"
                               + QString::number(month) + "月" + QString::number(day) + "日";
                QMessageBox::information(this, "日期设置成功", info);
                return;
            }
        }
        QMessageBox::warning(this, "错误", "在日期文本中检测到错误");
    }
}

void MainWindow::setDate(int year, int month, int day) {
    ui->yearLCD->display(year);
    ui->monthLCD->display(month);
    ui->dayLCD->display(day);
}

bool MainWindow::is_leap_year(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else {
        return false;
    }
}

