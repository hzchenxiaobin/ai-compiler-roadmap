#include <iostream>
#include <string>

// 基类指针指向派生类对象：统一的"人物档案"管理
class Person {
protected:
    std::string name_;

public:
    explicit Person(const std::string& name) : name_(name) {}
    void introduce() const {
        std::cout << "我是 " << name_ << std::endl;
    }
};

class Student : public Person {
private:
    int grade_;

public:
    Student(const std::string& name, int grade) : Person(name), grade_(grade) {}
    void study() const {
        std::cout << "  " << name_ << "（" << grade_ << " 年级）在学习" << std::endl;
    }
};

class Teacher : public Person {
private:
    std::string subject_;

public:
    Teacher(const std::string& name, const std::string& subject)
        : Person(name), subject_(subject) {}
    void teach() const {
        std::cout << "  " << name_ << " 老师在教 " << subject_ << std::endl;
    }
};

int main() {
    Student s1("小明", 9), s2("小红", 10);
    Teacher t("王老师", "数学");

    // 基类指针数组：学生和老师统一进"人物档案"
    Person* roster[] = {&s1, &s2, &t};

    std::cout << "统一点名（基类指针的威力）：" << std::endl;
    for (int i = 0; i < 3; ++i) {
        roster[i]->introduce();        // 都能调 Person 的函数
    }
    std::cout << "各自工作（派生类自己的能力，指针直接调不到）：" << std::endl;
    s1.study();
    t.teach();
    // roster[0]->study();   // 取消注释：编译错误！基类指针只认基类接口
    return 0;
}
