#include "../include/index.h"
#include "../include/avgdialog.h"
#include "../include/scatterdialog.h"
#include "../include/relatedialog.h"
#include "../include/pcadialog.h"
#include "../include/meansdialog.h"
#include "../ui_index.h"

#include <QMessageBox>
#include <QFileDialog>
#include <vector>
#include <map>

#define APP_BTN_FILTER_INSTALL(tag) do{ \
     ui->button_##tag->installEventFilter(this);\
ui->label_##tag->installEventFilter(this);\
ui->pushButton_##tag->installEventFilter(this);\
}while(0);

#define APP_BTN_TAG_HIT(tag) ((watched == ui->button_##tag || watched == ui->label_##tag \
|| watched == ui->pushButton_##tag) && event->type() == QEvent::MouseButtonRelease\
&& ui->button_##tag->isEnabled())

#define APP_BTN_SET_ENABLE(tag, value) do { \
            ui->button_##tag->setEnabled(value);                               \
            ui->pushButton_##tag->setEnabled(value);                               \
} while (0);

Index::Index(QWidget *parent) :
        XMainWindow(parent),
        ui(new Ui::Index) {
    ui->setupUi(this);
    setWindowIcon(QIcon("resources/logo128.png"));
    auto *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(16);
    shadow->setOffset(0);
    shadow->setColor(QColor(0, 0, 0, 100));
    ui->widget->setGraphicsEffect(shadow);
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(quit()));
    connect(ui->buttonMinimize, SIGNAL(clicked()), this, SLOT(minimize()));
    connect(ui->buttonMiddle, SIGNAL(clicked()), this, SLOT(normal_maximum()));
    connect(ui->tableWidget->horizontalHeader(),
            SIGNAL(sectionClicked(int)), this, SLOT(onTableHeaderSelected(int)));
    connect(ui->tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(tabSelected()));
    APP_BTN_FILTER_INSTALL(attr);
    APP_BTN_FILTER_INSTALL(avg);
    APP_BTN_FILTER_INSTALL(scatter);
    APP_BTN_FILTER_INSTALL(relate);
    APP_BTN_FILTER_INSTALL(PCA);
    APP_BTN_FILTER_INSTALL(means);
    APP_BTN_FILTER_INSTALL(import);
    ui->fileFrame->setVisible(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);
}

Index::~Index() {
    delete ui;
}

void Index::quit() {
    qApp->quit();
}

void Index::minimize() {
    this->showMinimized();
}

