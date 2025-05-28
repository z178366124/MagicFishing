#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QFontComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QListWidget>
#include <QColorDialog>
#include <QCloseEvent>
#include <QMessageBox>

// static SettingWidget* s_SettingWidget;

class SettingWidget : public QWidget {
    Q_OBJECT

signals:
    void settingsChanged(const QString& formatStr, int textSize, const QColor& color, const QFont& font);

public:
    static SettingWidget* getInstance() {
        static SettingWidget* instance = nullptr;
        if (instance == nullptr) {
            instance = new SettingWidget();
        }
        return instance;
    }
    void openWindow() {
        this->show();
    }
    
    void closeWindow() {
        this->hide();
    }

    void closeEvent(QCloseEvent* event) override {
        // 隐藏窗口而不是关闭
        this->hide();
        event->ignore();
    }
    
    ~SettingWidget() {}

private:
    SettingWidget(QWidget *parent = nullptr) : QWidget(parent) {
        initUI();
    }
    // friend SettingWidget* SettingWidget::getInstance();

    void initUI() {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        
        tabWidget = new QTabWidget(this);
        
        // 创建各个设置页面
        // setupGeneralSettings();
        setupAppearanceSettings(); 
        // setupFontSettings();
        // setupEditorSettings();
        // setupShortcutSettings();
        // setupPluginSettings();
        // setupAdvancedSettings();
        
        // 底部按钮
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        okButton = new QPushButton("确定", this);
        cancelButton = new QPushButton("取消", this);
        applyButton = new QPushButton("应用", this);
        
        buttonLayout->addStretch();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        buttonLayout->addWidget(applyButton);
        
        mainLayout->addWidget(tabWidget);
        mainLayout->addLayout(buttonLayout);
        
        connect(okButton, &QPushButton::clicked, this, &SettingWidget::onOkButtonClicked);
        connect(cancelButton, &QPushButton::clicked, this, &SettingWidget::closeWindow);
        connect(applyButton, &QPushButton::clicked, this, &SettingWidget::onApplyButtonClicked);
    }

    void onOkButtonClicked(){
        // 获取计费表示相关设置
        onApplyButtonClicked();
        
        this->close();
    }

    void onApplyButtonClicked(){
        QString formatStr = formatStringEdit->text();
        // 校验格式化字符串是否只包含一个 %1 参数
        int count = formatStr.count("%1");
        if (count != 1) {
            QMessageBox::warning(this, "格式错误", "格式化字符串只能包含一个 %1 参数，%1 为金额的位置");
            return;
        }
        int textSize = textSizeSpinBox->value();
        QColor color = colorButton->palette().color(QPalette::Button);
        qDebug() << formatStr << " "<< textSize << " "<< color ;
        
        // 应用设置
        emit settingsChanged(formatStr, textSize, color, fontComboBox->currentFont());
    }

    // void setupGeneralSettings() {
    //     QWidget* generalTab = new QWidget();
    //     QVBoxLayout* layout = new QVBoxLayout(generalTab);
        
    //     // 语言设置
    //     QGroupBox* languageGroup = new QGroupBox("语言设置");
    //     QVBoxLayout* langLayout = new QVBoxLayout();
    //     languageComboBox = new QComboBox();
    //     languageComboBox->addItems({"简体中文", "English"});
    //     langLayout->addWidget(languageComboBox);
    //     languageGroup->setLayout(langLayout);
        
    //     // 主题设置
    //     QGroupBox* themeGroup = new QGroupBox("主题设置");
    //     QVBoxLayout* themeLayout = new QVBoxLayout();
    //     themeComboBox = new QComboBox();
    //     themeComboBox->addItems({"浅色", "深色", "跟随系统"});
    //     themeLayout->addWidget(themeComboBox);
    //     themeGroup->setLayout(themeLayout);
        
    //     // 自动保存
    //     autoSaveCheckBox = new QCheckBox("启用自动保存");
    //     showWelcomePageCheckBox = new QCheckBox("启动时显示欢迎页面");
        
