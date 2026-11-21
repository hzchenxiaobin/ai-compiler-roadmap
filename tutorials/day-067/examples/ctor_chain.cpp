#include <iostream>
#include <string>

// 继承的构造顺序：基类部分先建、派生类部分后建（组合规则的近亲）
class Person {
protected:
    std::string name_;

public:
    explicit Person(const std::string& name) : name_(name) {
        std::cout << "  [Person 构造] " << name_ << std::endl;
    }
    ~Person() {
        std::cout << "  [Person 析构] " << name_ << std::endl;
    }
};

class Student : public Person {
private:
    int grade_;

public:
    Student(const std::string& name, int grade) : Person(name), grade_(grade) {
        std::cout << "  [Student 构造] " << name_ << " " << grade_ << " 年级" << std::endl;
    }
    ~Student() {
        std::cout << "  [Student 析构] " << name_ << std::endl;
    }
};

int main() {
    std::cout << "声明 Student s（进入作用域）：" << std::endl;
    {
        Student s("小明", 9);
        std::cout << "  ……使用 s……" << std::endl;
    }
    std::cout << "s 已销毁（离开作用域）" << std::endl;
    return 0;
}
