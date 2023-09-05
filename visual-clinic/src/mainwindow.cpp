#include "../include/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : XMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

