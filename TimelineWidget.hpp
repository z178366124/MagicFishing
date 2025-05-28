#ifndef TIMELINEWIDGET_HPP
#define TIMELINEWIDGET_HPP

#include "BaseicInfo.h"
#include "WaterBase.hpp"
#include "utils/WindowApi.hpp"

class TimelineWidget : public WaterBase {
private:
    QVector<QPair<double, QColor>> segments; // 存储每个颜色段的百分比和颜色

public:
    TimelineWidget(QWidget *parent = nullptr, BasicInfo *basicInfo = nullptr) : WaterBase(), m_basicInfo(basicInfo) {
        setMinimumHeight(30);
        this->isDraggable = false;  // 初始化拖动状态
        updateWindowFlags();  // 使用新函数设置窗口标志
        setAttribute(Qt::WA_TranslucentBackground);     // 背景透明
        setAttribute(Qt::WA_ShowWithoutActivating);     // 保持窗口在顶层

        // 设置全屏大小
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        resize(screenGeometry.size());
        move(screenGeometry.topLeft());

        QTime currentTime = QTime::currentTime();
        // int secondsWorked = QTime(18,0).secsTo(currentTime); // 从9点开始计算
        int secondsWorked = m_basicInfo->startTime.secsTo(currentTime);
        int totalSeconds = m_basicInfo->workHours * 3600;  // 总工作时长(秒)
        addSegment((double)secondsWorked / (double)totalSeconds, Qt::gray);
        initTimer();
        show();
    }
    void addSegment(double percentage, const QColor color) {
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

        int idleTime = GetIdleTime();
        // qDebug() << "idleTime" << idleTime;
        QColor c = Qt::green;
        if(idleTime > 10 * 1000){
            c = Qt::red;
        }

        

        if (!segments.isEmpty()) {
            if (segments.last().second == c) {
                // 如果最后一段是红色,增加其百分比
                double newPercentage = segments.last().first + 10.0/(m_basicInfo->workHours * 3600);
                segments.last().first = newPercentage;
                // qDebug() << "size:" << segments.size() <<" newPercentage:" << newPercentage;
            } else {
                // 如果最后一段不是红色,添加新的红色段
                // qDebug() << "is red:" << (segments.last().second == Qt::red) << " is green:" << (segments.last().second == Qt::green);
                // qDebug() << "size:" << segments.size() <<" newPercentage:" << 1.0/(m_basicInfo->workHours * 3600) * 100;

                addSegment(10.0/(m_basicInfo->workHours * 3600), c);

            }
        } else {
            // 如果没有任何段,添加新的红色段
            addSegment(1.0/(m_basicInfo->workHours * 3600), c);
        }

        // 绘制颜色段
        double currentX = 0;
        for (const auto& segment : segments) {
            int segmentWidth = width() * segment.first;
            painter.setPen(QPen(segment.second, 1));
            painter.drawLine(currentX, lineY, currentX + segmentWidth, lineY);
            currentX += segmentWidth;
        }
    }

private:
    void initTimer() {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, static_cast<void (QWidget::*)()>(&QWidget::update));
        timer->start(1000); // 每秒更新一次
    }

    
};

#endif // TIMELINEWIDGET_HPP