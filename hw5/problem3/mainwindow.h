#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *scanner;
    QString image_path;
    std::vector<std::pair<QString,QString>> images;
    int image_ptr;
    void lookup_images();
    QSize optimize_size(QSize frame, QSize image) const;
private slots:
    void next_image();
    void timer_timeout();
};
#endif // MAINWINDOW_H
