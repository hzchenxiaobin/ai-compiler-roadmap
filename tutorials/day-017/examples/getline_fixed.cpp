#include <iostream>
#include <string>

// 修复版：>> 之后、getline 之前，先冲掉残留的换行
int main() {
    int age = 0;
    std::string name;

    std::cout << "年龄：";
    std::cin >> age;
    std::cin.ignore(10000, '\n');          // 冲掉换行（Day 4 抢救三件套的近亲）
    std::cout << "姓名：";
    std::getline(std::cin, name);          // 现在拿到的是真正的整行

    std::cout << "读到的 name 是【" << name << "】，长度 " << name.size() << std::endl;
    return 0;
}
