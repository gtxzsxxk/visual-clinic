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
    /* 设置窗体外观属性 */
    setWindowIcon(QIcon("resources/logo128.png"));
    auto *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(16);
    shadow->setOffset(0);
    shadow->setColor(QColor(0, 0, 0, 100));
    ui->widget->setGraphicsEffect(shadow);

    /* 连接槽函数 */
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(quit()));
    connect(ui->buttonMinimize, SIGNAL(clicked()), this, SLOT(minimize()));
    connect(ui->buttonMiddle, SIGNAL(clicked()), this, SLOT(normal_maximum()));
    connect(ui->tableWidget->horizontalHeader(),
            SIGNAL(sectionClicked(int)), this, SLOT(onTableHeaderSelected(int)));
    connect(ui->tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(tabSelected()));

    /* 安装事件过滤器 */
    APP_BTN_FILTER_INSTALL(attr);
    APP_BTN_FILTER_INSTALL(avg);
    APP_BTN_FILTER_INSTALL(scatter);
    APP_BTN_FILTER_INSTALL(relate);
    APP_BTN_FILTER_INSTALL(PCA);
    APP_BTN_FILTER_INSTALL(means);
    APP_BTN_FILTER_INSTALL(import);

    /* 设置表格属性 */
    ui->fileFrame->setVisible(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);

    /* 设置表格数据验证器 */
    TableValidator::setTableWidget(ui->tableWidget);
    tableValidator = TableValidator::getInstance();
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
    QMessageBox::information(this, "In Progress", "归因分析尚未完成");
}

void Index::goAvgAndVari() {
    /* 检查是否选中带有离散数据的列并且将其转化为0和1 */
    auto parsed_data = tableValidator->parseDiscreteColumn(selected_column);
    auto avgDialog = new AvgDialog(this, std::move(std::get<0>(parsed_data)),
                                   ui->tableWidget->horizontalHeaderItem(selected_column)->text(),
                                   std::get<1>(parsed_data), std::get<2>(parsed_data));
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
    if(tableValidator->selectingInfoAndDiscreteColumns()){
        QMessageBox::warning(this,"错误","不能选择前两列数据作为参数");
        return;
    }
    auto meansdial = new MeansDialog(this, ui->tableWidget);
    meansdial->setModal(true);
    meansdial->show();
}

void Index::goScatter() {
    if(tableValidator->selectingInfoAndDiscreteColumns()){
        QMessageBox::warning(this,"错误","不能选择前两列数据作为参数");
        return;
    }
    auto scatterdial = new ScatterDialog(this, ui->tableWidget);
    scatterdial->setModal(true);
    scatterdial->show();
}

void Index::goRelate() {
    if(tableValidator->selectingInfoAndDiscreteColumns()){
        QMessageBox::warning(this,"错误","不能选择前两列数据作为参数");
        return;
    }
    auto relatedial = new RelateDialog(this, ui->tableWidget);
    relatedial->setModal(true);
    relatedial->show();
}

void Index::goPCA() {
    if(tableValidator->selectingInfoAndDiscreteColumns()){
        QMessageBox::warning(this,"错误","不能选择前两列数据作为参数");
        return;
    }
    auto pcadial = new PCADialog(this, ui->tableWidget);
    pcadial->setModal(true);
    pcadial->show();
}

void Index::titleBarAdd(const QString &path) {
    /* 将layout向下转换 */
    auto *box = dynamic_cast<QHBoxLayout *>(ui->titleBarFrame->layout());
    if (box != nullptr) {
        /* 先构造变量 */
        auto *item = new FileTab(ui->titleBarFrame, ui->tableWidget, path);
        if (FileTab::tab_total_size > ui->titleBarFrame->geometry().width() - 150) {
            /* 如果titlebar的空间已经满了后，就删除这个变量，调用其析构函数，释放资源 */
            QMessageBox::warning(this, "错误", "无法打开更多的文件");
            file_full = true;
            delete item;
        } else {
            /* 删掉spacer，加入item，再加入spacer，确保外观合理 */
            box->removeItem(ui->titleBarSpacer);
            connect(item, SIGNAL(tabClosed(int)), this, SLOT(onTabClosed(int)));
            box->addWidget(item);
            box->addItem(ui->titleBarSpacer);
            /* 调用新增项目的select函数，选中该filetab */
            item->select();
        }
    }
}

void Index::tabSelected() {
    /* 槽函数，当表格中的列被选中时，自动根据规则使能或重置按钮 */
    bool selectingTwoColumns = tableValidator->selectingTwoColumns();
    if (selectingTwoColumns) {
        APP_BTN_SET_ENABLE(scatter, true);
    } else {
        APP_BTN_SET_ENABLE(scatter, false);
    }
    auto entire_res = tableValidator->selectingEntireColumn();
    selected_column = std::get<1>(entire_res);
    if (std::get<0>(entire_res)) {
        APP_BTN_SET_ENABLE(avg, true);
    } else {
        APP_BTN_SET_ENABLE(avg, false);
    }
    auto multiple_res = tableValidator->selectingMultipleColumns();
    column_selected_num = std::get<1>(multiple_res);
    if (std::get<0>(multiple_res)) {
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
}

void Index::onTabClosed(int tabIndex) {
    /* 该槽接收FileTab的信号，用于处理与UI界面有关的代码 */
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
