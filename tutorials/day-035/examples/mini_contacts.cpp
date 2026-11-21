#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// ===== Day 35 阶段自测参考答案：脱稿版通讯录核心 =====
// 核心功能子集：读盘 → 新增 → 查找 → 列表 → 存盘
// 先合上教程自己写，写完再对照这份参考

struct Contact {
    std::string name;
    std::string phone;
};

int load_contacts(Contact contacts[], const char* file) {
    std::ifstream in(file);
    if (!in) {
        return 0;
    }
    int count = 0;
    std::string line;
    while (std::getline(in, line) && count < 100) {
        std::istringstream ss(line);
        if (std::getline(ss, contacts[count].name, ',') &&
            std::getline(ss, contacts[count].phone)) {
            ++count;
        }
    }
    return count;
}

void save_contacts(const Contact contacts[], int count, const char* file) {
    std::ofstream out(file);
    for (int i = 0; i < count; ++i) {
        out << contacts[i].name << "," << contacts[i].phone << std::endl;
    }
}

int add_contact(Contact contacts[], int& count, const std::string& name, const std::string& phone) {
    if (count >= 100) {
        return 2;
    }
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            return 1;
        }
    }
    contacts[count].name = name;
    contacts[count].phone = phone;
    ++count;
    return 0;
}

int find_contact(const Contact contacts[], int count, const std::string& name) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            return i;
        }
    }
    return -1;
}

void list_contacts(const Contact contacts[], int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "  " << (i + 1) << ". " << contacts[i].name
                  << "　" << contacts[i].phone << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "用法：" << argv[0] << " 数据文件" << std::endl;
        return 1;
    }
    const char* file = argv[1];              // 文件名从命令行来（argv 学以致用）

    Contact contacts[100];
    int count = load_contacts(contacts, file);
    std::cout << "启动读到 " << count << " 位联系人" << std::endl;

    // 简化菜单：数字选功能，姓名电话逐行输入
    int choice = 0;
    while (true) {
        std::cout << "1)新增 2)查找 3)列表 0)退出：";
        if (!(std::cin >> choice)) {
            break;                            // EOF 也走存盘出口
        }
        std::cin.ignore(10000, '\n');

        if (choice == 0) {
            break;
        }
        std::string name;
        std::string phone;
        if (choice == 1) {
            std::cout << "姓名 电话（空格隔开）：";
            std::cin >> name >> phone;
            std::cin.ignore(10000, '\n');
            int r = add_contact(contacts, count, name, phone);
            std::cout << (r == 0 ? "已添加" : (r == 1 ? "同名已存在" : "已满")) << std::endl;
        } else if (choice == 2) {
            std::cout << "姓名：";
            std::cin >> name;
            std::cin.ignore(10000, '\n');
            int i = find_contact(contacts, count, name);
            if (i >= 0) {
                std::cout << "  找到：" << contacts[i].name << "　" << contacts[i].phone << std::endl;
            } else {
                std::cout << "  没找到" << std::endl;
            }
        } else if (choice == 3) {
            list_contacts(contacts, count);
        }
    }

    save_contacts(contacts, count, file);
    std::cout << "已保存 " << count << " 位联系人，再见" << std::endl;
    return 0;
}
