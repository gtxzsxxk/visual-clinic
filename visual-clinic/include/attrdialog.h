#ifndef ATTRDIALOG_H
#define ATTRDIALOG_H

#include <QDialog>

namespace Ui {
class AttrDialog;
}

class AttrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttrDialog(QWidget *parent = nullptr);
    ~AttrDialog();

private:
    Ui::AttrDialog *ui;
};

#endif // ATTRDIALOG_H
