//
// Created by hanyuan on 2023/9/7.
//

#ifndef VISUAL_CLINIC_XGRADIENTFRAME_H
#define VISUAL_CLINIC_XGRADIENTFRAME_H

#include <QFrame>
#include <QColor>
#include <vector>
#include <functional>

class XGradientFrame : public QFrame {
Q_OBJECT

public:
    explicit XGradientFrame(QWidget *parent);
    void setPoints(std::function<int (float)> const &f,float max_value,float min_value);
private:
    std::vector<std::pair<float,QColor>> color_maps;
    void paintEvent(QPaintEvent *) override;
};


#endif //VISUAL_CLINIC_XGRADIENTFRAME_H
