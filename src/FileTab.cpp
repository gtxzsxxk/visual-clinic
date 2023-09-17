//
// Created by hanyuan on 2023/9/6.
//

#include "../include/FileTab.h"

#include <QEvent>
#include <QFontMetrics>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>

std::vector<FileTab *> FileTab::fileTabs;

int FileTab::tab_total_size = 0;

FileTab::FileTab(QWidget *parent, QTableWidget *tableWidget, const QString &filepath) :
        QFrame(parent), filepath(filepath), selected(false), tableWidget(tableWidget) {

    /* 获取文件名 */
    QFile f(filepath);
    if (f.open(QIODevice::ReadOnly)) {
        QFileInfo info(filepath);
        filename = info.fileName();
    } else {
        QMessageBox::warning(this, "错误", "无法打开文件：" + filepath);
    }
    f.close();

    /* 通过文件名设置容器大小 */
    int width = QFontMetrics(this->font()).boundingRect(filename).width() + 10;
    container_width = width + 50;
    tab_total_size += container_width;

    /* 初始化UI界面 */
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(180);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy2);
    setMinimumSize(QSize(container_width, 28));
    setMaximumSize(QSize(container_width, 28));
    setStyleSheet(QString::fromUtf8("QFrame{\n"
                                    "	background:white;\n"
                                    "border:none;\n"
                                    "border-radius:5px;\n"
                                    "padding: 3px 0px 3px 0px;\n"
                                    "border:1px solid #D1D3D5;\n"
                                    "}"));
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
    horizontalLayout_6 = new QHBoxLayout(this);
    horizontalLayout_6->setSpacing(0);
    horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(this);
    label->setMinimumSize(QSize(width, 20));
    label->setMaximumSize(QSize(width, 20));
    label->setStyleSheet(QString::fromUtf8("QLabel {\n"
                                           "	border:none;\n"
                                           "padding-left:0px;background: transparent;\n"
                                           "}"));
    label->setText(filename);
    label->setWordWrap(true);

    horizontalLayout_6->addWidget(label);

    buttonCloseCurrentFile = new QPushButton(this);
    buttonCloseCurrentFile->setMinimumSize(QSize(18, 18));
    buttonCloseCurrentFile->setMaximumSize(QSize(18, 18));
    buttonCloseCurrentFile->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                            "	background:transparent;\n"
                                                            "	border:none;\n"
                                                            "	border-radius:20px;\n"
                                                            "}\n"
                                                            "\n"
                                                            "QPushButton:hover{\n"
                                                            "	background:#efefef;\n"
                                                            "	border:none;\n"
                                                            "	border-radius:20px;\n"
                                                            "}"));
    QIcon icon;
    icon.addFile(QString::fromUtf8("resources/win_close.png"), QSize(), QIcon::Normal, QIcon::Off);
    buttonCloseCurrentFile->setIcon(icon);
    buttonCloseCurrentFile->setIconSize(QSize(28, 28));

    setCursor(QCursor(Qt::PointingHandCursor));

    horizontalLayout_6->addWidget(buttonCloseCurrentFile);


    unselect();
    index = fileTabs.size();
    fileTabs.push_back(this);
    installEventFilter(this);
    label->installEventFilter(this);
    buttonCloseCurrentFile->installEventFilter(this);
    readCSV();
}

FileTab::~FileTab() {
    /* 析构时在filetabs中删掉自己的对象 */
    FileTab::fileTabs.erase(FileTab::fileTabs.begin() + index);
    for(int i=0;i<FileTab::fileTabs.size();i++){
        FileTab::fileTabs[i]->index = i;
    }
    tableWidget->clear();
    while (tableWidget->rowCount()) {
        tableWidget->removeRow(0);
    }
}

void FileTab::select() {
    /* 更新ui，emit信号 */
    for (auto &it: fileTabs) {
        it->unselect();
    }
    setStyleSheet(QString::fromUtf8("QFrame{\n"
                                    "	background:white;\n"
                                    "border:none;\n"
                                    "border-radius:5px;\n"
                                    "padding: 3px 0px 3px 0px;\n"
                                    "border:1px solid #D1D3D5;\n"
                                    "}"));
    selected = true;
    emit tabSelected(index);
    loadTable();
}

void FileTab::unselect() {
    /* 更新ui*/
    setStyleSheet(QString::fromUtf8("QFrame{"
                                    "border:none;"
                                    "background:rgb(219, 222, 224);"
                                    "padding: 3px 0px 3px 0px;\n"
                                    "border-radius:5px;"
                                    "}"
                                    "QFrame:hover{"
                                    "border:none;"
                                    "border-radius:5px;"
                                    "padding: 3px 0px 3px 0px;\n"
                                    "background:rgb(215, 215, 216);"
                                    "}"));
    selected = false;
}

bool FileTab::eventFilter(QObject *watched, QEvent *event) {
    if ((watched == this || watched == label) && event->type() == QEvent::MouseButtonRelease) {
        if (!selected) {
            select();
        }
        return true;
    } else if (watched == buttonCloseCurrentFile && event->type() == QEvent::MouseButtonPress) {
        tab_total_size -= container_width;
        emit tabClosed(index);
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void FileTab::readCSV() {
    QFile f(filepath);
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream stream(&f);
        std::vector<QString> lines;
        while (!stream.atEnd()) {
            lines.push_back(stream.readLine());
        }
        if (lines.size() < 2) {
            QMessageBox::warning(this, "错误", "文件是空的，或者文件格式错误");
        }

        QFileInfo info(filepath);
        filename = info.fileName();
        label->setText(filename);
        CSVLines = std::move(lines);
    } else {
        QMessageBox::warning(this, "错误", "无法打开文件：" + filepath);
    }
    f.close();
}

void FileTab::loadTable() {
    auto header_src = CSVLines[0];
    auto headers = header_src.split(',');
    tableWidget->clear();
    while (tableWidget->rowCount()) {
        tableWidget->removeRow(0);
    }
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    for (size_t i = 1; i < CSVLines.size(); i++) {
        auto data = CSVLines[i].split(',');
        tableWidget->insertRow(tableWidget->rowCount());
        int row_index = tableWidget->rowCount() - 1;
        for (int j = 0; j < data.size(); j++) {
            tableWidget->setItem(row_index, j, new QTableWidgetItem(data[j]));
        }
    }
}


