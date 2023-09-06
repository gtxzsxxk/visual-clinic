#ifndef SCATTERDIALOG_H
#define SCATTERDIALOG_H

#include <QDialog>

namespace Ui {
class ScatterDialog;
}

class ScatterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScatterDialog(QWidget *parent = nullptr);
    ~ScatterDialog();

private:
    Ui::ScatterDialog *ui;
};

#endif // SCATTERDIALOG_H
