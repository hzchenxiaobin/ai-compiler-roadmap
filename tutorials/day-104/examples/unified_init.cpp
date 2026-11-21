#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

// Day 104：统一初始化 {} 与 initializer_list——窄化报错观察（计划指定）
class Point {
public:
    int x, y;
    Point() : x(0), y(0) {}                            // 默认构造（most vexing 用）
    Point(int a, int b) : x(a), y(b) {}
};

void take_list(std::initializer_list<int> lst) {
    std::cout << "  收到 " << lst.size() << " 个：";
    for (int v : lst) { std::cout << v << ' '; }
    std::cout << std::endl;
}

int main() {
    std::cout << "== {} 的五种用法（统一初始化） ==" << std::endl;
    int a{42};                                          // 基本类型
    double d{3.14};
    int arr[]{1, 2, 3};                                 // 数组
    Point p{10, 20};                                    // 类对象（调构造）
    std::vector<int> v{1, 2, 3, 4};                     // 容器（init-list 构造）
    std::cout << "  a=" << a << " d=" << d << " arr[2]=" << arr[2]
              << " p=(" << p.x << "," << p.y << ") v.size=" << v.size() << std::endl;
    std::cout << "  （一个语法 {} 通吃五种场景——“统一”的含义）" << std::endl;

    std::cout << "\n== 最想当的解析坑（vector 专属） ==" << std::endl;
    std::vector<int> v1(3);                             // (3)：三个 0！
    std::vector<int> v2{3};                             // {3}：一个 3！
    std::cout << "  vector<int> v1(3) → size = " << v1.size()
              << "（三个 0——“大小”构造）" << std::endl;
    std::cout << "  vector<int> v2{3} → size = " << v2.size()
              << "（一个 3——列表构造）" << std::endl;
    std::cout << "  （() 和 {} 语义不同——vector 的历史坑）" << std::endl;

    std::cout << "\n== 计划指定：窄化转换报错观察 ==" << std::endl;
    std::cout << "  int x1 = src;    （允许——隐式截断成 3）" << std::endl;
    double src = 3.14;
    int x1 = src;
    std::cout << "  x1 = " << x1 << "（赋值初始化不吭声）" << std::endl;
    std::cout << "  int x2{3.14};    （编译错！{} 禁止窄化）" << std::endl;
    // int x2{3.14};   // ✘ error: type 'double' cannot be narrowed to 'int'
    std::cout << "  （{} 的红线：double→int、long→int 这类丢信息的转换直接拒收）" << std::endl;
    std::cout << "  int ok{42};      （安全——同宽度不窄化 ✔）" << std::endl;
    int ok{42};
    (void)ok;

    std::cout << "\n== initializer_list：可变参数的糖 ==" << std::endl;
    take_list({1, 2, 3});
    take_list({10, 20, 30, 40});
    take_list({});
    std::cout << "  （{...} 直接传——函数按列表收）" << std::endl;

    std::cout << "\n== 自定义类吃 initializer_list ==" << std::endl;
    class Sum {
        int total_;
    public:
        Sum(std::initializer_list<int> lst) : total_(0) {
            for (int v : lst) { total_ += v; }
        }
        int value() const { return total_; }
    };
    Sum s{1, 2, 3, 4, 5};
    std::cout << "  Sum s{1,2,3,4,5} → " << s.value()
              << "（构造函数收列表——15）" << std::endl;

    std::cout << "\n== {} 的保留节目：防 most vexing parse ==" << std::endl;
    std::cout << "  Point q();   ← 被解析成函数声明！（不是对象）" << std::endl;
    Point q{};
    std::cout << "  Point q{};   ← 确定是对象：q.x = " << q.x
              << ", q.y = " << q.y << "（{} 消歧）" << std::endl;
    return 0;
}
