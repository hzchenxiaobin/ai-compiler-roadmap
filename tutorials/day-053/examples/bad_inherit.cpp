#include <iostream>
#include <string>
#include <vector>

// 坏例：为复用而继承——Stack 继承 vector 来"白得" push_back
// 问题：vector 的随机访问/中间插入对 Stack 全部不该暴露——但继承全给了
template <typename T>
class BadStack : public std::vector<T> {       // "是一个 vector"？——不是！
public:
    void push(const T& v) { this->push_back(v); }
    T pop() {
        T v = this->back();
        this->pop_back();
        return v;
    }
};

int main() {
    BadStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    std::cout << "pop = " << s.pop() << std::endl;   // 3（后进先出 ✔）

    // 坏例的漏洞：Stack 不该有的操作全部暴露——
    s.insert(s.begin(), 99);                         // 中间插入？Stack 没这个语义！
    s[0] = 42;                                       // 随机访问改值？破坏后进先出！
    std::cout << "s[0] = " << s[0] << "，size = " << s.size() << std::endl;
    std::cout << "（Stack 的不变量'只能顶端进出'已被绕过）" << std::endl;
    return 0;
}
