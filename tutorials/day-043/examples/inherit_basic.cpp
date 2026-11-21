#include <iostream>
#include <string>

// 继承基础：Person → Student / Teacher（计划指定练习）
class Person {
protected:                    // protected：派生类可见、外界仍不可见
    std::string name_;
    int age_;

public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}

    void introduce() const {
        std::cout << "我是 " << name_ << "，" << age_ << " 岁。" << std::endl;
    }
};

class Student : public Person {           // Student 是一个 Person（is-a）
private:
    int grade_;

public:
    Student(const std::string& name, int age, int grade)
        : Person(name, age), grade_(grade) {}       // 基类部分由 Person 构造

    void study() const {
        std::cout << name_ << "（" << grade_ << " 年级）正在学习 C++" << std::endl;
    }
};

class Teacher : public Person {
private:
    std::string subject_;

public:
    Teacher(const std::string& name, int age, const std::string& subject)
        : Person(name, age), subject_(subject) {}

    void teach() const {
        std::cout << name_ << " 老师（教 " << subject_ << "）正在上课" << std::endl;
    }
};

int main() {
    Student s("小明", 15, 9);
    Teacher t("王老师", 38, "数学");

    s.introduce();      // 继承来的能力：Person 的成员函数直接用
    s.study();          // 自己的新能力
    t.introduce();      // Teacher 也继承了 Person
    t.teach();

    // s.name_ = "改名";   // 取消注释：编译错误！protected 外界仍不可见
    return 0;
}
