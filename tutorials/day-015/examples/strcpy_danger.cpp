#include <iostream>
#include <cstring>

// strcpy 的危险：往小杯子里倒大杯水
int main() {
    char big[] = "0123456789abcdefghijklmn";   // 24 个字符 + '\0'，共 25 字节
    char small[8];                              // 只有 8 个字节的杯子

    std::cout << "源串长度 = " << std::strlen(big) << std::endl;
    std::cout << "目标杯子大小 = " << sizeof(small) << " 字节" << std::endl;
    std::cout << "现在 strcpy(small, big)……" << std::endl;

    std::strcpy(small, big);                    // 溢出！写入越过杯子边缘
    std::cout << "程序还在跑（普通编译下没有报错）" << std::endl;
    return 0;
}
