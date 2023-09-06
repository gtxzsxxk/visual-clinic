//
// Created by hanyuan on 2023/9/6.
//

#ifndef VISUAL_CLINIC_FILETAB_H
#define VISUAL_CLINIC_FILETAB_H

#include <QObject>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <vector>
#include <QTableWidget>

class FileTab : public QFrame {
Q_OBJECT

public:
    static std::vector<FileTab *> fileTabs;

    explicit FileTab(QWidget *parent, QTableWidget *tableWidget, const QString &filepath);

    void select();

    void unselect();

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    int index;
    bool selected;
    QString filename;
    const QString &filepath;
    QLabel *label;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *buttonCloseCurrentFile;
    QTableWidget *tableWidget;
    std::vector<QString> CSVLines;

    void readCSV();

    void loadTable();

signals:

    void tabSelected(int tabIndex);

    void tabClosed(int tabIndex);
};


#endif //VISUAL_CLINIC_FILETAB_H
