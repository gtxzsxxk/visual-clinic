#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double cesius_degree;

    void cesius_degree_set(double deg);

    static double celsius2fahrenheit(double cel);

    static double fahrenheit2celsius(double fah);

private slots:

    void cesius_degree_updated(int deg);

    void fahrenheit_degree_updated(int deg);
};

#endif // MAINWINDOW_H
