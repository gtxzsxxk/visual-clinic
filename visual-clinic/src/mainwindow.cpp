#include "../include/mainwindow.h"
#include "../include/avgdialog.h"
#include "../ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
        : XMainWindow(parent), ui(new Ui::MainWindow), fileOpened(false) {
    ui->setupUi(this);
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(quit()));
    connect(ui->buttonMinimize, SIGNAL(clicked()), this, SLOT(minimize()));
    connect(ui->tableWidget->horizontalHeader(),
            SIGNAL(sectionClicked(int)), this, SLOT(onTableHeaderSelected(int)));
    APP_BTN_FILTER_INSTALL(attr);
    APP_BTN_FILTER_INSTALL(avg);
    APP_BTN_FILTER_INSTALL(scatter);
    APP_BTN_FILTER_INSTALL(relate);
    APP_BTN_FILTER_INSTALL(PCA);
    APP_BTN_FILTER_INSTALL(means);
    APP_BTN_FILTER_INSTALL(import);
    ui->fileFrame->setVisible(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::minimize() {
    this->showMinimized();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
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

void MainWindow::goAttributionAnalysis() {
    QMessageBox::information(this, "Debug", "归因分析");
}

void MainWindow::goAvgAndVari() {
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
                                   discrete_flag,discrete_num);
    avgDialog->setModal(true);
    avgDialog->show();
}

void MainWindow::importCSV() {
    auto path = QFileDialog::getOpenFileName(this, "选择病例文件", ".", "*.csv");
    if (path.isEmpty()) {
        return;
    }
    titleBarAdd(path);
}

void MainWindow::goMeans() {
    QMessageBox::information(this, "Debug", "聚类分析");
}

void MainWindow::goScatter() {
    QMessageBox::information(this, "Debug", "散点分析");
}

void MainWindow::goRelate() {
    QMessageBox::information(this, "Debug", "相关性分析");
}

void MainWindow::goPCA() {
    QMessageBox::information(this, "Debug", "主成分分析");
}

void MainWindow::titleBarAdd(const QString &path) {
    auto *box = dynamic_cast<QHBoxLayout *>(ui->titleBarFrame->layout());
    if (box != nullptr) {
        box->removeItem(ui->titleBarSpacer);
        auto *item = new FileTab(ui->titleBarFrame, ui->tableWidget, path);
        connect(item, SIGNAL(onTabClosed(int)), this, SLOT(onTabClosed(int)));
        box->addWidget(item);
        box->addItem(ui->titleBarSpacer);
        item->select();
    }
}

void MainWindow::tabSelected(int tabIndex) {

}

void MainWindow::onTabClosed(int tabIndex) {
    auto *item = FileTab::fileTabs[tabIndex];
    auto *box = dynamic_cast<QHBoxLayout *>(ui->titleBarFrame->layout());
    if (box != nullptr) {
        item->hide();
        box->removeWidget(item);
    }
    FileTab::fileTabs.erase(FileTab::fileTabs.begin() + tabIndex);
    if (!FileTab::fileTabs.empty()) {
        FileTab::fileTabs[0]->select();
    }
}

void MainWindow::onTableHeaderSelected(int index) {
    if (!FileTab::fileTabs.empty()) {
        APP_BTN_SET_ENABLE(avg, true);
        selected_column = index;
    }
}

