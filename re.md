$env:Path += ";D:\Qt\Tools\mingw1310_64\bin"

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="D:/QT/6.10.0/mingw_64"

# 将依赖拷贝到 执行文件环境中
D:\QT\6.10.0\mingw_64\bin\windeployqt.exe .\MagicFishing.exe


.\MaintenanceTool.exe --mirror https://mirrors.sjtug.sjtu.edu.cn/qt/

mingw32-make