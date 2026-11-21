#include <iostream>
#include <string>

// 第 10 天主角二：带默认参数的 greet
// 规则：默认值只能出现在参数表最右边，且必须连续
void greet(const std::string& name, const std::string& greeting, int times) {
    for (int i = 0; i < times; ++i) {
        std::cout << greeting << "，" << name << "！" << std::endl;
    }
}

int main() {
    greet("小明", "早上好", 1);     // 三样全给
    greet("小红", "加油", 2);       // 换个问候，打两遍
    return 0;
}
