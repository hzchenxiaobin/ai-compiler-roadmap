#include <iostream>
#include <cstring>

// 手写 my_strlen 与 my_strcmp，再和标准库版本对照
int my_strlen(const char s[]) {
    int n = 0;
    while (s[n] != '\0') {        // 从头数到终结符
        ++n;
    }
    return n;
}

int my_strcmp(const char a[], const char b[]) {
    int i = 0;
    while (a[i] != '\0' && a[i] == b[i]) {   // 一起走到第一个不同的字符
        ++i;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];   // 差值：负/零/正
}

void show_cmp(const char* label, const char a[], const char b[]) {
    int mine = my_strcmp(a, b);
    int lib = std::strcmp(a, b);
    std::cout << label << "：my_strcmp = " << mine
              << "，std::strcmp = " << lib << "（符号一致即同结论）" << std::endl;
}

int main() {
    const char apple[] = "apple";
    const char banana[] = "banana";
    const char apple2[] = "apple";

    std::cout << "my_strlen(apple) = " << my_strlen(apple)
              << "，std::strlen = " << std::strlen(apple) << std::endl;

    show_cmp("apple vs banana", apple, banana);   // 期望负：'a'-'b'
    show_cmp("apple vs apple ", apple, apple2);   // 期望 0
    show_cmp("banana vs apple", banana, apple);   // 期望正
    return 0;
}
