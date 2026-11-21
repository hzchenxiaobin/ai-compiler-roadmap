#include <iostream>
#include <string>

// 结构体：把一个学生的信息打包成一张卡片
struct Student {
    std::string name;
    int score;
    char grade;
};

int main() {
    Student s1 = {"小明", 92, 'A'};          // 按成员顺序初始化
    Student s2;
    s2.name = "小红";                         // 也可以逐个成员赋值
    s2.score = 78;
    s2.grade = 'C';

    std::cout << s1.name << "：" << s1.score << " 分，等级 " << s1.grade << std::endl;
    std::cout << s2.name << "：" << s2.score << " 分，等级 " << s2.grade << std::endl;

    s1.score = 95;                            // 改成员就是改卡片上的那一栏
    std::cout << "改分后 " << s1.name << "：" << s1.score << std::endl;

    std::cout << "sizeof(Student) = " << sizeof(Student) << "（三样家当打包后的大小）" << std::endl;
    std::cout << "sizeof(s1) 与 sizeof(Student) 相同：" << (sizeof(s1) == sizeof(Student) ? "是" : "否") << std::endl;
    return 0;
}
