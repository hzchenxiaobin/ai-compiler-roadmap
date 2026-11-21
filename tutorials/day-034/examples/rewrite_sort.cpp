#include <iostream>
#include <string>

// 重写四：struct 卡片 + 冒泡排序（Day 29）
struct Student {
    std::string name;
    int score;
};

int main() {
    Student roster[4] = {{"小明", 92}, {"小红", 78}, {"小刚", 85}, {"小丽", 66}};

    for (int round = 0; round < 3; ++round) {
        for (int i = 0; i + 1 < 4 - round; ++i) {
            if (roster[i].score > roster[i + 1].score) {
                Student t = roster[i];              // 整卡交换
                roster[i] = roster[i + 1];
                roster[i + 1] = t;
            }
        }
    }
    for (int i = 0; i < 4; ++i) {
        std::cout << roster[i].name << "：" << roster[i].score << "　";
    }
    std::cout << std::endl;
    return 0;
}
