#include <iostream>

// this 指针：成员函数里隐含的"我自己的门牌"
class Probe {
private:
    int id_;

public:
    explicit Probe(int id) : id_(id) {}

    void who_am_i() const {
        // this 是指向当前对象的指针；*this 是对象本身
        std::cout << "我是对象 #" << id_ << "，this = " << this << std::endl;
    }

    bool same_as(const Probe& other) const {
        return this == &other;          // 比较两个引用是不是同一个对象
    }

    Probe& scale(int times) {           // 返回 *this：支持链式调用
        id_ *= times;
        return *this;
    }

    int id() const { return id_; }
};

int main() {
    Probe a(1);
    Probe b(2);

    a.who_am_i();                        // 打印 a 的地址
    b.who_am_i();                        // 打印 b 的地址（不同）

    std::cout << "a 和 a 是同一个对象吗？" << (a.same_as(a) ? "是" : "否") << std::endl;
    std::cout << "a 和 b 是同一个对象吗？" << (a.same_as(b) ? "是" : "否") << std::endl;

    a.scale(10).scale(3);                // 链式调用：scale 返回 *this
    std::cout << "a.scale(10).scale(3) 后 id = " << a.id() << std::endl;
    return 0;
}
