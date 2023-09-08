//
// Created by hanyuan on 2023/9/7.
//

#include "../include/XGradientFrame.h"

#include <QPen>
#include <QPainter>

XGradientFrame::XGradientFrame(QWidget *parent) : QFrame(parent) {

}

void XGradientFrame::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置渐变色
    QLinearGradient linear(QPointF(80, 80), QPointF(150, 150));
    linear.setColorAt(0, Qt::black);
    linear.setColorAt(1, Qt::white);

    // 设置显示模式
    linear.setSpread(QGradient::PadSpread);

    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 2));
    // 设置画刷填充
    painter.setBrush(linear);

    // 绘制椭圆
    painter.drawRect(QRect(10, 0, 20, this->height()));
}
