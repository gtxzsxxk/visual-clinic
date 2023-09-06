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
    explicit AvgDialog(QWidget *parent, std::vector<float> data, const QString &&name, bool discrete = false,
                       int discrete_categories = 2);

    ~AvgDialog();

private:
    const QString name;
    Ui::AvgDialog *ui;
    std::vector<float> data;
    std::map<float, int> coordinates;

    void data_initialize(double groups = 0);

    void chart_initialize() const;

private slots:

    void onSpinBoxValueChanged(int value);
};

#endif // AVGDIALOG_H
