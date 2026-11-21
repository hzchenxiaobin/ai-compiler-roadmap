#include <iostream>

// 数组退化：数组名在表达式里变成"首元素的地址"
int main() {
    int scores[5] = {10, 20, 30, 40, 50};
    int* p = scores;                 // 等价于 p = &scores[0]——数组名退化成首地址

    std::cout << "scores        = " << scores << std::endl;
    std::cout << "&scores[0]    = " << &scores[0] << "（两者相同：退化）" << std::endl;
    std::cout << "scores + 1    = " << (scores + 1) << std::endl;
    std::cout << "&scores[1]    = " << &scores[1] << "（加 1 跳一个元素）" << std::endl;

    std::cout << "sizeof(scores) = " << sizeof(scores) << "（整个数组 5×4 字节）" << std::endl;
    std::cout << "sizeof(p)      = " << sizeof(p) << "（指针自己只有 8 字节）" << std::endl;
    std::cout << "退化陷阱：数组传给函数后，函数里 sizeof 拿到的只是指针大小！" << std::endl;
    return 0;
}
