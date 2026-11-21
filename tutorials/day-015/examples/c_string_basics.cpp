#include <iostream>
#include <cstring>

// C 风格字符串的第一课：'\0' 终结符、sizeof 与 strlen 的区别
int main() {
    char greet[6] = {'h', 'e', 'l', 'l', 'o', '\0'};   // 手工摆出终结符
    char shortcut[] = "hi";                             // 字面量自动带 '\0'

    std::cout << "greet 内容：" << greet << std::endl;
    std::cout << "sizeof(greet) = " << sizeof(greet) << "（数组占的字节数）" << std::endl;
    std::cout << "strlen(greet) = " << std::strlen(greet) << "（数到 '\\0' 为止的长度）" << std::endl;
    std::cout << "sizeof(shortcut) = " << sizeof(shortcut) << "，strlen(shortcut) = "
              << std::strlen(shortcut) << std::endl;

    // 逐字符走一遍，亲眼看看 '\0' 在哪
    for (int i = 0; i < 6; ++i) {
        std::cout << "greet[" << i << "] = '" << greet[i] << "' (编码 " << (int)greet[i] << ")"
                  << std::endl;
    }

    greet[0] = 'H';                                     // 字符数组可以改
    std::cout << "改完后：" << greet << std::endl;
    return 0;
}
