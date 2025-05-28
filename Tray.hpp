#ifndef TRAY_HPP
#define TRAY_HPP

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QIcon>
#include <QDebug>
#include <QKeySequence>
#include <QObject>
#include <QMetaObject>

class Tray : public QObject {
    Q_OBJECT
public:
    // 构造函数,初始化成员变量
    Tray() : trayIcon(nullptr), m_menu(nullptr) {}
    
    // 析构函数,释放资源
    ~Tray() {
        if(trayIcon) {
            delete trayIcon;
            trayIcon = nullptr;
        }
        if(m_menu) {
            delete m_menu;
            m_menu = nullptr;
        }
    }

    // 初始化托盘图标
    void initTrayIcon() {
        trayIcon = new QSystemTrayIcon();
        trayIcon->setIcon(QIcon(":/icon.ico"));
        trayIcon->setToolTip("工资计算器"); // 设置提示文字

        // 检查系统托盘是否可用
        if (!trayIcon->isSystemTrayAvailable()) {
            qDebug() << "系统托盘不可用";
            return;
        }

        // 检查图标是否加载成功
        if (!trayIcon->icon().isNull()) {
            qDebug() << "图标加载成功";
        } else {
            qDebug() << "图标加载失败";
            exit(1);
        }

        // 创建托盘菜单
        createTrayMenu();
        
        // 显示托盘图标
        trayIcon->show();

        // 连接托盘图标的信号槽
        connect(trayIcon, &QSystemTrayIcon::activated, this, &Tray::onTrayIconActivated);
    }

    // 获取托盘图标指针
    QSystemTrayIcon* getTrayIcon() const {
        return trayIcon;
    }

    QAction* showAction;
    QAction* hideAction; 
    QAction* positionAction;
    QAction* settingsAction;
    QAction* quitAction;

protected:
    // 处理托盘图标的激活事件
    virtual void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
        switch(reason) {
            case QSystemTrayIcon::DoubleClick:
                emit doubleClicked();
                break;
            case QSystemTrayIcon::Trigger:
                emit clicked();
                break;
            default:
                break;
        }
    }

signals:
    void clicked();
    void doubleClicked();

private:
    QSystemTrayIcon* trayIcon;
    QMenu* m_menu;

    // 创建托盘菜单
    void createTrayMenu() {
        m_menu = new QMenu();
        
        showAction = new QAction(QString("显示"), this);
        hideAction = new QAction(QString("隐藏"), this);
        positionAction = new QAction(QString("位置控制"), this);
        settingsAction = new QAction(QString("设置"), this);
        quitAction = new QAction(QString("退出"), this);

        // 添加快捷键
        showAction->setShortcut(QKeySequence("Ctrl+S"));
        hideAction->setShortcut(QKeySequence("Ctrl+H"));
        quitAction->setShortcut(QKeySequence("Ctrl+Q"));

        // 默认连接
        // connect(showAction, &QAction::triggered, this, &QWidget::show);
        // connect(hideAction, &QAction::triggered, this, &QWidget::hide);
        connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

        m_menu->addAction(showAction);
        m_menu->addAction(hideAction);
        m_menu->addAction(positionAction);
        m_menu->addAction(settingsAction);
        m_menu->addSeparator();
        m_menu->addAction(quitAction);

        trayIcon->setContextMenu(m_menu);
    }

    // 提供对外连接接口
    // void connectPositionAction(const QObject* receiver, const char* member) {
    //     connect(positionAction, SIGNAL(triggered()), receiver, member);
    // }

    // void connectSettingsAction(const QObject* receiver, const char* member) {
    //     connect(settingsAction, SIGNAL(triggered()), receiver, member);
    // }

private:

    // 显示设置对话框
    virtual void showSettings() {
        // 子类实现具体的设置界面
    }
};

#endif // TRAY_HPP
