#include <cstdint>
#include <cstring>
#include <iostream>

// 字节序（了解级）：同一 4 字节 int 的两种读法
int main() {
    uint32_t x = 0x01020304;              // 高位字节 01，低位字节 04
    unsigned char* p = reinterpret_cast<unsigned char*>(&x);

    std::cout << "== 内存里的字节排列 ==" << std::endl;
    std::cout << "  x = 0x01020304 在内存中的字节（从低地址到高地址）：" << std::endl;
    std::cout << "  ";
    for (int i = 0; i < 4; ++i) {
        std::cout << std::hex << std::uppercase << static_cast<int>(p[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    if (p[0] == 0x04) {
        std::cout << "  低地址放着低位字节（04）→ 小端（little-endian）" << std::endl;
    } else {
        std::cout << "  低地址放着高位字节（01）→ 大端（big-endian）" << std::endl;
    }
    std::cout << "  （本机是 Apple Silicon（ARM）——通常小端）" << std::endl;

    std::cout << "\n== 为什么你平时不用管 ==" << std::endl;
    std::cout << "  同一台机器上写读一致——C++ 抽象掉了字节序" << std::endl;
    int y = 258;                           // 0x0102
    int z = 0;
    memcpy(&z, &y, sizeof(y));            // 按字节拷——两种字节序都对
    std::cout << "  memcpy 拷 int：" << y << " → " << z << "（正常）" << std::endl;

    std::cout << "\n== 什么时候必须管 ==" << std::endl;
    std::cout << "  网络传输/文件格式/硬件协议——字节流跨机器时" << std::endl;
    std::cout << "  （网络字节序规定大端——htons/htonl 函数族的由来）" << std::endl;
    return 0;
}
