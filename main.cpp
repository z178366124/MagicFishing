#include <QWidget>
#include <QPainter>
#include <QScreen>
#include <QInputDialog>
#include <QDialog>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCloseEvent>
#include <QStyle>
#include <QMouseEvent>

#include "Tray.hpp"
#include "WorkTimeWindow.hpp"
#include "TimelineWidget.hpp"
#include "SettingWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Tray tray;
    tray.initTrayIcon();

    SettingWidget* settingWidget = SettingWidget::getInstance();
    QObject::connect(tray.settingsAction, &QAction::triggered, settingWidget, &SettingWidget::openWindow);
    
    // 创建主对话框
    QDialog mainDialog;
    mainDialog.setWindowTitle("工资计算器设置");
    QVBoxLayout* mainLayout = new QVBoxLayout(&mainDialog);

    // 创建日薪输入框
    QDoubleSpinBox* salarySpinBox = new QDoubleSpinBox(&mainDialog);
    salarySpinBox->setRange(0, 100000);
    salarySpinBox->setValue(1000.0);
    salarySpinBox->setDecimals(2);
    salarySpinBox->setPrefix("¥");
    mainLayout->addWidget(new QLabel("请输入您的日薪:"));
    mainLayout->addWidget(salarySpinBox);

    // 创建工作时长输入框
    QSpinBox* hoursSpinBox = new QSpinBox(&mainDialog);
    hoursSpinBox->setRange(1, 24);
    hoursSpinBox->setValue(8);
    hoursSpinBox->setSuffix(" 小时");
    mainLayout->addWidget(new QLabel("请输入每天工作时长:"));
    mainLayout->addWidget(hoursSpinBox);

    // 创建时间选择框
    QTimeEdit* timeEdit = new QTimeEdit(&mainDialog);
    timeEdit->setTime(QTime(9, 0));  // 默认早上9点
    mainLayout->addWidget(new QLabel("请选择上班时间:"));
    mainLayout->addWidget(timeEdit);

    // 添加确定按钮
    QPushButton* okButton = new QPushButton("确定", &mainDialog);
    mainLayout->addWidget(okButton);

    double salary = 0;
    int workHours = 0;
    QTime startTime;

    QObject::connect(okButton, &QPushButton::clicked, [&]() {
        salary = salarySpinBox->value();
        workHours = hoursSpinBox->value();
        startTime = timeEdit->time();
        mainDialog.accept();
    });

    if (mainDialog.exec() != QDialog::Accepted) return 1;

    BasicInfo basicInfo(salary, workHours, startTime);
        
    WatermarkWindow watermark(salary, workHours, startTime);

    QObject::connect(tray.showAction, &QAction::triggered, &watermark, &QWidget::show);
    QObject::connect(tray.hideAction, &QAction::triggered, &watermark, &QWidget::hide);
    QObject::connect(tray.positionAction, &QAction::triggered, &watermark, &WatermarkWindow::toggleDraggable);
    QObject::connect(settingWidget, &SettingWidget::settingsChanged, &watermark, &WatermarkWindow::updateDisplay);


    // 创建时间轴控件
    // TimelineWidget* timeline = new TimelineWidget(&mainDialog);
    TimelineWidget timeline(nullptr, &basicInfo);
    QObject::connect(tray.showAction, &QAction::triggered, &timeline, &QWidget::show);
    QObject::connect(tray.hideAction, &QAction::triggered, &timeline, &QWidget::hide);
    QObject::connect(tray.positionAction, &QAction::triggered, &timeline, &TimelineWidget::toggleDraggable);

    timeline.setWindowTitle("时间轴");
    timeline.resize(400, 100);
    timeline.show();

    

    
    // 添加一些示例颜色段
    // timeline.addSegment(0.3, Qt::red);
    // timeline.addSegment(0.3, Qt::green); 
    // timeline.addSegment(0.4, Qt::blue);
    return app.exec();
}
