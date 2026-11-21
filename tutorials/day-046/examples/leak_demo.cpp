#include <cstring>
#include <iostream>
#include <string>

// 资源泄漏的真实后果：派生类持有堆内存，基类析构非虚 → delete 只拆基类 → 内存泄漏
class Buffer {
public:
    Buffer() { std::cout << "  [Buffer 构造]" << std::endl; }
    ~Buffer() { std::cout << "  [Buffer 析构]" << std::endl; }
};

class BigBuffer : public Buffer {
private:
    char* data_;

public:
    explicit BigBuffer(std::size_t n) : data_(new char[n]) {
        std::cout << "  [BigBuffer 构造] 拿了 " << n << " 字节" << std::endl;
    }
    ~BigBuffer() {
        delete[] data_;
        std::cout << "  [BigBuffer 析构] 释放了内存" << std::endl;
    }
};

int main() {
    std::cout << "非虚析构版（Buffer 的析构没标 virtual）：" << std::endl;
    {
        Buffer* p = new BigBuffer(1000);
        delete p;    // ~BigBuffer 被跳过 → data_ 的 1000 字节永远拿不回来了
    }
    std::cout << "（上面缺了 [BigBuffer 析构] —— 1000 字节泄漏）" << std::endl;
    return 0;
}
