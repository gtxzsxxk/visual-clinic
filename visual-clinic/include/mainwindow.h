#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/XMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public XMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void quit();
    void minimize();
};
#endif // MAINWINDOW_H
