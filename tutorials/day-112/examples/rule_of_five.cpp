#include <cstring>
#include <iostream>
#include <utility>

// Day 112：五法则完整版 + =default / =delete——Rule.of.Five 探针（计划指定：MyVector 五法则）
class Probe {
public:
    Probe()                          { std::cout << "  [默认构造]\n"; }
    Probe(const Probe&)              { std::cout << "  [拷贝构造]\n"; }
    Probe(Probe&&) noexcept          { std::cout << "  [移动构造]\n"; }
    Probe& operator=(const Probe&)   { std::cout << "  [拷贝赋值]\n"; return *this; }
    Probe& operator=(Probe&&) noexcept { std::cout << "  [移动赋值]\n"; return *this; }
    ~Probe()                         { std::cout << "  [析构]\n"; }
};

// ===== MyVector 完成五法则（Day 98 的模板类 + 移动两件 + =default 管理） =====
template <typename T>
class MyVector {
public:
    MyVector() : data_(nullptr), size_(0), cap_(0) {}                       // 默认构造

    explicit MyVector(std::size_t n) : data_(new T[n]), size_(n), cap_(n) {} // 定长构造

    MyVector(const MyVector& o)                                             // ①拷贝构造
        : data_(new T[o.size_]), size_(o.size_), cap_(o.size_) {
        std::cout << "  [五法则①拷贝构造]\n";
        for (std::size_t i = 0; i < size_; ++i) { data_[i] = o.data_[i]; }
    }

    MyVector(MyVector&& o) noexcept                                         // ②移动构造
        : data_(o.data_), size_(o.size_), cap_(o.cap_) {
        std::cout << "  [五法则②移动构造]\n";
        o.data_ = nullptr; o.size_ = o.cap_ = 0;                            // 掏空源
    }

    MyVector& operator=(const MyVector& o) {                                // ③拷贝赋值
        std::cout << "  [五法则③拷贝赋值]\n";
        if (this == &o) { return *this; }
        delete[] data_;
        data_ = new T[o.size_]; size_ = cap_ = o.size_;
        for (std::size_t i = 0; i < size_; ++i) { data_[i] = o.data_[i]; }
        return *this;
    }

    MyVector& operator=(MyVector&& o) noexcept {                            // ④移动赋值
        std::cout << "  [五法则④移动赋值]\n";
        if (this == &o) { return *this; }
        delete[] data_;
        data_ = o.data_; size_ = o.size_; cap_ = o.cap_;
        o.data_ = nullptr; o.size_ = o.cap_ = 0;
        return *this;
    }

    ~MyVector() {                                                           // ⑤析构
        std::cout << "  [五法则⑤析构]（size=" << size_ << "）\n";
        delete[] data_;
    }

    void push_back(const T& v) {
        if (size_ == cap_) { grow(); }
        data_[size_++] = v;
    }
    std::size_t size() const { return size_; }
    T& operator[](std::size_t i) { return data_[i]; }

    // =delete 的经典应用：禁止按值传/存（强制引用——防切片的轻量版）
    // void take_by_value(MyVector v);  ← 这个声明没人写，但用 =delete 可以主动禁一类操作

private:
    void grow() {
        std::size_t nc = (cap_ == 0) ? 1 : cap_ * 2;
        T* nd = new T[nc];
        for (std::size_t i = 0; i < size_; ++i) { nd[i] = std::move(data_[i]); }   // 搬家用移动！
        delete[] data_;
        data_ = nd; cap_ = nc;
    }
    T* data_;
    std::size_t size_, cap_;
};

// =default / =delete 的演示类
class NoCopy {
public:
    NoCopy() = default;                          // 显式要默认（比如有其他 =delete 时）
    NoCopy(const NoCopy&) = delete;              // ①禁拷贝构造
    NoCopy& operator=(const NoCopy&) = delete;   // ②禁拷贝赋值
    NoCopy(NoCopy&&) noexcept = default;         // ③移动用编译器默认（偷成员）
    NoCopy& operator=(NoCopy&&) noexcept = default;
    ~NoCopy() = default;
    int value = 42;
};

int main() {
    std::cout << "== 五法则全景：Probe 的六件 ==" << std::endl;
    Probe a;
    Probe b = a;              // 拷贝构造
    Probe c = std::move(a);   // 移动构造
    b = c;                    // 拷贝赋值
    c = std::move(b);         // 移动赋值
    std::cout << "  （六件齐——五法则的'五' + 默认构造）" << std::endl;

    std::cout << "\n== 计划指定：MyVector 完成五法则 ==" << std::endl;
    MyVector<int> v1;
    v1.push_back(1); v1.push_back(2); v1.push_back(3);
    MyVector<int> v2 = v1;                 // ①拷贝构造
    MyVector<int> v3 = std::move(v1);      // ②移动构造（v1 被掏空）
    MyVector<int> v4;
    v4 = v2;                               // ③拷贝赋值
    MyVector<int> v5;
    v5 = std::move(v3);                    // ④移动赋值
    std::cout << "  v5 还有 " << v5.size() << " 个元素（一路传下来）" << std::endl;
    std::cout << "  （v1/v3 已掏空——size=0，析构安全）" << std::endl;

    std::cout << "\n== =delete：不可拷贝的类型 ==" << std::endl;
    NoCopy n1;
    std::cout << "  NoCopy n1; n1.value = " << n1.value << std::endl;
    NoCopy n2 = std::move(n1);             // 移动 ✔（=default）
    (void)n2;
    std::cout << "  NoCopy n2 = move(n1); ✔（移动默认）" << std::endl;
    // NoCopy n3 = n2;    // ✘ 编译错！拷贝构造被 =delete
    std::cout << "  NoCopy n3 = n2;  → 编译错（拷贝被 =delete）" << std::endl;
    std::cout << "  （独占语义的类型都这么写——智能指针/线程/文件句柄）" << std::endl;
    return 0;
}
