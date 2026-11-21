#include <cstring>
#include <iostream>

// Day 41：深拷贝 vs 浅拷贝——含指针成员的结构体的坑（计划指定练习）
// 用最简的 struct 复现 double-free（第 10 周三法则的地基课）
struct RawBuffer {
    char* data;
    int len;
};

int main() {
    std::cout << "== 构造源对象 ==" << std::endl;
    RawBuffer a;
    a.len = 6;
    a.data = new char[a.len]{};
    memcpy(a.data, "hello", 6);
    std::cout << "  a.data = " << static_cast<void*>(a.data) << "：\"" << a.data << "\"" << std::endl;

    std::cout << "\n== 默认拷贝（浅拷贝现场） ==" << std::endl;
    RawBuffer b = a;                     // struct 默认逐成员拷贝：指针抄的是地址！
    std::cout << "  b.data = " << static_cast<void*>(b.data) << std::endl;
    std::cout << "  a.data 和 b.data " << (a.data == b.data ? "相同——两根指针一块内存！" : "不同") << std::endl;

    std::cout << "\n== 改 b 看 a（共享的证据） ==" << std::endl;
    b.data[0] = 'X';
    std::cout << "  改 b.data[0]='X' 后：a.data = \"" << a.data << "\"（a 也变了！）" << std::endl;

    std::cout << "\n== 析构的连环雷 ==" << std::endl;
    delete[] a.data;
    std::cout << "  delete[] a.data 完成——b.data 现在指向已释放内存" << std::endl;
    // delete[] b.data;                  // ✘ 再释放同一块 = double free（取消注释崩溃/ASan 现形）
    std::cout << "  （b 的 delete 被注释掉——否则 double free；但这本身又泄漏了语义）" << std::endl;
    std::cout << "  这就是'坑'：释放也错、不释放也错——浅拷贝没有出路" << std::endl;
    return 0;
}
