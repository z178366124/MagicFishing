cmake_minimum_required(VERSION 3.16)
project(MagicFishing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置版本信息
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

# 终端显示配置
if (WIN32)
  set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

# Qt 安装路径可能要自己指定
set(Qt6_DIR "D:/QT/6.10.0/mingw_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR})

# 添加源文件
set(SOURCES
    # main.cc
    main.cpp
    WaterBase.hpp
    tray.hpp
    WorkTimeWindow.hpp
    WorkTimeWindow.hpp
    TimelineWidget.hpp
    utils/WindowApi.hpp
    SettingWidget.hpp
    app.rc  # 添加资源文件
)


add_executable(MagicFishing ${SOURCES} resources.qrc)
target_link_libraries(MagicFishing PRIVATE Qt6::Widgets Qt6::Core)


# 设置应用程序图标
if(WIN32)
    set_target_properties(MagicFishing PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "-Wl,--subsystem,windows"
    )
endif()
