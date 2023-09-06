#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/XMainWindow.h"
#include "FileTab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public XMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool fileOpened;
    QString currentFilePath;
    std::vector<QString> currentCSVLines;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void goAttributionAnalysis();
    void goAvgAndVari();
    void goMeans();
    void goScatter();
    void goRelate();
    void goPCA();
    void importCSV();
    void titleBarAdd(const QString& path);
public slots:
    void quit();
    void minimize();
    void tabSelected(int tabIndex);
    void tabClosed(int tabIndex);
};
#endif // MAINWINDOW_H
