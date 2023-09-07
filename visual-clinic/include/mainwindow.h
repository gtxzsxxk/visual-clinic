#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/XMainWindow.h"
#include "FileTab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public XMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool fileOpened;
    int selected_column;
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

    void titleBarAdd(const QString &path);

    bool isSelectingMultipleColumns();

    bool isSelectingTwoColumns();

    bool isSelectingEntireColumn();

public slots:

    void quit();

    void minimize();

    void tabSelected();

    void onTabClosed(int tabIndex);

    void onTableHeaderSelected(int index);
};

#endif // MAINWINDOW_H
