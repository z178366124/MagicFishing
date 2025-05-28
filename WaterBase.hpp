#ifndef WATERBASE_HPP
#define WATERBASE_HPP

#include <QWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMouseEvent>
#include <QPoint>

class WaterBase: public QWidget {
public:
    void toggleDraggable() {
        isDraggable = !isDraggable;
        updateWindowFlags();
        show();  // 重新显示窗口以应用新的窗口标志
    }

protected:
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
};

#endif // WATERBASE_HPP