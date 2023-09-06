//
// Created by hanyuan on 2023/9/5.
//

#ifndef VISUAL_CLINIC_XMAINWINDOW_H
#define VISUAL_CLINIC_XMAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QGraphicsDropShadowEffect>

class XMainWindow : public QMainWindow {
public:
    explicit XMainWindow(QWidget *parent = nullptr): QMainWindow(parent) {
        setWindowFlag(Qt::FramelessWindowHint);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            move_start_point = frameGeometry().topLeft() - event->globalPosition();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        QPointF p = event->globalPosition() + move_start_point;
        this->move(QPoint(static_cast<int>(p.x()), static_cast<int>(p.y())));
    }

private:
    QPointF move_start_point;
};

#endif //VISUAL_CLINIC_XMAINWINDOW_H
