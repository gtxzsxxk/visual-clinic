//
// Created by hanyuan on 2023/9/17.
//

#ifndef VISUAL_CLINIC_TABLEVALIDATOR_H
#define VISUAL_CLINIC_TABLEVALIDATOR_H

#include <QTableWidget>
#include <vector>
#include <tuple>

class TableValidator {
    /* 单例 */
    static TableValidator instance;
    /* 对象的表格控件指针 */
    QTableWidget *tableWidget = nullptr;

    TableValidator() {}

public:

    /* 设置表格控件指针 */
    static void setTableWidget(QTableWidget *widget);

    [[nodiscard]] static TableValidator *getInstance();

    /* 是否选中了两列 */
    [[nodiscard]] bool selectingTwoColumns() const;

    /* 是否选中了编号列和离散数据的一列 */
    [[nodiscard]] bool selectingInfoAndDiscreteColumns() const;

    /* 是否选中了一整列 */
    [[nodiscard]] std::pair<bool, int> selectingEntireColumn() const;

    /* 是否选中了多列 */
    [[nodiscard]] std::pair<bool, int> selectingMultipleColumns() const;

    /* 将离散的列parse成0和1 */
    [[nodiscard]] std::tuple<std::vector<float>, bool, int> parseDiscreteColumn(int selected_column) const;
};


#endif //VISUAL_CLINIC_TABLEVALIDATOR_H
