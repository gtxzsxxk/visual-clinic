#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), cesius_degree(0) {
    ui->setupUi(this);
    cesius_degree_set(26);
    connect(ui->celsiusDial, SIGNAL(valueChanged(int)), this, SLOT(cesius_degree_updated(int)));
    connect(ui->fahrenheitDial, SIGNAL(valueChanged(int)), this, SLOT(fahrenheit_degree_updated(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::cesius_degree_set(double deg) {
    int deg_int = static_cast<int>(deg);
    if (deg_int == static_cast<int>(cesius_degree)) {
        return;
    }
    cesius_degree = deg;
    int deg_fa_int = static_cast<int>(celsius2fahrenheit(deg));
    ui->celsiusDial->setValue(deg_int);
    ui->celsiusLcd->display(deg_int);
    ui->fahrenheitDial->setValue(deg_fa_int);
    ui->fahrenheitLcd->display(deg_fa_int);
}

void MainWindow::cesius_degree_updated(int deg) {
    cesius_degree_set(deg);
}

void MainWindow::fahrenheit_degree_updated(int deg) {
    double fa = fahrenheit2celsius(deg);
    cesius_degree_set(fa);
}

double MainWindow::celsius2fahrenheit(double cel) {
    return cel * 9 / 5 + 32;
}

double MainWindow::fahrenheit2celsius(double fah) {
    return (fah - 32) * 5 / 9;
}

