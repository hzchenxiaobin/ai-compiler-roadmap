#include <iostream>
#include <string>

// greet_full.cpp 的升级版：末位参数带默认值
void greet(const std::string& name, const std::string& greeting = "你好", int times = 1) {
    for (int i = 0; i < times; ++i) {
        std::cout << greeting << "，" << name << "！" << std::endl;
    }
}

int main() {
    greet("小明", "早上好", 2);   // 三样全给：打 2 遍
    greet("小红", "晚上好");      // 省略 times：默认打 1 遍
    greet("小刚");               // 只给名字：连问候语也用默认
    return 0;
}
