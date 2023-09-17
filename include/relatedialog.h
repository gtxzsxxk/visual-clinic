#ifndef RELATEDIALOG_H
#define RELATEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <vector>

namespace Ui {
    class RelateDialog;
}

class RelateDialog : public QDialog {
Q_OBJECT

public:
    explicit RelateDialog(QWidget *parent, QTableWidget *tableWidget);

    ~RelateDialog();

private:
    Ui::RelateDialog *ui;
    bool graph_initialized = false;
    /* 是否显示相关性系数 */
    bool is_relate_co = true;
    /* 主页面的表格控件 */
    QTableWidget *tableWidget;
    /* 表格的表头index和字符串 */
    std::vector<std::pair<int, QString>> column_name_pairs;

    void draw_hotgraph();

    void update_data();

private slots:

    void onSwitchButtonClicked();
};

#endif // RELATEDIALOG_H
