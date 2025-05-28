#ifndef WATERMARKWINDOW_HPP
#define WATERMARKWINDOW_HPP

#include "BaseicInfo.h"
#include "WaterBase.hpp"

class WatermarkWindow : public WaterBase {
public:
    WatermarkWindow(double salary, int workHours, QTime startTime) : WaterBase() {
        this->salary = salary;
        this->workHours = workHours;
        this->startTime = startTime;
        this->isDraggable = false;  // 初始化拖动状态
        updateWindowFlags();  // 使用新函数设置窗口标志
        setAttribute(Qt::WA_TranslucentBackground);     // 背景透明
        setAttribute(Qt::WA_ShowWithoutActivating);     // 保持窗口在顶层

        // 设置全屏大小
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        resize(screenGeometry.size());
        move(screenGeometry.topLeft());
        
        initTimer();  // 初始化并启动定时器
        show();  // 显示窗口
    }

    void updateDisplay(const QString& formatStr, int textSize, const QColor& color, const QFont& font){
        m_formatStr = formatStr;
        m_textSize = textSize;
        m_fontColor = color;
        m_font.setFamily(font.family());
        m_font.setPointSize(m_textSize);
        m_font.setBold(font.bold());
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
        // gradient.setColorAt(0, QColor(255, 0, 255, 128));  // 紫色
        // gradient.setColorAt(0.5, QColor(0, 255, 255, 128));  // 青色
        // gradient.setColorAt(1, QColor(255, 255, 0, 128));  // 黄色
        // painter.setPen(QPen(QBrush(gradient), 2));  // 使用渐变笔刷

        painter.setPen(QPen(m_fontColor, 2));  // 使用单一颜色
        // painter.setFont(QFont("Arial", m_textSize, QFont::Bold));
        painter.setFont(m_font);

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
            QString text = QString(m_formatStr).arg(earnedMoney, 0, 'f', 2);
            
            // 绘制文本
            QRect textRect = rect();
            textRect.setBottom(height() / 2);  // 文本在上半部分
            painter.drawText(textRect, Qt::AlignCenter, text); 
        } else {
            painter.drawText(rect(), Qt::AlignCenter, "非工作时间");
        }
        // qDebug() << "secondsWorked:" << secondsWorked;
    }

private:
    QString m_formatStr = "今日已赚: ¥%1";
    int m_textSize = 30;
    QColor m_fontColor = QColor(173, 216, 230); // 浅蓝色
    QFont m_font = QFont("Arial", m_textSize, QFont::Bold);
    

    double salary;      // 日薪金额
    int workHours;      // 每天工作时长(小时)
    QTime startTime;    // 上班开始时间
    QTimer* timer;      // 定时器,用于定期更新界面
    QSystemTrayIcon* trayIcon;  // 系统托盘图标

    void initTimer() {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, static_cast<void (QWidget::*)()>(&QWidget::update));
        timer->start(1000); // 每秒更新一次
    }
};

#endif // WATERMARKWINDOW_HPP