    //     // 最近文件数
    //     QHBoxLayout* recentLayout = new QHBoxLayout();
    //     recentLayout->addWidget(new QLabel("最近文件数量:"));
    //     recentFilesSpinBox = new QSpinBox();
    //     recentFilesSpinBox->setRange(0, 50);
    //     recentLayout->addWidget(recentFilesSpinBox);
        
    //     layout->addWidget(languageGroup);
    //     layout->addWidget(themeGroup);
    //     layout->addWidget(autoSaveCheckBox);
    //     layout->addWidget(showWelcomePageCheckBox);
    //     layout->addLayout(recentLayout);
    //     layout->addStretch();
        
    //     tabWidget->addTab(generalTab, "常规");
    // }

    void setupAppearanceSettings() {
        QWidget* appearanceTab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(appearanceTab);
        
        // 配色方案
        QGroupBox* textGroup = new QGroupBox("计费表示");
        QVBoxLayout* str_text = new QVBoxLayout();
        QHBoxLayout* formatLayout = new QHBoxLayout();
        formatLayout->addWidget(new QLabel("格式化字符串"));
        formatStringEdit = new QLineEdit();
        formatStringEdit->setText("今日已赚: ¥%1");
        formatStringEdit->setPlaceholderText("输入格式化字符串,如 %.2f");
        formatLayout->addWidget(formatStringEdit);
        str_text->addLayout(formatLayout);

        // 字号大小设置
        QHBoxLayout* fontSizeLayout = new QHBoxLayout();
        fontSizeLayout->addWidget(new QLabel("字号大小:"));
        textSizeSpinBox = new QSpinBox();
        textSizeSpinBox->setRange(8, 72);
        textSizeSpinBox->setValue(30); // 默认字号
        fontSizeLayout->addWidget(textSizeSpinBox);
        str_text->addLayout(fontSizeLayout);

        // 颜色选择
        QHBoxLayout* colorLayout = new QHBoxLayout();
        colorLayout->addWidget(new QLabel("文字颜色:"));
        colorButton = new QPushButton();
        colorButton->setFixedSize(50, 25);
        colorButton->setStyleSheet("background-color: #FF00FF;"); // 默认颜色
        connect(colorButton, &QPushButton::clicked, this, [=](){
            QColor color = QColorDialog::getColor(Qt::magenta, this, "选择文字颜色");
            if(color.isValid()) {
                colorButton->setStyleSheet(QString("background-color: %1;").arg(color.name()));
            }
        });
        colorLayout->addWidget(colorButton);
        str_text->addLayout(colorLayout);

        // 字体选择
        QHBoxLayout* fontLayout = new QHBoxLayout();
        fontLayout->addWidget(new QLabel("字体:"));
        fontComboBox = new QFontComboBox();
        fontComboBox->setCurrentFont(QFont("Arial")); // 默认字体
        fontLayout->addWidget(fontComboBox);
        str_text->addLayout(fontLayout);

        textGroup->setLayout(str_text);
        
        // // 界面元素
        // showToolbarCheckBox = new QCheckBox("显示工具栏");
        // showStatusBarCheckBox = new QCheckBox("显示状态栏");
        // enableAnimationsCheckBox = new QCheckBox("启用界面动画");
        
        layout->addWidget(textGroup);
        // layout->addWidget(showToolbarCheckBox);
        // layout->addWidget(showStatusBarCheckBox);
        // layout->addWidget(enableAnimationsCheckBox);
        // layout->addStretch();
        
        tabWidget->addTab(appearanceTab, "外观");
    }

    // void setupFontSettings() {
    //     QWidget* fontTab = new QWidget();
    //     QVBoxLayout* layout = new QVBoxLayout(fontTab);
        
