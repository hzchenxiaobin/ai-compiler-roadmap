#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// ===== Day 98：类模板——MyVector<T>（计划指定：动态数组改模板类） =====
// 类外定义成员函数的完整语法示范
template <typename T>
class MyVector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

public:
    MyVector();                                        // 类内声明
    ~MyVector();
    MyVector(const MyVector& other);                   // 拷贝构造（Day 57 三法则）
    MyVector& operator=(const MyVector& other);        // 拷贝赋值

    void push_back(const T& value);
    std::size_t size() const { return size_; }         // 短小函数类内定义
    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    using iterator = T*;                                // 裸指针当迭代器（Day 88 随机访问）
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
};

// ===== 类外定义：每都要 template <typename T> 打头 =====
template <typename T>
MyVector<T>::MyVector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
MyVector<T>::~MyVector() { delete[] data_; }

template <typename T>
MyVector<T>::MyVector(const MyVector& other)
    : data_(new T[other.size_]), size_(other.size_), capacity_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) { data_[i] = other.data_[i]; }
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this == &other) { return *this; }
    delete[] data_;
    data_ = new T[other.size_];
    size_ = capacity_ = other.size_;
    for (std::size_t i = 0; i < size_; ++i) { data_[i] = other.data_[i]; }
    return *this;
}

template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;   // 倍增（Day 47/79）
        T* new_data = new T[new_cap];
        for (std::size_t i = 0; i < size_; ++i) { new_data[i] = data_[i]; }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }
    data_[size_++] = value;
}

// 值类型也要有 operator<<（print_any 同款 duck typing）
template <typename T>
void dump(const char* label, MyVector<T>& v) {
    std::cout << "  " << label << "：";
    for (const auto& x : v) { std::cout << x << ' '; }
    std::cout << "（size=" << v.size() << "）" << std::endl;
}

int main() {
    std::cout << "== MyVector<int>：整型版 ==" << std::endl;
    MyVector<int> vi;
    for (int i = 1; i <= 5; ++i) { vi.push_back(i * 10); }
    dump("vi", vi);
    std::cout << "  vi[2] = " << vi[2] << "（下标访问）" << std::endl;

    std::cout << "\n== MyVector<std::string>：字符串版（同款代码！） ==" << std::endl;
    MyVector<std::string> vs;
    vs.push_back("hello");
    vs.push_back("template");
    vs.push_back("world");
    dump("vs", vs);

    std::cout << "\n== 三法则拷贝（Day 57 的模板版） ==" << std::endl;
    MyVector<int> vc = vi;                              // 拷贝构造
    vi.push_back(999);                                  // 原对象追加
    dump("vc（拷贝）", vc);
    dump("vi（原，追加后）", vi);
    std::cout << "  （各自独立——深拷贝 ✔）" << std::endl;

    MyVector<int> va;
    va.push_back(7);
    va = vi;                                            // 拷贝赋值
    dump("va（赋值后）", va);

    std::cout << "\n== 迭代器（裸指针——随机访问） ==" << std::endl;
    std::cout << "  范围 for（糖衣）：";
    for (int x : vc) { std::cout << x << ' '; }
    std::cout << std::endl;
    std::cout << "  指针算术：*(vc.begin() + 2) = " << *(vc.begin() + 2) << std::endl;

    std::cout << "\n== 类外定义的语法要点 ==" << std::endl;
    std::cout << "  每个类外定义都要 template <typename T> 打头" << std::endl;
    std::cout << "  返回类型写 MyVector<T>::（带尖括号）" << std::endl;
    std::cout << "  类内短函数直接定义更省事（size/[] 那样）" << std::endl;
    return 0;
}
