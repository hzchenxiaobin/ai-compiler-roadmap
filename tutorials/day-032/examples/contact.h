#ifndef CONTACT_H
#define CONTACT_H

#include <string>

// ===== 通讯录 v2 的名片区：v1 + 持久化 + 输入校验 =====
struct Contact {
    std::string name;
    std::string phone;
};

const int MAX_CONTACTS = 100;

// v1 的五项操作（不变）
int  add_contact(Contact contacts[], int& count, const std::string& name, const std::string& phone);
bool remove_contact(Contact contacts[], int& count, const std::string& name);
bool update_contact(Contact contacts[], int count, const std::string& name, const std::string& phone);
bool find_contact(const Contact contacts[], int count, const std::string& name);
void list_contacts(const Contact contacts[], int count);

// v2 新增：持久化（Day 31）与输入校验
void save_contacts(const Contact contacts[], int count, const std::string& file_name);
int  load_contacts(Contact contacts[], const std::string& file_name);
bool is_valid_name(const std::string& name);
bool is_valid_phone(const std::string& phone);

#endif
