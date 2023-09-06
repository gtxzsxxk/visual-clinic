//
// Created by hanyuan on 2023/9/6.
//

#include "../include/FileTab.h"

#include <QEvent>

std::vector<FileTab*> FileTab::fileTabs;

FileTab::FileTab(QWidget *parent, const QString &filename) :
        QFrame(parent), filename(filename), selected(false) {
    int width = filename.length() * 10;
    int container_width = width + 60;
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
    fileTabs.push_back(this);
    installEventFilter(this);
    label->installEventFilter(this);
    buttonCloseCurrentFile->installEventFilter(this);
}

void FileTab::select() {
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
}

void FileTab::unselect() {
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
    }
    return QObject::eventFilter(watched, event);
}


