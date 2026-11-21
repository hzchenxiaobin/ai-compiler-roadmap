#include <cstring>
#include <iostream>

// 修复版：手写深拷贝（new 新块 + memcpy 抄内容）
struct DeepBuffer {
    char* data;
    int len;

    void init(const char* s) {
        len = static_cast<int>(strlen(s)) + 1;
        data = new char[len];
        memcpy(data, s, len);
    }

    DeepBuffer clone() const {                 // 手写深拷贝（struct 版；类版的拷贝构造 Day 55 正课）
        DeepBuffer c;
        c.len = len;
        c.data = new char[len];                // ① 新开内存
        memcpy(c.data, data, len);             // ② 抄内容
        return c;
    }

    void destroy() {
        delete[] data;
        data = nullptr;
    }
};

int main() {
    std::cout << "== 深拷贝版 ==" << std::endl;
    DeepBuffer a;
    a.init("hello");
    std::cout << "  a.data = " << static_cast<void*>(a.data) << "：\"" << a.data << "\"" << std::endl;

    DeepBuffer b = a.clone();
    std::cout << "  b.data = " << static_cast<void*>(b.data) << std::endl;
    std::cout << "  两块内存" << (a.data != b.data ? "不同——各自独立 ✔" : "相同（异常）") << std::endl;

    std::cout << "\n== 独立性验证 ==" << std::endl;
    b.data[0] = 'X';
    std::cout << "  改 b 后：a = \"" << a.data << "\"，b = \"" << b.data << "\"（互不影响）" << std::endl;

    std::cout << "\n== 各自安全释放 ==" << std::endl;
    a.destroy();
    b.destroy();                          // 两块内存各自归还——没有 double free
    std::cout << "  a、b 都已释放（ASan 版零报错）——深拷贝的出路" << std::endl;
    return 0;
}
