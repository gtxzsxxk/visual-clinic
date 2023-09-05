#include "../include/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : XMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonClose,SIGNAL(clicked()),this,SLOT(quit()));
    connect(ui->buttonMinimize,SIGNAL(clicked()),this,SLOT(minimize()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::minimize() {
    this->showMinimized();
}

