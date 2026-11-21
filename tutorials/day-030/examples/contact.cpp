#include <iostream>
#include "contact.h"

// ===== 通讯录 v1 的机器区 =====

// 新增：返回 0 成功；1 已存在；2 已满
int add_contact(Contact contacts[], int& count, const std::string& name, const std::string& phone) {
    if (count >= MAX_CONTACTS) {
        return 2;
    }
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            return 1;                   // 同名拒绝
        }
    }
    contacts[count].name = name;
    contacts[count].phone = phone;
    ++count;
    return 0;
}

// 删除：找到后从后往前覆盖，腾出空位
bool remove_contact(Contact contacts[], int& count, const std::string& name) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            for (int j = i; j + 1 < count; ++j) {
                contacts[j] = contacts[j + 1];   // 后面的卡片整体前移一格
            }
            --count;
            return true;
        }
    }
    return false;
}

// 修改：按名字找到，改电话栏
bool update_contact(Contact contacts[], int count, const std::string& name, const std::string& phone) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            contacts[i].phone = phone;
            return true;
        }
    }
    return false;
}

// 查找：打印匹配的卡片
bool find_contact(const Contact contacts[], int count, const std::string& name) {
    for (int i = 0; i < count; ++i) {
        if (contacts[i].name == name) {
            std::cout << "  找到：" << contacts[i].name << "　" << contacts[i].phone << std::endl;
            return true;
        }
    }
    return false;
}

// 列表：全部卡片带序号
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
