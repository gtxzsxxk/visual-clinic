#ifndef PCADIALOG_H
#define PCADIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class PCADialog;
}

class PCADialog : public QDialog
{
    Q_OBJECT

public:
    explicit PCADialog(QWidget *parent,QTableWidget *tableWidget);
    ~PCADialog();

private:
    Ui::PCADialog *ui;
};

#endif // PCADIALOG_H
