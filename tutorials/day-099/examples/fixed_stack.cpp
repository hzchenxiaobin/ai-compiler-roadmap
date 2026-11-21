#include <cassert>
#include <iostream>
#include <string>

// ===== Day 99：非类型模板参数 + 默认模板参数——固定容量 Stack（计划指定） =====
template <typename T, int MaxSize = 100>     // 类型参数 T + 非类型参数 MaxSize（默认 100）
class FixedStack {
private:
    T data_[MaxSize];                        // 容量是编译期常量——数组直接开！
    int top_;                                // 栈顶下标（-1 = 空）
public:
    FixedStack() : top_(-1) {}

    bool push(const T& value) {
        if (top_ + 1 >= MaxSize) { return false; }   // 满——拒绝（不扩容！）
        data_[++top_] = value;
        return true;
    }

    bool pop() {
        if (top_ < 0) { return false; }
        --top_;
        return true;
    }

    T& top() { return data_[top_]; }
    bool empty() const { return top_ < 0; }
    bool full() const { return top_ + 1 >= MaxSize; }
    static constexpr int capacity() { return MaxSize; }   // 编译期能查容量
};

int main() {
    std::cout << "== 计划指定：Stack<int, 100> ==" << std::endl;
    FixedStack<int, 100> s100;
    std::cout << "  capacity = " << FixedStack<int, 100>::capacity() << std::endl;
    for (int i = 1; i <= 5; ++i) { s100.push(i * 11); }
    std::cout << "  push 5 个后 top = " << s100.top() << std::endl;
    s100.pop();
    std::cout << "  pop 后 top = " << s100.top() << std::endl;

    std::cout << "\n== 非类型参数的执法：小容量满栈 ==" << std::endl;
    FixedStack<int, 3> s3;                   // 容量 3 的另一个类！
    std::cout << "  push 1：" << (s3.push(1) ? "成功" : "失败") << std::endl;
    std::cout << "  push 2：" << (s3.push(2) ? "成功" : "失败") << std::endl;
    std::cout << "  push 3：" << (s3.push(3) ? "成功" : "失败") << std::endl;
    bool r4 = s3.push(4);
    std::cout << "  push 4：" << (r4 ? "？" : "失败（满了！）") << std::endl;
    std::cout << "  full() = " << (s3.full() ? "true" : "false") << std::endl;

    std::cout << "\n== 默认参数：不写容量就是 100 ==" << std::endl;
    FixedStack<std::string> sd;              // MaxSize 用默认 100
    std::cout << "  FixedStack<string> 的容量 = " << FixedStack<std::string>::capacity()
              << "（没写第二个参数——默认生效）" << std::endl;
    sd.push("默认");
    sd.push("参数");
    std::cout << "  top = " << sd.top() << std::endl;

    std::cout << "\n== 每个容量是独立的类 ==" << std::endl;
    std::cout << "  Stack<int,3> 和 Stack<int,100> 是两个不同的类" << std::endl;
    std::cout << "  sizeof(FixedStack<int,3>) = " << sizeof(FixedStack<int, 3>) << std::endl;
    std::cout << "  sizeof(FixedStack<int,100>) = " << sizeof(FixedStack<int, 100>) << std::endl;
    std::cout << "  （容量进类型——sizeof 差 97 个 int）" << std::endl;

    std::cout << "\n== 和运行期限定的对照 ==" << std::endl;
    std::cout << "  非类型参数：编译期定容量——数组直接开（栈上零分配）" << std::endl;
    std::cout << "  运行期 new[]：灵活但要管内存（Day 98 的 MyVector 路线）" << std::endl;
    std::cout << "  （std::array<int,N> 同款设计——N 是非类型参数）" << std::endl;
    return 0;
}
