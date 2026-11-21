#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// ===== Day 97：函数模板——实参推导与泛型 myMax/mySum（计划指定） =====
template <typename T>
T myMax(const T& a, const T& b) {
    return (a > b) ? a : b;
}

// 两个类型参数的版本（异型比较）
template <typename T, typename U>
T myMax2(const T& a, const U& b) {
    return (a > b) ? a : T(b);
}

// 泛型 mySum（容器版——迭代器做接口，Day 80 通用语言的模板化）
template <typename It>
auto mySum(It first, It last) {
    typename std::iterator_traits<It>::value_type total{};    // 零初始化
    for (It it = first; it != last; ++it) { total += *it; }
    return total;
}

// 泛型 print_any（万能打印——模板的 duck typing）
template <typename T>
void print_any(const T& x) {
    std::cout << x;
}

int main() {
    std::cout << "== 模板实参推导（不用写 <int>） ==" << std::endl;
    std::cout << "  myMax(3, 7) = " << myMax(3, 7) << "（推导 T=int）" << std::endl;
    std::cout << "  myMax(2.5, 1.5) = " << myMax(2.5, 1.5) << "（推导 T=double）" << std::endl;
    std::cout << "  myMax(\"apple\", \"banana\") 按字典序……" << std::endl;
    std::cout << "  myMax('a', 'z') = " << myMax('a', 'z') << "（推导 T=char）" << std::endl;
    std::cout << "  （显式指定也可以：myMax<int>(3, 7)——通常不必）" << std::endl;

    std::cout << "\n== 推导失败的经典场景 ==" << std::endl;
    std::cout << "  myMax(3, 7.5) → 编译错（T 推不出唯一类型）" << std::endl;
    std::cout << "  解决①：显式 myMax<double>(3, 7.5)" << std::endl;
    std::cout << "  解决②：双参数版本 myMax2（T/U 各推各的）" << std::endl;
    std::cout << "  myMax2(3, 7.5) = " << myMax2(3, 7.5) << "（返回 T=int——注意 7.5 被截！）" << std::endl;

    std::cout << "\n== 泛型 mySum（容器通吃） ==" << std::endl;
    std::vector<int> vi = {1, 2, 3, 4, 5};
    std::vector<double> vd = {1.5, 2.5, 3.0};
    std::cout << "  mySum(int 数组) = " << mySum(vi.begin(), vi.end()) << std::endl;
    std::cout << "  mySum(double 数组) = " << mySum(vd.begin(), vd.end()) << std::endl;
    std::cout << "  mySum(部分区间 [1,4)) = " << mySum(vi.begin() + 1, vi.begin() + 4) << std::endl;
    std::cout << "  （同一个函数模板——容器和区间都吃）" << std::endl;

    std::cout << "\n== print_any：模板的 duck typing ==" << std::endl;
    print_any(42);       std::cout << ' ';
    print_any(3.14);     std::cout << ' ';
    print_any("hello");  std::cout << ' ';
    print_any('x');      std::cout << std::endl;
    std::cout << "  （T 是什么都能印——只要它有 operator<<）" << std::endl;

    std::cout << "\n== 模板是编译期的代码生成器 ==" << std::endl;
    std::cout << "  myMax(3,7) 和 myMax(2.5,1.5) 是两个不同的函数实例" << std::endl;
    std::cout << "  （编译器按用到的类型各刻一份——零运行时开销）" << std::endl;
    return 0;
}
