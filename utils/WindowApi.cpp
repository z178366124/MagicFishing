#include "WindowApi.hpp"

int main() {
    const DWORD idleThreshold = 10 * 1000; // 10秒（毫秒）

    while (true) {
        DWORD idleTime = GetIdleTime();
        if (idleTime >= idleThreshold) {
            std::cout << "[状态] 空闲中（Idle）... 已空闲 " << idleTime / 1000 << " 秒" << std::endl;
        } else {
            std::cout << "[状态] 正在使用（Active）... 空闲时间 " << idleTime / 1000 << " 秒" << std::endl;
        }

        Sleep(1000); // 每秒检查一次
    }

    return 0;
} 