#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->lineEdit && event->type() == QEvent::KeyPress) {
        auto *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            ui->textEdit->insertPlainText(ui->lineEdit->text() + "\r\n");
            ui->textEdit->moveCursor(QTextCursor::End);
            ui->lineEdit->clear();
        }
    }
    return QObject::eventFilter(watched, event);
}

