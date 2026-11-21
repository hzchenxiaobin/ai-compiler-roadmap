#include <iostream>
#include <string>

// 坑：cin >> 之后再 getline，会拿到一个"空行"
int main() {
    int age = 0;
    std::string name;

    std::cout << "年龄：";
    std::cin >> age;                       // 吃掉 20，把换行符留在传送带上
    std::cout << "姓名：";
    std::getline(std::cin, name);          // 一口吞掉残留的换行 → 拿到空串！

    std::cout << "读到的 name 是【" << name << "】，长度 " << name.size() << std::endl;
    return 0;
}
