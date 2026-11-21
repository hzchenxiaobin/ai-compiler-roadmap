#include <cstdint>
#include <iostream>

// Day 44：alignof 打印常见类型的对齐值（计划指定练习）
struct Packed1 { char a; };                // 1 字节成员
struct Packed2 { char a; char b; };
struct MixA { char a; int b; };            // 1 + 填充 3 + 4
struct MixB { int b; char a; };            // 4 + 1 + 填充 3
struct Big { char a; double d; int i; };   // 1 + 填7 + 8 + 4 + 填4

int main() {
    std::cout << "== 内建类型的对齐值（arm64 实测） ==" << std::endl;
    std::cout << "  alignof(char)    = " << alignof(char) << std::endl;
    std::cout << "  alignof(short)   = " << alignof(short) << std::endl;
    std::cout << "  alignof(int)     = " << alignof(int) << std::endl;
    std::cout << "  alignof(long)    = " << alignof(long) << std::endl;
    std::cout << "  alignof(double)  = " << alignof(double) << std::endl;
    std::cout << "  alignof(void*)   = " << alignof(void*) << std::endl;

    std::cout << "\n== struct 的对齐与大小（成员顺序的影响） ==" << std::endl;
    std::cout << "  Packed1{char}          : align=" << alignof(Packed1) << " size=" << sizeof(Packed1) << std::endl;
    std::cout << "  Packed2{char,char}     : align=" << alignof(Packed2) << " size=" << sizeof(Packed2) << std::endl;
    std::cout << "  MixA{char,int}         : align=" << alignof(MixA) << " size=" << sizeof(MixA) << "（1+填3+4）" << std::endl;
    std::cout << "  MixB{int,char}         : align=" << alignof(MixB) << " size=" << sizeof(MixB) << "（4+1+填3）" << std::endl;
    std::cout << "  Big{char,double,int}   : align=" << alignof(Big) << " size=" << sizeof(Big) << "（1+填7+8+4+填4）" << std::endl;

    std::cout << "\n== 填充的地址证据（Big 内部） ==" << std::endl;
    Big b;
    std::cout << "  &b.a (char)   = " << static_cast<void*>(&b.a)
              << "，偏移 " << reinterpret_cast<char*>(&b.a) - reinterpret_cast<char*>(&b) << std::endl;
    std::cout << "  &b.d (double) = " << &b.d
              << "，偏移 " << reinterpret_cast<char*>(&b.d) - reinterpret_cast<char*>(&b) << std::endl;
    std::cout << "  &b.i (int)    = " << &b.i
              << "，偏移 " << reinterpret_cast<char*>(&b.i) - reinterpret_cast<char*>(&b) << std::endl;
    std::cout << "  （a 在 0、d 在 8——中间 7 字节是填充；i 在 16、尾部填 4 到 24）" << std::endl;
    return 0;
}
