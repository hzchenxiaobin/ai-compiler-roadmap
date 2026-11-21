#include <iostream>
#include <string>

// 第 10 天主角一：三个重载的 print——同一个名字，按参数各就各位
void print(int value) {
    std::cout << "[整数] " << value << std::endl;
}

void print(double value) {
    std::cout << "[小数] " << value << std::endl;
}

void print(char value) {
    std::cout << "[字符] " << value << std::endl;
}

void print(const std::string& value) {
    std::cout << "[文本] " << value << std::endl;
}

int main() {
    print(42);            // 精确匹配 print(int)
    print(3.14);          // 精确匹配 print(double)
    print('A');           // 精确匹配 print(char)
    print("Hello");       // 匹配 print(string)
    return 0;
}
