#include "../include/mainwindow.h"
#include "../include/FileTab.h"
#include "../ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <vector>

#define FILTER_INSTALL(tag) do{ \
     ui->button_##tag->installEventFilter(this);\
ui->label_##tag->installEventFilter(this);\
ui->pushButton_##tag->installEventFilter(this);\
}while(0);

#define TAG_HIT(tag) ((watched == ui->button_##tag || watched == ui->label_##tag \
|| watched == ui->pushButton_##tag) && event->type() == QEvent::MouseButtonRelease\
&& ui->button_##tag->isEnabled())

MainWindow::MainWindow(QWidget *parent)
        : XMainWindow(parent), ui(new Ui::MainWindow), fileOpened(false) {
    ui->setupUi(this);
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(quit()));
    connect(ui->buttonMinimize, SIGNAL(clicked()), this, SLOT(minimize()));
    FILTER_INSTALL(attr);
    FILTER_INSTALL(avg);
    FILTER_INSTALL(scatter);
    FILTER_INSTALL(relate);
    FILTER_INSTALL(PCA);
    FILTER_INSTALL(means);
    FILTER_INSTALL(import);
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
    if TAG_HIT(attr) {
        goAttributionAnalysis();
        return true;
    } else if TAG_HIT(avg) {
        goAvgAndVari();
        return true;
    } else if TAG_HIT(import) {
        importCSV();
        return true;
    } else if TAG_HIT(means) {
        goMeans();
        return true;
    } else if TAG_HIT(scatter) {
        goScatter();
        return true;
    } else if TAG_HIT(relate) {
        goRelate();
        return true;
    } else if TAG_HIT(PCA) {
        goPCA();
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void MainWindow::goAttributionAnalysis() {
    QMessageBox::information(this, "Debug", "归因分析");
}

void MainWindow::goAvgAndVari() {
    QMessageBox::information(this, "Debug", "均值与方差分析");
}

void MainWindow::importCSV() {
    currentFilePath = QFileDialog::getOpenFileName(this, "选择病例文件", ".", "*.csv");
    QFile f(currentFilePath);

    if (f.open(QIODevice::ReadOnly)) {
        QTextStream stream(&f);
        std::vector<QString> lines;
        while (!stream.atEnd()) {
            lines.push_back(stream.readLine());
        }
        if (lines.size() < 2) {
            QMessageBox::warning(this, "错误", "文件是空的，或者文件格式错误");
        }
        auto header_src = lines[0];
        auto headers = header_src.split(',');
        ui->tableWidget->setColumnCount(headers.size());
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        QFileInfo info(currentFilePath);
        auto real_name = info.fileName();
        titleBarAdd(real_name);
        fileOpened = true;
    } else {
        QMessageBox::warning(this, "错误", "无法打开文件：" + currentFilePath);
    }
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

void MainWindow::titleBarAdd(const QString &name) {
    auto *box = dynamic_cast<QHBoxLayout *>(ui->titleBarFrame->layout());
    if (box != nullptr) {
        box->removeItem(ui->titleBarSpacer);
        auto *item = new FileTab(this, name);
        box->addWidget(item);
        box->addItem(ui->titleBarSpacer);
        item->select();
    }
}

