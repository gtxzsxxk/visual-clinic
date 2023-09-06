#ifndef AVGDIALOG_H
#define AVGDIALOG_H

#include <QDialog>
#include <vector>

namespace Ui {
class AvgDialog;
}

class AvgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AvgDialog(QWidget *parent,const std::vector<double>&& data,bool discrete = false);
    ~AvgDialog();

private:
    Ui::AvgDialog *ui;
};

#endif // AVGDIALOG_H
