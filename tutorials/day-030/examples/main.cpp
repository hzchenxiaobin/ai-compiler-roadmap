#include <iostream>
#include <string>
#include <cstdlib>
#include "contact.h"

// ===== 通讯录 v1 的调度区：菜单循环 =====
enum MenuChoice { Exit = 0, Add = 1, Remove = 2, Update = 3, Find = 4, List = 5 };

int main() {
    Contact contacts[MAX_CONTACTS];    // 一叠卡片（定长数组）
    int count = 0;                     // 当前有效卡数（数组的长随行计数）

    std::cout << "=== 通讯录 v1（定长数组版）===" << std::endl;
    while (true) {
        std::cout << "\n1)新增 2)删除 3)改号 4)查找 5)列表 0)退出　请选择：";
        int choice = 0;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                std::cout << "\n输入结束，再见。" << std::endl;
                return 0;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "请输入数字 0~5。" << std::endl;
            continue;
        }
        std::cin.ignore(10000, '\n');          // 冲掉数字后的换行，给 getline 让路

        if (choice == Exit) {
            std::cout << "再见！当前共 " << count << " 位联系人。" << std::endl;
            break;
        }

        std::string name;
        std::string phone;
        switch ((MenuChoice)choice) {
            case Add:
                std::cout << "姓名：";
                std::getline(std::cin, name);
                std::cout << "电话：";
                std::getline(std::cin, phone);
                if (add_contact(contacts, count, name, phone) == 0) {
                    std::cout << "已添加 " << name << "（当前 " << count << " 人）" << std::endl;
                } else {
                    std::cout << "添加失败：同名已存在或通讯录已满。" << std::endl;
                }
                break;
            case Remove:
                std::cout << "要删除的姓名：";
                std::getline(std::cin, name);
                std::cout << (remove_contact(contacts, count, name)
                                  ? "已删除 " + name
                                  : "没找到 " + name) << std::endl;
                break;
            case Update:
                std::cout << "要改号的姓名：";
                std::getline(std::cin, name);
                std::cout << "新电话：";
                std::getline(std::cin, phone);
                std::cout << (update_contact(contacts, count, name, phone)
                                  ? "已更新 " + name
                                  : "没找到 " + name) << std::endl;
                break;
            case Find:
                std::cout << "要查找的姓名：";
                std::getline(std::cin, name);
                if (!find_contact(contacts, count, name)) {
                    std::cout << "  没找到 " << name << std::endl;
                }
                break;
            case List:
                list_contacts(contacts, count);
                break;
            default:
                std::cout << "没有这个选项：" << choice << std::endl;
        }
    }
    return 0;
}
