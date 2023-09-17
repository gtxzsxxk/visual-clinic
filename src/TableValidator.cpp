//
// Created by hanyuan on 2023/9/17.
//

#include "../include/TableValidator.h"

#include <cassert>
#include <set>
#include <map>

TableValidator TableValidator::instance;

TableValidator *TableValidator::getInstance() {
    return &instance;
}

bool TableValidator::selectingTwoColumns() const {
    assert(tableWidget != nullptr);

    QList<QTableWidgetItem *> selected_items = tableWidget->selectedItems();
    int col = -1;
    int column_cnt = 1;
    std::map<int, int> row_counter;
    for (const auto &it: selected_items) {
        int r = it->row();
        if (!row_counter.count(r)) {
            row_counter[r] = 1;
        } else {
            row_counter[r]++;
        }
        if (col == -1) {
            col = it->column();
            continue;
        }
        if (it->column() == col) {
            column_cnt++;
        }
    }
    if (selected_items.size() != 2 * column_cnt) {
        return false;
    }
    for (const auto &it: row_counter) {
        if (it.second != 2) {
            return false;
        }
    }
    return true;
}

std::pair<bool, int> TableValidator::selectingEntireColumn() const {
    assert(tableWidget != nullptr);

    auto selected_items = tableWidget->selectedItems();
    int col = -1;
    int column_cnt = 1;
    for (const auto &it: selected_items) {
        if (col == -1) {
            col = it->column();
            continue;
        }
        if (it->column() == col) {
            column_cnt++;
        }
    }
    if (selected_items.size() != column_cnt) {
        return std::make_pair(false, -1);
    }
    if (tableWidget->rowCount() == column_cnt) {
        return std::make_pair(true, col);
    }
    return std::make_pair(false, -1);
}

std::pair<bool, int> TableValidator::selectingMultipleColumns() const {
    assert(tableWidget != nullptr);

    QList<QTableWidgetItem *> selected_items = tableWidget->selectedItems();
    int col = -1;
    int column_cnt = 1;
    std::map<int, int> row_counter;
    for (const auto &it: selected_items) {
        int r = it->row();
        if (!row_counter.count(r)) {
            row_counter[r] = 1;
        } else {
            row_counter[r]++;
        }
        if (col == -1) {
            col = it->column();
            continue;
        }
        if (it->column() == col) {
            column_cnt++;
        }
    }
    if (selected_items.size() % column_cnt) {
        return std::make_pair(false, -1);
    }
    int counts = selected_items.size() / column_cnt;
    if (counts < 2) {
        return std::make_pair(false, -1);
    }
    for (const auto &it: row_counter) {
        if (it.second != counts) {
            return std::make_pair(false, -1);
        }
    }
    return std::make_pair(true, counts);
}

void TableValidator::setTableWidget(QTableWidget *widget) {
    instance.tableWidget = widget;
}

std::tuple<std::vector<float>, bool, int> TableValidator::parseDiscreteColumn(int selected_column) const {
    std::vector<float> data;
    bool discrete_flag = false;
    int discrete_num = 2;
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        auto *item = tableWidget->item(i, selected_column);
        if (item->text()[0] >= 'A') {
            discrete_flag = true;
            break;
        } else {
            data.push_back(item->text().toDouble());
        }
    }
    if (discrete_flag) {
        std::map<QString, int> categories;
        for (int i = 0; i < tableWidget->rowCount(); i++) {
            auto *item = tableWidget->item(i, selected_column);
            auto key = item->text();
            if (categories.count(key)) {
                categories[key]++;
            } else {
                categories[key] = 1;
            }
        }
        discrete_num = categories.size();
        int cnt = 0;
        data.clear();
        for (const auto &it: categories) {
            for (int k = 0; k < it.second; k++) {
                data.push_back((float) cnt);
            }
            cnt++;
        }
    }
    return std::make_tuple(data, discrete_flag, discrete_num);
}

bool TableValidator::selectingInfoAndDiscreteColumns() const {
    QList<QTableWidgetItem *> selected_items = tableWidget->selectedItems();
    for (const auto &it: selected_items) {
        if (it->column() == 0 || it->column() == 1) {
            return true;
        }
    }
    return false;
}
