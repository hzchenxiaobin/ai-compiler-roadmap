#include <iostream>
#include <string>
#include "contact.h"

// ===== 通讯录 v2 的调度区：启动读盘 + 菜单循环 + 退出存盘 =====
const std::string FILE_NAME = "contacts_v2.txt";

enum MenuChoice { Exit = 0, Add = 1, Remove = 2, Update = 3, Find = 4, List = 5 };

int main() {
    Contact contacts[MAX_CONTACTS];
    int count = load_contacts(contacts, FILE_NAME);       // v2：启动先读盘
    std::cout << "=== 通讯录 v2（持久化版）===" << std::endl;
    std::cout << "启动时从 " << FILE_NAME << " 读到 " << count << " 位联系人" << std::endl;

    while (true) {
        std::cout << "\n1)新增 2)删除 3)改号 4)查找 5)列表 0)退出　请选择：";
        int choice = 0;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;                                     // EOF 也走正常存盘出口
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "请输入数字 0~5。" << std::endl;
            continue;
        }
        std::cin.ignore(10000, '\n');

        if (choice == Exit) {
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
                if (!is_valid_name(name)) {                // v2：输入校验
                    std::cout << "姓名不能为空。" << std::endl;
                } else if (!is_valid_phone(phone)) {
                    std::cout << "电话不合法（5~15 位纯数字）。" << std::endl;
                } else {
                    int r = add_contact(contacts, count, name, phone);
                    if (r == 0) {
                        std::cout << "已添加 " << name << "（当前 " << count << " 人）" << std::endl;
                    } else {
                        std::cout << "添加失败：同名已存在或通讯录已满。" << std::endl;
                    }
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
                if (!is_valid_phone(phone)) {              // v2：改号也校验
                    std::cout << "电话不合法（5~15 位纯数字）。" << std::endl;
                } else {
                    std::cout << (update_contact(contacts, count, name, phone)
                                      ? "已更新 " + name
                                      : "没找到 " + name) << std::endl;
                }
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

    save_contacts(contacts, count, FILE_NAME);            // v2：收摊存盘
    std::cout << "已保存 " << count << " 位联系人到 " << FILE_NAME << "，再见！" << std::endl;
    return 0;
}
