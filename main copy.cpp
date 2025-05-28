#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QScreen>
#include <QInputDialog>
#include <QTime>
#include <QDialog>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QIcon>
#include <QStyle>
#include <QMouseEvent>

// 基本信息类，用于存储和管理工资计算器的基本配置信息
class BasicInfo {
protected:
    double salary;      // 日薪金额
    int workHours;      // 每天工作时长(小时)
    QTime startTime;    // 上班开始时间
    QTimer* timer;      // 定时器,用于定期更新界面
    QSystemTrayIcon* trayIcon;  // 系统托盘图标
    bool isDraggable;   // 是否可拖动窗口
    QPoint dragPosition;  // 记录拖动时的鼠标位置

public:
    BasicInfo(double salary = 0, int workHours = 8, QTime startTime = QTime(9,0)) {
        this->salary = salary;
        this->workHours = workHours; 
        this->startTime = startTime;
        this->isDraggable = false;
        this->timer = nullptr;
        this->trayIcon = nullptr;
    }

    // getter方法
    double getSalary() const { return salary; }
    int getWorkHours() const { return workHours; }
    QTime getStartTime() const { return startTime; }
    bool getIsDraggable() const { return isDraggable; }
    QPoint getDragPosition() const { return dragPosition; }

    // setter方法
    void setSalary(double value) { salary = value; }
    void setWorkHours(int value) { workHours = value; }
    void setStartTime(const QTime& value) { startTime = value; }
    void setIsDraggable(bool value) { isDraggable = value; }
    void setDragPosition(const QPoint& value) { dragPosition = value; }
};



class WatermarkWindow : public QWidget {
public:
    WatermarkWindow(double salary, int workHours, QTime startTime) {
        this->salary = salary;
        this->workHours = workHours;
        this->startTime = startTime;
        this->isDraggable = false;  // 初始化拖动状态
        updateWindowFlags();  // 使用新函数设置窗口标志
        setAttribute(Qt::WA_TranslucentBackground);     // 背景透明
        setAttribute(Qt::WA_ShowWithoutActivating);     // 保持窗口在顶层

        // 设置全屏大小（多屏幕可遍历 QGuiApplication::screens()）
        // 获取主屏幕的几何信息(大小和位置)
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        // 将窗口大小设置为屏幕大小
        resize(screenGeometry.size());
        // 将窗口移动到屏幕左上角
        move(screenGeometry.topLeft());
        
        initTimer();  // 初始化并启动定时器
        initTrayIcon();  // 初始化系统托盘图标
        show();  // 显示窗口
    }

    ~WatermarkWindow() {
        if (trayIcon) {
            delete trayIcon;
        }
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        // 使用渐变色
        QLinearGradient gradient(0, 0, width(), height());
        gradient.setColorAt(0, QColor(255, 0, 255, 128));  // 紫色
        gradient.setColorAt(0.5, QColor(0, 255, 255, 128));  // 青色
        gradient.setColorAt(1, QColor(255, 255, 0, 128));  // 黄色
        painter.setPen(QPen(QBrush(gradient), 2));  // 使用渐变笔刷
        painter.setFont(QFont("Arial", 30, QFont::Bold));

        // 计算当前时间工作了多少秒
        QTime currentTime = QTime::currentTime();
        // int secondsWorked = QTime(18,0).secsTo(currentTime); // 从9点开始计算
        int secondsWorked = startTime.secsTo(currentTime);
        int totalSeconds = workHours * 3600;  // 总工作时长(秒)
        
        if(secondsWorked > 0 && secondsWorked < totalSeconds) {
            QFontMetrics fm(painter.font());
            int textHeight = fm.height();
            // 计算当前赚到的钱
            double earnedMoney = (salary / workHours / 3600.0) * secondsWorked;
            QString text = QString("今日已赚: ¥%1").arg(earnedMoney, 0, 'f', 2);
            
            // 绘制文本
            QRect textRect = rect();
            textRect.setBottom(height() / 2);  // 文本在上半部分
            painter.drawText(textRect, Qt::AlignCenter, text);
            
            // // 绘制进度条
            // int progressBarHeight = 20;  // 进度条高度
            // int progressBarWidth = width() * 0.15;  // 进度条宽度为窗口宽度的80%
            // int progressBarX = (width() - progressBarWidth) / 2;  // 居中
            // // int progressBarY = height() / 2 + 0;  // 在文本下方
            // int spacing = 0;
            // int progressBarY = (height() - textHeight) / 2 + textHeight + spacing;
            
            // // 绘制总进度条背景(绿色代表摸鱼)
            // painter.setPen(Qt::NoPen);
            // painter.setBrush(QColor(0, 255, 0, 128));  // 半透明绿色
            // painter.drawRect(progressBarX, progressBarY, progressBarWidth, progressBarHeight);
            
            // // 绘制已工作时间(红色)
            // double progress = (double)secondsWorked / totalSeconds;
            // painter.setBrush(QColor(255, 0, 0, 128));  // 半透明红色
            // painter.drawRect(progressBarX, progressBarY, progressBarWidth * progress, progressBarHeight);
            
        } else {
            painter.drawText(rect(), Qt::AlignCenter, "非工作时间");
        }
        // qDebug() << "secondsWorked:" << secondsWorked;
    }

