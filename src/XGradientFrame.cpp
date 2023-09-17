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

    /* 设置渐变色 */
    QLinearGradient linear(10, this->height(), 10, 0);
    for (const auto &it: color_maps) {
        linear.setColorAt(it.first, it.second);
    }

    /* 设置显示模式 */
    linear.setSpread(QGradient::PadSpread);

    /* 设置画笔颜色、宽度 */
    painter.setPen(QPen(QColor(255, 255, 255), 2));
    /* 设置画刷填充 */
    painter.setBrush(linear);

    /* 绘制标签 */
    painter.drawRect(QRect(0, 0, 20, this->height()));
    painter.setPen(QPen(QColor(0, 0, 0), 2));
    float step = this->height() / 6;
    float y = this->height();
    float value = min_value;
    float value_step = (max_value - min_value) / 6;
    for (int i = 0; i < 6; i++) {
        painter.drawText(20, static_cast<int>(y), "- " + QString::number(value, 'f', 2));
        y -= step;
        value += value_step;
    }
    y += step;
    value -= value_step;
    y -= step * 0.85;
    value += value_step * 0.85;
    painter.drawText(20, static_cast<int>(y), "- " + QString::number(value, 'g', 4));
}

void XGradientFrame::setPoints(const std::function<int(float)> &f, float max_value, float min_value) {
    /* 设置点集 */
    float k = 1 / (max_value - min_value);
    float step = (max_value - min_value) / 10;
    float x = min_value;
    this->max_value = max_value;
    this->min_value = min_value;
    color_maps.clear();
    for (int i = 0; i < 10; i++) {
        float x_1 = (x - min_value) * k;
        auto color = f(x);
        QColor qColor;
        if (color < 255) {
            qColor = QColor(color, 0, 0);
        } else {
            color -= 255;
            qColor = QColor(255, color, color);
        }
        color_maps.emplace_back(x_1, qColor);
        x += step;
    }
    update();
    repaint();
}
