#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// 主角：Contact 数组的存盘与读盘（每行一条 "姓名,电话"）
struct Contact {
    std::string name;
    std::string phone;
};

const std::string FILE_NAME = "contacts.txt";

void save_contacts(const Contact contacts[], int count) {
    std::ofstream out(FILE_NAME);
    for (int i = 0; i < count; ++i) {
        out << contacts[i].name << "," << contacts[i].phone << std::endl;
    }
    std::cout << "已把 " << count << " 位联系人写入 " << FILE_NAME << std::endl;
}

int load_contacts(Contact contacts[]) {
    std::ifstream in(FILE_NAME);
    if (!in) {
        return 0;                              // 文件不存在：空通讯录起步
    }
    int count = 0;
    std::string line;
    while (std::getline(in, line) && count < 100) {
        std::istringstream ss(line);           // Day 17：按逗号切两段
        if (std::getline(ss, contacts[count].name, ',') &&
            std::getline(ss, contacts[count].phone)) {
            ++count;
        }
    }
    return count;
}

int main() {
    Contact contacts[100];

    int loaded = load_contacts(contacts);      // 启动先读盘
    std::cout << "启动时从文件读到 " << loaded << " 位联系人" << std::endl;

    if (loaded + 2 <= 100) {                   // 容量守卫：装得下才加（Day 13 军规）
        contacts[loaded] = {"张三", "13800000001"};
        contacts[loaded + 1] = {"李四", "13800000002"};
        loaded += 2;                           // 内存里再加两位
    }

    save_contacts(contacts, loaded);           // 收摊前存盘
    return 0;
}
