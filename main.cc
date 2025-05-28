#include <QApplication>
#include "SettingWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    SettingWidget* settingWidget = SettingWidget::getInstance();
    settingWidget->show();
    
    return app.exec();
}
