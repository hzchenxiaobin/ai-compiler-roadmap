#include <iostream>

// 二级指针：门牌也有自己的门牌
int main() {
    int x = 42;
    int* p = &x;          // p 存 x 的门牌
    int** pp = &p;        // pp 存 p 的门牌（指针也是变量，也住房间）

    std::cout << "x = " << x << std::endl;
    std::cout << "&x = " << &x << std::endl;
    std::cout << "p = " << p << "（x 的门牌）" << std::endl;
    std::cout << "&p = " << &p << "（p 自己的门牌）" << std::endl;
    std::cout << "pp = " << pp << "（就是 &p）" << std::endl;

    std::cout << "*pp = " << *pp << "（顺着 pp 拿到 p 的内容 = x 的门牌）" << std::endl;
    std::cout << "**pp = " << **pp << "（再顺一层，拿到 x 的值）" << std::endl;

    **pp = 77;            // 两层解引用，改的还是 x 本尊
    std::cout << "**pp = 77 之后：x = " << x << std::endl;
    return 0;
}
