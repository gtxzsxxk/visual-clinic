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

class FileTab : public QFrame {
Q_OBJECT

public:
    static std::vector<FileTab*> fileTabs;
    explicit FileTab(QWidget *parent, const QString &filename);
    void select();
    void unselect();
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    bool selected;
    const QString &filename;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label;
    QPushButton *buttonCloseCurrentFile;

};


#endif //VISUAL_CLINIC_FILETAB_H
