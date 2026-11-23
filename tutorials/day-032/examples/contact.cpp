#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include "contact.h"

// ===== 通讯录 v2 的机器区 = v1 五项 + 存读盘 + 校验 =====
// （五项操作原样沿用 v1，实现见下方）

int add_contact(Contact contacts[], int& count, const std::string& name, const std::string& phone) {
    if (count >= MAX_CONTACTS) {
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

bool remove_contact(Contact contacts[], int& count, const std::string& name) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            for (int j = i; j + 1 < count; ++j) {
                contacts[j] = contacts[j + 1];
            }
            --count;
            return true;
        }
    }
    return false;
}

bool update_contact(Contact contacts[], int count, const std::string& name, const std::string& phone) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            contacts[i].phone = phone;
            return true;
        }
    }
    return false;
}

bool find_contact(const Contact contacts[], int count, const std::string& name) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            std::cout << "  找到：" << contacts[i].name << "　" << contacts[i].phone << std::endl;
            return true;
        }
    }
    return false;
}

void list_contacts(const Contact contacts[], int count) {
    if (count == 0) {
        std::cout << "  （通讯录是空的）" << std::endl;
        return;
    }
    for (int i = 0; i < count; ++i) {
        std::cout << "  " << (i + 1) << ". " << contacts[i].name
                  << "　" << contacts[i].phone << std::endl;
    }
}

// ---- v2 新增：持久化（Day 31 的存读盘） ----
void save_contacts(const Contact contacts[], int count, const std::string& file_name) {
    std::ofstream out(file_name);
    for (int i = 0; i < count; ++i) {
        out << contacts[i].name << "," << contacts[i].phone << std::endl;
    }
}

int load_contacts(Contact contacts[], const std::string& file_name) {
    std::ifstream in(file_name);
    if (!in) {
        return 0;                          // 文件不存在：首次运行，空表起步
    }
    int count = 0;
    std::string line;
    while (std::getline(in, line) && count < MAX_CONTACTS) {
        std::istringstream ss(line);
        if (std::getline(ss, contacts[count].name, ',') &&
            std::getline(ss, contacts[count].phone)) {
            ++count;
        }
    }
    return count;
}

// ---- v2 新增：输入校验 ----
bool is_valid_name(const std::string& name) {
    return !name.empty();                  // 姓名非空即可
}

bool is_valid_phone(const std::string& phone) {
    if (phone.length() < 5 || phone.length() > 15) {
        return false;                      // 长度先卡一道
    }
    for (std::size_t i = 0; i < phone.size(); ++i) {
        if (!std::isdigit((unsigned char)phone[i])) {
            return false;                  // 全程数字（<cctype> 的 isdigit）
        }
    }
    return true;
}
