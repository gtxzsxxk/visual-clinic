#ifndef RELATEDIALOG_H
#define RELATEDIALOG_H

#include <QDialog>

namespace Ui {
class RelateDialog;
}

class RelateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RelateDialog(QWidget *parent = nullptr);
    ~RelateDialog();

private:
    Ui::RelateDialog *ui;
};

#endif // RELATEDIALOG_H
