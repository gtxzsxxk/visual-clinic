//
// Created by hanyuan on 2023/9/5.
//

#ifndef VISUAL_CLINIC_XMAINWINDOW_H
#define VISUAL_CLINIC_XMAINWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QGraphicsDropShadowEffect>

class XMainWindow : public QDialog {
public:
    explicit XMainWindow(QWidget *parent = nullptr): QDialog(parent) {
        /* 设置无边框窗体 */
        setAutoFillBackground(false);
        setWindowFlag(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_NoSystemBackground,true);
        setAttribute(Qt::WA_TranslucentBackground,true);
    }

    void mousePressEvent(QMouseEvent *event) override {
        /* 实现拖动 */
        if (event->button() == Qt::LeftButton) {
            move_start_point = frameGeometry().topLeft() - event->globalPosition();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        /* 实现拖动 */
        QPointF p = event->globalPosition() + move_start_point;
        this->move(QPoint(static_cast<int>(p.x()), static_cast<int>(p.y())));
    }

private:
    QPointF move_start_point;
};

#endif //VISUAL_CLINIC_XMAINWINDOW_H
