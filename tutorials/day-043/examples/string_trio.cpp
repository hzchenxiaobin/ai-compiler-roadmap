#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

// Day 43：std::string / string_view / C 字符串——三种字符串遍历对比（计划指定练习）
int main() {
    // 三种"字符串"的同一段话
    const char* cstr = "hello world";                    // C 字符串（常量区 + '\0' 结尾）
    char stack_buf[32];
    strcpy(stack_buf, cstr);                             // 拷进栈数组（可改版）
    std::string str = "hello world";                     // std::string（自带长度、可增长）
    std::string_view sv = str;                           // 视图（C++17：只借不看管）

    std::cout << "== 遍历对比 ==" << std::endl;
    std::cout << "C 字符串（strlen 先数一遍再走）：" << std::endl;
    std::cout << "  ";
    for (std::size_t i = 0; i < strlen(stack_buf); ++i) {
        std::cout << stack_buf[i] << ' ';
    }
    std::cout << "（长度 " << strlen(stack_buf) << "）" << std::endl;

    std::cout << "std::string（size() 直接给）：" << std::endl;
    std::cout << "  ";
    for (std::size_t i = 0; i < str.size(); ++i) {
        std::cout << str[i] << ' ';
    }
    std::cout << "（长度 " << str.size() << "）" << std::endl;

    std::cout << "string_view（也是 O(1) 取长度）：" << std::endl;
    std::cout << "  ";
    for (std::size_t i = 0; i < sv.size(); ++i) {
        std::cout << sv[i] << ' ';
    }
    std::cout << "（长度 " << sv.size() << "）" << std::endl;

    std::cout << "\n== 内部表示的差别 ==" << std::endl;
    std::cout << "  cstr        指向常量区 " << static_cast<const void*>(cstr) << std::endl;
    std::cout << "  stack_buf   栈数组     " << static_cast<const void*>(stack_buf) << std::endl;
    std::cout << "  str.data()  堆（或 SSO）" << static_cast<const void*>(str.data()) << std::endl;
    std::cout << "  sv.data()   借自 str   " << static_cast<const void*>(sv.data())
              << (sv.data() == str.data() ? "（同址——纯借看！）" : "") << std::endl;
    std::cout << "  sizeof: cstr=" << sizeof(cstr) << " stack_buf=" << sizeof(stack_buf)
              << " str=" << sizeof(str) << " sv=" << sizeof(sv) << std::endl;

    std::cout << "\n== C 字符串互转 ==" << std::endl;
    const char* back = str.c_str();                      // string → C 字符串
    std::string again = back;                            // C 字符串 → string
    std::cout << "  str.c_str() → " << back << "，转回 string → " << again << std::endl;
    std::cout << "  （互转零拷贝？c_str() 是直通；反向要拷——strlen+memcpy）" << std::endl;

    std::cout << "\n== string_view 的悬空陷阱 ==" << std::endl;
    std::string_view bad;
    {
        std::string temp = "我是临时";
        bad = temp;                                      // 借了临时的
    }                                                    // temp 析构——bad 悬空！
    std::cout << "  临时 string 已析构，bad.size() = " << bad.size()
              << "（数据已死——再读是 UB）" << std::endl;
    std::cout << "  （string_view 不管生死——只借看是它的本质也是它的险）" << std::endl;
    return 0;
}
