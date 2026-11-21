#include <cstdint>
#include <iostream>
#include <string>

// ===== Day 105：nullptr / enum class / using / constexpr——四件套替换旧写法（计划指定） =====

// ① nullptr：类型安全的空指针
void take_ptr(int* p)  { std::cout << "  take_ptr(int*)：指针重载" << std::endl; }
void take_ptr(int v)   { std::cout << "  take_ptr(int)：整数重载" << std::endl; }

// ② enum class：带作用域的强类型枚举
enum class Color { Red, Green, Blue };
enum class Traffic { Red, Yellow, Green };            // 同名 Red 不打架（各有作用域！）
enum OldColor { OLD_RED, OLD_GREEN };                  // 旧式（无作用域——对比用）

// ③ using 别名（typedef 的现代版）
using IntList = std::string;                           // 演示用别名（实际可以是任何类型）
using Callback = void (*)(int);                        // 函数指针别名——typedef 时代的噩梦
// typedef void (*OldCallback)(int);                  // 旧写法（读不懂系列）

// ④ constexpr：编译期常量/函数
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}
constexpr int kMax = 100;                              // 编译期常量（替代宏/const）

int main() {
    std::cout << "== ① nullptr：类型安全的空 ==" << std::endl;
    take_ptr(nullptr);                                 // 明确选指针重载
    take_ptr(0);                                       // 0 是 int → 整数重载
    int* p = nullptr;                                  // 现代空指针写法
    std::cout << "  p = nullptr; p == nullptr → " << (p == nullptr ? "true" : "false") << std::endl;
    std::cout << "  （NULL 是整数宏——重载会选错；nullptr 是指针类型）" << std::endl;

    std::cout << "\n== ② enum class：作用域 + 强类型 ==" << std::endl;
    Color c = Color::Red;                              // 必须带作用域 Color::
    (void)c;
    Traffic t = Traffic::Red;                          // 同名 Red 各在各家——不打架！
    (void)t;
    std::cout << "  Color::Red 和 Traffic::Red 共存 ✔（旧式 enum 全局就撞名）" << std::endl;
    std::cout << "  static_cast<int>(Color::Green) = " << static_cast<int>(Color::Green) << std::endl;
    // int x = c;  // ✘ 编译错！enum class 不隐式转 int（强类型）
    std::cout << "  （不隐式转 int——要转必须显式 static_cast）" << std::endl;
    int old_x = OLD_RED;                               // 旧式：隐式转 int（松）
    std::cout << "  旧式 OLD_RED 隐式转 int = " << old_x << "（无保护）" << std::endl;

    std::cout << "\n== ③ using 别名：typedef 的现代版 ==" << std::endl;
    IntList s = "别名也能当类型用";
    std::cout << "  IntList s = " << s << std::endl;
    std::cout << "  Callback 是函数指针别名——typedef 时代的噩梦变可读" << std::endl;
    std::cout << "  （模板别名 using Vec<T> = vector<T,Alloc<T>> 更是 typedef 做不到）" << std::endl;

    std::cout << "\n== ④ constexpr：编译期计算 ==" << std::endl;
    constexpr int f5 = factorial(5);                   // 编译期算好（120 直接进指令）
    std::cout << "  constexpr factorial(5) = " << f5 << std::endl;
    int n = 6;                                         // 运行期值
    int f6 = factorial(n);                             // 也能运行期调用（同函数两用）
    std::cout << "  运行期调用 factorial(6) = " << f6 << std::endl;
    std::cout << "  kMax = " << kMax << "（替代 #define MAX——有类型有作用域）" << std::endl;
    int arr[factorial(4)];                             // 编译期值能当数组大小！
    std::cout << "  int arr[factorial(4)] → 大小 " << sizeof(arr) / sizeof(arr[0]) << "（编译期定的）" << std::endl;

    std::cout << "\n== 四件套的替换清单（计划指定） ==" << std::endl;
    std::cout << "  NULL        → nullptr" << std::endl;
    std::cout << "  enum {…}    → enum class [名] {…}" << std::endl;
    std::cout << "  typedef …   → using … = …" << std::endl;
    std::cout << "  #define N   → constexpr int N" << std::endl;
    std::cout << "  （四对替换——旧代码现代化的第一波）" << std::endl;
    return 0;
}
