#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSignalMapper>
#include <sstream>

#define NUMPAD(X) do { \
connect(ui->numButton_##X,SIGNAL(clicked()),mapper, SLOT(map())); \
mapper->setMapping(ui->numButton_##X,X); \
} while(0);

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mapper = new QSignalMapper(this);
    NUMPAD(0);
    NUMPAD(1);
    NUMPAD(2);
    NUMPAD(3);
    NUMPAD(4);
    NUMPAD(5);
    NUMPAD(6);
    NUMPAD(7);
    NUMPAD(8);
    NUMPAD(9);

    connect(mapper, SIGNAL(mappedInt(int)), this, SLOT(actionPerformed(int)));
    connect(ui->numButton_ac, &QPushButton::clicked, [&]() {
        ui->lineEdit->clear();
    });
}

MainWindow::~MainWindow() {
    delete mapper;
    delete ui;
}

void MainWindow::actionPerformed(int key) {
    ui->lineEdit->setText(ui->lineEdit->text() + QString::number(key));
}

