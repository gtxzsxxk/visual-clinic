#ifndef AVGDIALOG_H
#define AVGDIALOG_H

#include <QDialog>
#include <vector>
#include <map>

namespace Ui {
    class AvgDialog;
}

class AvgDialog : public QDialog {
Q_OBJECT

public:
    explicit AvgDialog(QWidget *parent, std::vector<double> &&data, const QString &&name, bool discrete = false);

    ~AvgDialog();

private:
    const QString &name;
    Ui::AvgDialog *ui;
    std::vector<double> data;
    std::map<double, int> coordinates;

    void data_initialize();

    void chart_initialize() const;
};

#endif // AVGDIALOG_H
