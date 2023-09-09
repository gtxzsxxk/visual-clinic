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
    bool is_relate_co = true;
    QTableWidget *tableWidget;
    std::vector<std::pair<int, QString>> column_name_pairs;

    void draw_hotgraph();

    void update_data();

private slots:

    void onSwitchButtonClicked();
};

#endif // RELATEDIALOG_H