    //     // 字体选择
    //     QGroupBox* fontGroup = new QGroupBox("字体设置");
    //     QVBoxLayout* fontLayout = new QVBoxLayout();
    //     editorFontComboBox = new QFontComboBox();
    //     fontSizeSpinBox = new QSpinBox();
    //     fontSizeSpinBox->setRange(8, 72);
        
    //     QHBoxLayout* styleLayout = new QHBoxLayout();
    //     boldCheckBox = new QCheckBox("粗体");
    //     italicCheckBox = new QCheckBox("斜体");
    //     styleLayout->addWidget(boldCheckBox);
    //     styleLayout->addWidget(italicCheckBox);
        
    //     fontLayout->addWidget(editorFontComboBox);
    //     fontLayout->addWidget(fontSizeSpinBox);
    //     fontLayout->addLayout(styleLayout);
    //     fontGroup->setLayout(fontLayout);
        
    //     // 行间距
    //     QHBoxLayout* lineSpacingLayout = new QHBoxLayout();
    //     lineSpacingLayout->addWidget(new QLabel("行间距:"));
    //     lineSpacingSpinBox = new QSpinBox();
    //     lineSpacingSpinBox->setRange(100, 200);
    //     lineSpacingSpinBox->setSuffix("%");
    //     lineSpacingLayout->addWidget(lineSpacingSpinBox);
        
    //     layout->addWidget(fontGroup);
    //     layout->addLayout(lineSpacingLayout);
    //     layout->addStretch();
        
    //     tabWidget->addTab(fontTab, "字体");
    // }

    // void setupEditorSettings() {
    //     QWidget* editorTab = new QWidget();
    //     QVBoxLayout* layout = new QVBoxLayout(editorTab);
        
    //     // 缩进设置
    //     QGroupBox* indentGroup = new QGroupBox("缩进");
    //     QVBoxLayout* indentLayout = new QVBoxLayout();
    //     indentTypeComboBox = new QComboBox();
    //     indentTypeComboBox->addItems({"空格", "制表符"});
    //     indentLayout->addWidget(indentTypeComboBox);
    //     indentGroup->setLayout(indentLayout);
        
    //     // 编辑器功能
    //     showLineNumbersCheckBox = new QCheckBox("显示行号");
    //     enableCodeFoldingCheckBox = new QCheckBox("启用代码折叠");
    //     wordWrapCheckBox = new QCheckBox("自动换行");
    //     bracketHighlightCheckBox = new QCheckBox("括号匹配高亮");
        
    //     layout->addWidget(indentGroup);
    //     layout->addWidget(showLineNumbersCheckBox);
    //     layout->addWidget(enableCodeFoldingCheckBox);
    //     layout->addWidget(wordWrapCheckBox);
    //     layout->addWidget(bracketHighlightCheckBox);
    //     layout->addStretch();
        
    //     tabWidget->addTab(editorTab, "编辑器");
    // }

    // void setupShortcutSettings() {
    //     QWidget* shortcutTab = new QWidget();
    //     QVBoxLayout* layout = new QVBoxLayout(shortcutTab);
        
    //     shortcutTableWidget = new QTableWidget();
    //     shortcutTableWidget->setColumnCount(2);
    //     shortcutTableWidget->setHorizontalHeaderLabels({"功能", "快捷键"});
        
    //     resetShortcutsButton = new QPushButton("恢复默认");
        
    //     layout->addWidget(shortcutTableWidget);
    //     layout->addWidget(resetShortcutsButton);
        
    //     tabWidget->addTab(shortcutTab, "快捷键");
    // }

    // void setupPluginSettings() {
    //     QWidget* pluginTab = new QWidget();
    //     QVBoxLayout* layout = new QVBoxLayout(pluginTab);
        
    //     pluginListWidget = new QListWidget();
        
    //     QHBoxLayout* buttonLayout = new QHBoxLayout();
    //     enablePluginButton = new QPushButton("启用");
    //     disablePluginButton = new QPushButton("禁用");
    //     buttonLayout->addWidget(enablePluginButton);
    //     buttonLayout->addWidget(disablePluginButton);
        