    void closeEvent(QCloseEvent *event) override {
        if (trayIcon && trayIcon->isVisible()) {
            hide();
            event->ignore();
        } else {
            event->accept();
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (isDraggable && event->button() == Qt::LeftButton) {
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isDraggable && event->buttons() & Qt::LeftButton) {
            move(event->globalPosition().toPoint() - dragPosition);
            event->accept();
        }
    }

private:
    double salary;      // 日薪金额
    int workHours;      // 每天工作时长(小时)
    QTime startTime;    // 上班开始时间
    QTimer* timer;      // 定时器,用于定期更新界面
    QSystemTrayIcon* trayIcon;  // 系统托盘图标
    bool isDraggable;   // 是否可拖动窗口
    QPoint dragPosition;  // 记录拖动时的鼠标位置

    void updateWindowFlags() {
        Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool;
        if (!isDraggable) {
            flags |= Qt::WindowTransparentForInput;
        }
        #ifdef Q_OS_LINUX
        flags |= Qt::X11BypassWindowManagerHint;
        #endif
        setWindowFlags(flags);
    }

    void toggleDraggable() {
        isDraggable = !isDraggable;
        updateWindowFlags();
        show();  // 重新显示窗口以应用新的窗口标志
    }

    void initTimer() {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, static_cast<void (QWidget::*)()>(&QWidget::update));
        timer->start(1000); // 每秒更新一次
    }

    void initTrayIcon() {
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon(":/icon.ico"));
        if (!trayIcon->isSystemTrayAvailable()) {
            qDebug() << "系统托盘不可用";
            return;
        }

        if (!trayIcon->icon().isNull()) {
            qDebug() << "图标加载成功";
        } else {
            qDebug() << "图标加载失败";
            exit(1);
        }

        QMenu* trayMenu = new QMenu();
        QAction* showAction = new QAction("显示", this);
        QAction* hideAction = new QAction("隐藏", this);
        QAction* positionAction = new QAction("位置控制", this);  // 新增位置控制按钮
        QAction* quitAction = new QAction("退出", this);

        connect(showAction, &QAction::triggered, this, &QWidget::show);
        connect(hideAction, &QAction::triggered, this, &QWidget::hide);
        connect(positionAction, &QAction::triggered, this, &WatermarkWindow::toggleDraggable);  // 连接位置控制按钮
        connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

        trayMenu->addAction(showAction);
        trayMenu->addAction(hideAction);
        trayMenu->addAction(positionAction);  // 添加到菜单
        trayMenu->addSeparator();
        trayMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();

        connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::DoubleClick) {
                if (isVisible()) {
                    hide();
                } else {
                    show();
                }
            }
        });
    }
};

class TimelineWidget : public QWidget {
private:
    QVector<QPair<double, QColor>> segments; // 存储每个颜色段的百分比和颜色

public:
    TimelineWidget(QWidget *parent = nullptr, BasicInfo *basicInfo = nullptr) : QWidget(parent), m_basicInfo(basicInfo) {
        setMinimumHeight(30);

        this->m_basicInfo->setIsDraggable(false) ;  // 初始化拖动状态
        updateWindowFlags();  // 使用新函数设置窗口标志
        setAttribute(Qt::WA_TranslucentBackground);     // 背景透明
        setAttribute(Qt::WA_ShowWithoutActivating);     // 保持窗口在顶层
    }
    // TimelineWidget(BasicInfo *basicInfo = nullptr) : m_basicInfo(basicInfo) {
    //     setMinimumHeight(30);
    // }
    void updateWindowFlags() {
        Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool;
        if (!m_basicInfo->getIsDraggable()) {
            flags |= Qt::WindowTransparentForInput;
        }
        #ifdef Q_OS_LINUX
        flags |= Qt::X11BypassWindowManagerHint;
        #endif
        setWindowFlags(flags);
    }
    // 添加一个新的颜色段
    void addSegment(double percentage, const QColor& color) {
        segments.append(qMakePair(percentage, color));
        update();
    }

    // 清除所有颜色段
    void clearSegments() {
        segments.clear();
        update();
    }
    BasicInfo *m_basicInfo;

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制底色横线（灰色）
        int lineY = height() / 2;
        painter.setPen(QPen(Qt::gray, 2));
        painter.drawLine(0, lineY, width(), lineY);

        // 绘制颜色段
        double currentX = 0;
        for (const auto& segment : segments) {
            int segmentWidth = width() * segment.first;
            painter.setPen(QPen(segment.second, 4));
            painter.drawLine(currentX, lineY, currentX + segmentWidth, lineY);
            currentX += segmentWidth;
        }
    }
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
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

    // 创建时间轴控件
    // TimelineWidget* timeline = new TimelineWidget(&mainDialog);
    TimelineWidget* timeline = new TimelineWidget(nullptr, &basicInfo);
    timeline->setWindowTitle("时间轴");
    timeline->resize(400, 100);
    timeline->show();
    
    // 添加一些示例颜色段
    timeline->addSegment(0.3, Qt::red);
    timeline->addSegment(0.3, Qt::green); 
    timeline->addSegment(0.4, Qt::blue);
    return app.exec();
}
