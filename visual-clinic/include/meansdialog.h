#ifndef MEANSDIALOG_H
#define MEANSDIALOG_H

#include <QDialog>

namespace Ui {
class MeansDialog;
}

class MeansDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeansDialog(QWidget *parent = nullptr);
    ~MeansDialog();

private:
    Ui::MeansDialog *ui;
};

#endif // MEANSDIALOG_H
