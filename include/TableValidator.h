//
// Created by hanyuan on 2023/9/17.
//

#ifndef VISUAL_CLINIC_TABLEVALIDATOR_H
#define VISUAL_CLINIC_TABLEVALIDATOR_H

#include <QTableWidget>
#include <vector>
#include <tuple>

class TableValidator {
    static TableValidator instance;
    QTableWidget *tableWidget = nullptr;

    TableValidator() {}

public:

    static void setTableWidget(QTableWidget *widget);

    [[nodiscard]] static TableValidator *getInstance();

    [[nodiscard]] bool selectingTwoColumns() const;

    [[nodiscard]] bool selectingInfoAndDiscreteColumns() const;

    [[nodiscard]] std::pair<bool, int> selectingEntireColumn() const;

    [[nodiscard]] std::pair<bool, int> selectingMultipleColumns() const;

    [[nodiscard]] std::tuple<std::vector<float>, bool, int> parseDiscreteColumn(int selected_column) const;
};


#endif //VISUAL_CLINIC_TABLEVALIDATOR_H
