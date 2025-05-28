#ifndef BASICINFO_H
#define BASICINFO_H

#include <QTime>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QPoint>

// 基本信息类，用于存储和管理工资计算器的基本配置信息
class BasicInfo {
public:
    double salary;      // 日薪金额
    int workHours;      // 每天工作时长(小时)
    QTime startTime;    // 上班开始时间
    QTimer* timer;      // 定时器,用于定期更新界面
    QSystemTrayIcon* trayIcon;  // 系统托盘图标
    bool isDraggable;   // 是否可拖动窗口
    QPoint dragPosition;  // 记录拖动时的鼠标位置


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

#endif // BASICINFO_H