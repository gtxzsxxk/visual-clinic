#ifndef INDEX_H
#define INDEX_H

#include "XMainWindow.h"
#include "FileTab.h"

#include <QDialog>

namespace Ui {
    class Index;
}

class Index : public XMainWindow {
Q_OBJECT

public:
    explicit Index(QWidget *parent = nullptr);

    ~Index();

private:
    Ui::Index *ui;
    int selected_column = 0;
    int column_selected_num = 0;
    bool file_full=false;
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

#endif // INDEX_H
