#include <iostream>

// 枚举：给一组相关的整数常量起名
enum Level { Easy = 1, Medium = 2, Hard = 3 };

const char* level_name(Level lv) {
    switch (lv) {
        case Easy:   return "简单";
        case Medium: return "中等";
        case Hard:   return "困难";
    }
    return "未知";
}

int main() {
    Level today = Medium;                  // 只能取枚举里列出的值

    std::cout << "今天的练习难度：" << level_name(today) << std::endl;
    std::cout << "它背后的数值：" << today << "（枚举本质是整数）" << std::endl;

    today = Hard;                          // 换个难度
    std::cout << "升级后：" << level_name(today) << std::endl;

    // 配合 Day 5 的 switch：按枚举分发行为
    for (int lv = Easy; lv <= Hard; ++lv) {
        std::cout << "难度 " << lv << "：";
        switch ((Level)lv) {
            case Easy:   std::cout << "刷 10 题" << std::endl; break;
            case Medium: std::cout << "刷 20 题" << std::endl; break;
            case Hard:   std::cout << "刷 30 题" << std::endl; break;
        }
    }
    return 0;
}