    //     layout->addWidget(pluginListWidget);
    //     layout->addLayout(buttonLayout);
        
    //     tabWidget->addTab(pluginTab, "插件");
    // }

    // void setupAdvancedSettings() {
    //     QWidget* advancedTab = new QWidget();
    //     QVBoxLayout* layout = new QVBoxLayout(advancedTab);
        
    //     // 性能模式
    //     QGroupBox* perfGroup = new QGroupBox("性能设置");
    //     QVBoxLayout* perfLayout = new QVBoxLayout();
    //     performanceModeComboBox = new QComboBox();
    //     performanceModeComboBox->addItems({"平衡", "性能优先", "节能"});
    //     perfLayout->addWidget(performanceModeComboBox);
    //     perfGroup->setLayout(perfLayout);
        
    //     // 缓存大小
    //     QHBoxLayout* cacheLayout = new QHBoxLayout();
    //     cacheLayout->addWidget(new QLabel("缓存大小:"));
    //     cacheSizeSpinBox = new QSpinBox();
    //     cacheSizeSpinBox->setRange(100, 1000);
    //     cacheSizeSpinBox->setSuffix(" MB");
    //     cacheLayout->addWidget(cacheSizeSpinBox);
        
    //     // 日志级别
    //     QGroupBox* logGroup = new QGroupBox("日志设置");
    //     QVBoxLayout* logLayout = new QVBoxLayout();
    //     logLevelComboBox = new QComboBox();
    //     logLevelComboBox->addItems({"错误", "警告", "信息", "调试"});
    //     logLayout->addWidget(logLevelComboBox);
    //     logGroup->setLayout(logLayout);
        
    //     debugModeCheckBox = new QCheckBox("调试模式");
        
    //     layout->addWidget(perfGroup);
    //     layout->addLayout(cacheLayout);
    //     layout->addWidget(logGroup);
    //     layout->addWidget(debugModeCheckBox);
    //     layout->addStretch();
        
    //     tabWidget->addTab(advancedTab, "高级");
    // }

private:
    // 常规设置组件
    QComboBox* languageComboBox;
    QComboBox* themeComboBox;
    QCheckBox* autoSaveCheckBox;
    QCheckBox* showWelcomePageCheckBox;
    QSpinBox* recentFilesSpinBox;

    // 外观设置组件
    QLineEdit* formatStringEdit;
    QSpinBox* textSizeSpinBox;
    QPushButton* colorButton;
    QFontComboBox* fontComboBox; // 字体选择
    QCheckBox* showToolbarCheckBox;
    QCheckBox* showStatusBarCheckBox;
    QCheckBox* enableAnimationsCheckBox;

    // 字体设置组件
    QFontComboBox* editorFontComboBox;
    QSpinBox* fontSizeSpinBox;
    QCheckBox* boldCheckBox;
    QCheckBox* italicCheckBox;
    QSpinBox* lineSpacingSpinBox;

    // 编辑器设置组件
    QComboBox* indentTypeComboBox;
    QCheckBox* showLineNumbersCheckBox;
    QCheckBox* enableCodeFoldingCheckBox;
    QCheckBox* wordWrapCheckBox;
    QCheckBox* bracketHighlightCheckBox;

    // 快捷键设置组件
    QTableWidget* shortcutTableWidget;
    QPushButton* resetShortcutsButton;

    // 插件设置组件
    QListWidget* pluginListWidget;
    QPushButton* enablePluginButton;
    QPushButton* disablePluginButton;

    // 高级设置组件
    QComboBox* performanceModeComboBox;
    QSpinBox* cacheSizeSpinBox;
    QComboBox* logLevelComboBox;
    QCheckBox* debugModeCheckBox;

    // 通用按钮
    QPushButton* applyButton;
    QPushButton* okButton;
    QPushButton* cancelButton;

    QTabWidget* tabWidget;
};
