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
    /* 用于存储所有FileTab的对象 */
    static std::vector<FileTab *> fileTabs;
    /* 当前所有打开的FileTab所占用的空间大小 */
    static int tab_total_size;

    explicit FileTab(QWidget *parent, QTableWidget *tableWidget, const QString &filepath);

    ~FileTab();

    void select();

    void unselect();

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    /* 容器x坐标占用大小 */
    int container_width;
    /* 当前对象的下标 */
    int index;
    bool selected;
    QString filename;
    /* 当前Tab对应的文件路径 */
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
