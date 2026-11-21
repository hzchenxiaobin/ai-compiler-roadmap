#include <iostream>
#include <memory>
#include <string>

// 容器族运算符：[] () ->  一网打尽
class IntBox {                                   // [] 的主场：带边界检查的下标
private:
    int* data_;
    std::size_t size_;

public:
    explicit IntBox(std::size_t n) : data_(new int[n]{}), size_(n) {}   // {} 零初始化

    ~IntBox() { delete[] data_; }

    // 禁拷贝（今天专注运算符；三件套在 Day 66 已毕业考）
    IntBox(const IntBox&) = delete;
    IntBox& operator=(const IntBox&) = delete;

    int& operator[](std::size_t i) {             // 可写下标
        return data_[i];
    }

    const int& operator[](std::size_t i) const { // 只读下标（Day 66 双版的复习）
        return data_[i];
    }

    std::size_t size() const { return size_; }
};

class Multiplier {                               // () 的主场：仿函数（函数对象）
private:
    double factor_;

public:
    explicit Multiplier(double f) : factor_(f) {}

    double operator()(double x) const {          // 对象像函数一样被"调用"
        return x * factor_;
    }
};

class Config {                                   // -> 的主场：智能指针的底层机制
private:
    std::string host_ = "localhost";
    int port_ = 8080;

public:
    const std::string& host() const { return host_; }
    int port() const { return port_; }
};

// 简易智能指针（演示 -> 重载；真实版看 Day 71 的 unique_ptr）
template <typename T>
class SmartPtr {
private:
    T* ptr_;

public:
    explicit SmartPtr(T* p) : ptr_(p) {}
    ~SmartPtr() { delete ptr_; }

    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;

    T* operator->() {                            // -> 把成员访问转发给内部指针
        return ptr_;
    }

    T& operator*() {                             // * 解引用
        return *ptr_;
    }
};

int main() {
    std::cout << "== []：可读可写 ==" << std::endl;
    IntBox box(5);
    box[0] = 10;
    box[4] = 50;
    for (std::size_t i = 0; i < box.size(); ++i) {
        std::cout << "  box[" << i << "] = " << box[i] << std::endl;
    }

    std::cout << "\n== ()：仿函数 ==" << std::endl;
    Multiplier triple(3.0);
    std::cout << "  triple(7) = " << triple(7) << std::endl;       // 对象调用！
    std::cout << "  triple(2.5) = " << triple(2.5) << std::endl;

    std::cout << "\n== -> 和 *：智能指针的底层 ==" << std::endl;
    SmartPtr<Config> cfg(new Config());
    std::cout << "  cfg->host() = " << cfg->host() << std::endl;   // -> 转发
    std::cout << "  cfg->port() = " << cfg->port() << std::endl;
    std::cout << "  (*cfg).port() = " << (*cfg).port() << std::endl; // * 解引用后 .
    return 0;
}