bool Index::eventFilter(QObject *watched, QEvent *event) {
    if APP_BTN_TAG_HIT(attr) {
        goAttributionAnalysis();
        return true;
    } else if APP_BTN_TAG_HIT(avg) {
        goAvgAndVari();
        return true;
    } else if APP_BTN_TAG_HIT(import) {
        importCSV();
        return true;
    } else if APP_BTN_TAG_HIT(means) {
        goMeans();
        return true;
    } else if APP_BTN_TAG_HIT(scatter) {
        goScatter();
        return true;
    } else if APP_BTN_TAG_HIT(relate) {
        goRelate();
        return true;
    } else if APP_BTN_TAG_HIT(PCA) {
        goPCA();
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void Index::goAttributionAnalysis() {
    QMessageBox::information(this, "Debug", "归因分析");
}

void Index::goAvgAndVari() {
    std::vector<float> data;
    bool discrete_flag = false;
    int discrete_num = 2;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        auto *item = ui->tableWidget->item(i, selected_column);
        if (item->text()[0] >= 'A') {
            discrete_flag = true;
            break;
        } else {
            data.push_back(item->text().toDouble());
        }
    }
    if (discrete_flag) {
        std::map<QString, int> categories;
        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            auto *item = ui->tableWidget->item(i, selected_column);
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
    auto avgDialog = new AvgDialog(this, std::move(data),
                                   ui->tableWidget->horizontalHeaderItem(selected_column)->text(),
                                   discrete_flag, discrete_num);
    avgDialog->setModal(true);
    avgDialog->show();
}

void Index::importCSV() {
    if (!file_full) {
        auto path = QFileDialog::getOpenFileName(this, "选择病例文件", ".", "*.csv");
        if (path.isEmpty()) {
            return;
        }
        titleBarAdd(path);
    } else {
        QMessageBox::warning(this, "错误", "无法打开更多的文件");
    }
}

void Index::goMeans() {
    auto meansdial = new MeansDialog(this, ui->tableWidget);
    meansdial->setModal(true);
    meansdial->show();
}

void Index::goScatter() {
    auto scatterdial = new ScatterDialog(this, ui->tableWidget);
    scatterdial->setModal(true);
    scatterdial->show();
}

void Index::goRelate() {
    /* TODO: B and M should not be allowed to go relate */
    auto relatedial = new RelateDialog(this, ui->tableWidget);
    relatedial->setModal(true);
    relatedial->show();
}

void Index::goPCA() {
    /* TODO: B and M should not be allowed to go PCA，统一命名风格 */
    auto pcadial = new PCADialog(this, ui->tableWidget);
    pcadial->setModal(true);
    pcadial->show();
}

void Index::titleBarAdd(const QString &path) {
    auto *box = dynamic_cast<QHBoxLayout *>(ui->titleBarFrame->layout());
    if (box != nullptr) {
        auto *item = new FileTab(ui->titleBarFrame, ui->tableWidget, path);
        if (FileTab::tab_total_size > ui->titleBarFrame->geometry().width() - 150) {
            QMessageBox::warning(this, "错误", "无法打开更多的文件");
            file_full = true;
            delete item;
        } else {
            box->removeItem(ui->titleBarSpacer);
            connect(item, SIGNAL(tabClosed(int)), this, SLOT(onTabClosed(int)));
            box->addWidget(item);
            box->addItem(ui->titleBarSpacer);
            item->select();
        }
    }
}

void Index::tabSelected() {
    if (isSelectingTwoColumns()) {
        APP_BTN_SET_ENABLE(scatter, true);
    } else {
        APP_BTN_SET_ENABLE(scatter, false);
    }
    if (isSelectingEntireColumn()) {
        APP_BTN_SET_ENABLE(avg, true);
    } else {
        APP_BTN_SET_ENABLE(avg, false);
    }
    if (isSelectingMultipleColumns()) {
        if (column_selected_num <= 10) {
            APP_BTN_SET_ENABLE(relate, true);
            if (column_selected_num > 2) {
                APP_BTN_SET_ENABLE(PCA, true);
                APP_BTN_SET_ENABLE(means, true);
            } else {
                APP_BTN_SET_ENABLE(PCA, false);
                APP_BTN_SET_ENABLE(means, false);
            }
        } else {
            APP_BTN_SET_ENABLE(relate, false);
            APP_BTN_SET_ENABLE(PCA, false);
            APP_BTN_SET_ENABLE(means, false);
        }
    } else {
        APP_BTN_SET_ENABLE(relate, false);
        APP_BTN_SET_ENABLE(PCA, false);
        APP_BTN_SET_ENABLE(means, false);
    }
}

void Index::onTabClosed(int tabIndex) {
    auto *item = FileTab::fileTabs[tabIndex];
    auto *box = dynamic_cast<QHBoxLayout *>(ui->titleBarFrame->layout());
    if (box != nullptr) {
        item->hide();
        box->removeWidget(item);
        delete item;
    }
    if (!FileTab::fileTabs.empty()) {
        FileTab::fileTabs[0]->select();
    }
    file_full = false;
}

void Index::onTableHeaderSelected(int index) {
    if (!FileTab::fileTabs.empty()) {
        APP_BTN_SET_ENABLE(avg, true);
        selected_column = index;
    }
}

bool Index::isSelectingTwoColumns() {
    QList<QTableWidgetItem *> selected_items = ui->tableWidget->selectedItems();
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

bool Index::isSelectingEntireColumn() {
    auto selected_items = ui->tableWidget->selectedItems();
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
        return false;
    }
    if (ui->tableWidget->rowCount() == column_cnt) {
        selected_column = col;
        return true;
    }
    return false;
}

bool Index::isSelectingMultipleColumns() {
    QList<QTableWidgetItem *> selected_items = ui->tableWidget->selectedItems();
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
        return false;
    }
    int counts = selected_items.size() / column_cnt;
    if (counts < 2) {
        return false;
    }
    for (const auto &it: row_counter) {
        if (it.second != counts) {
            return false;
        }
    }
    column_selected_num = counts;
    return true;
}

void Index::normal_maximum() {
    if (maximum_flag) {
        showNormal();
        setMinimumSize(900, 620);
        setMaximumSize(900, 620);
        QIcon icon;
        icon.addFile(QString::fromUtf8("resources/win_normal.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->buttonMiddle->setIcon(icon);
        maximum_flag = false;
    } else {
        setMaximumSize(10000, 10000);
        showMaximized();
        QIcon icon;
        icon.addFile(QString::fromUtf8("resources/win_full.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->buttonMiddle->setIcon(icon);
        maximum_flag = true;
    }
}
