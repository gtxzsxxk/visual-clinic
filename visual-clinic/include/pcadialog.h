#ifndef PCADIALOG_H
#define PCADIALOG_H

#include <QDialog>

namespace Ui {
class PCADialog;
}

class PCADialog : public QDialog
{
    Q_OBJECT

public:
    explicit PCADialog(QWidget *parent = nullptr);
    ~PCADialog();

private:
    Ui::PCADialog *ui;
};

#endif // PCADIALOG_H
