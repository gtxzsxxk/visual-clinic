#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow),
          image_ptr(0) {
    ui->setupUi(this);
    image_path = QDir::currentPath() + "/images/";
    lookup_images();
    connect(ui->imageButton, SIGNAL(clicked()), this, SLOT(next_image()));
    scanner = new QTimer(this);
    connect(scanner, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    scanner->start(200);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::lookup_images() {
    QDir dir(image_path);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    auto list = dir.entryInfoList();
    images.clear();
    for (const auto &img: list) {
        images.emplace_back(img.filePath(), img.fileName());
    }
}

void MainWindow::next_image() {
    if (!images.empty()) {
        QSize image_size = QImage(images[image_ptr].first).size();
        QIcon icon(images[image_ptr].first);
        ui->imageButton->setIcon(icon);
        ui->imageButton->setIconSize(optimize_size(ui->imageButton->size(), image_size));
        ui->filenameLabel->setText(images[image_ptr++].second);
        if (image_ptr >= images.size()) {
            image_ptr = 0;
        }
    } else {
        ui->filenameLabel->setText("No image");
    }
}

QSize MainWindow::optimize_size(QSize frame, QSize image) const {
    double n_x, n_y;
    if (((double) frame.width() / (double) frame.height()) > ((double) image.width() / (double) image.height())) {
        double portion = ((double) frame.height()) / ((double) image.height()) * 0.9;
        n_x = (double) image.width() * portion;
        n_y = (double) image.height() * portion;
    } else {
        double portion = ((double) frame.width()) / ((double) image.width()) * 0.9;
        n_y = (double) image.height() * portion;
        n_x = (double) image.width() * portion;
    }
    return {static_cast<int>(n_x), static_cast<int>(n_y)};
}

void MainWindow::timer_timeout() {
    next_image();
    scanner->stop();
}

