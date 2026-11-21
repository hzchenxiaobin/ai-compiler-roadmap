#include <iostream>

// 指针数组：数组的每个元素都是指针（char* 的经典舞台）
int main() {
    // 三个字符串字面量，names 里存的是三个门牌
    const char* names[3] = {"数组", "指针", "字符串"};

    std::cout << "sizeof(names) = " << sizeof(names) << "（3 个指针 × 8 字节）" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "names[" << i << "] = " << names[i]
                  << "（门牌 " << (const void*)names[i] << "）" << std::endl;
    }

    // main(int argc, char** argv) 里的 argv 正是这类结构的近亲：
    // 一串字符串门牌组成的数组，程序名在 argv[0]
    return 0;
}
