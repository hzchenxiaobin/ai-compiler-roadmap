#include <iostream>
#include <vector>

// 修复版：组合——Stack 有一个 vector（私用），只暴露 Stack 该有的接口
template <typename T>
class GoodStack {
private:
    std::vector<T> data_;                       // 组合：私用零件，外界看不见

public:
    void push(const T& v) { data_.push_back(v); }

    T pop() {
        T v = data_.back();
        data_.pop_back();
        return v;
    }

    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
    // 没有 insert / operator[] / clear——Stack 不该有的操作，物理上不存在
};

int main() {
    GoodStack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    std::cout << "pop = " << s.pop() << std::endl;
    std::cout << "size = " << s.size() << "，empty = " << (s.empty() ? "是" : "否") << std::endl;

    // s.insert(s.begin(), 99);    // ✘ 编译错误：GoodStack 没有 insert
    // s[0] = 42;                  // ✘ 编译错误：GoodStack 没有 operator[]
    std::cout << "（insert / s[0] 被编译器拦下——不变量锁死）" << std::endl;
    return 0;
}
