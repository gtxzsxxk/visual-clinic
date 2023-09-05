#include "../include/mainwindow.h"
#include "../resources/forms/ui_mainwindow.h"

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

