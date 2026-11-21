#ifndef CONTACT_H
#define CONTACT_H

#include <string>

// ===== 通讯录 v1 的名片区 =====
// Contact：一张联系人卡片（Day 29 的 struct）
struct Contact {
    std::string name;
    std::string phone;
};

const int MAX_CONTACTS = 100;          // 定长数组的容量（Day 13）

// 通讯录的五项操作（机器在 contact.cpp）
int  add_contact(Contact contacts[], int& count, const std::string& name, const std::string& phone);
bool remove_contact(Contact contacts[], int& count, const std::string& name);
bool update_contact(Contact contacts[], int count, const std::string& name, const std::string& phone);
bool find_contact(const Contact contacts[], int count, const std::string& name);
void list_contacts(const Contact contacts[], int count);

#endif
