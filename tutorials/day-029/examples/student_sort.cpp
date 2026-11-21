#include <iostream>
#include <string>

// 计划练习：Student 结构体数组按分数排序（冒泡法，Day 93 正式学算法）
struct Student {
    std::string name;
    int score;
    char grade;
};

void print_roster(const Student roster[], int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "  " << roster[i].name << "："
                  << roster[i].score << " 分（" << roster[i].grade << "）" << std::endl;
    }
}

int main() {
    Student roster[5] = {
        {"小明", 92, 'A'},
        {"小红", 78, 'C'},
        {"小刚", 85, 'B'},
        {"小丽", 66, 'D'},
        {"小强", 59, 'F'},
    };

    std::cout << "排序前：" << std::endl;
    print_roster(roster, 5);

    for (int round = 0; round < 4; ++round) {          // 冒泡：每一轮把最小分"沉"到前面
        for (int i = 0; i + 1 < 5 - round; ++i) {
            if (roster[i].score > roster[i + 1].score) {
                Student t = roster[i];                  // 交换整张卡片（三个成员一起走）
                roster[i] = roster[i + 1];
                roster[i + 1] = t;
            }
        }
    }

    std::cout << "按分数升序后：" << std::endl;
    print_roster(roster, 5);
    return 0;
}
