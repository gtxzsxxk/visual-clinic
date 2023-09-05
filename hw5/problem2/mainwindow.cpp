#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_3, SIGNAL(triggered()),this,SLOT(quit()));
    connect(ui->actionshezhiriqi, SIGNAL(triggered()),this,SLOT(openDateTimeSettingDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::openDateTimeSettingDialog() {

}

