#ifndef INDEX_H
#define INDEX_H

#include "XMainWindow.h"
#include "FileTab.h"
#include "TableValidator.h"

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
    /* 窗口是否最大化 */
    bool maximum_flag = false;
    /* 选中的列的数目 */
    int selected_column = 0;
    /* 选中的列的数目 */
    int column_selected_num = 0;
    /* 是否已经将titlebar填满 */
    bool file_full = false;
    /* 表格数据验证器 */
    TableValidator* tableValidator;

    bool eventFilter(QObject *watched, QEvent *event) override;

    void goAttributionAnalysis();

    void goAvgAndVari();

    void goMeans();

    void goScatter();

    void goRelate();

    void goPCA();

    void importCSV();

    void titleBarAdd(const QString &path);

public slots:

    void quit();

    void minimize();

    void normal_maximum();

    void tabSelected();

    void onTabClosed(int tabIndex);

    void onTableHeaderSelected(int index);
};

#endif // INDEX_H
