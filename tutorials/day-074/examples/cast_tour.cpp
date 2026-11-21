#include <iostream>
#include <string>

// static_cast / dynamic_cast / const_cast 巡礼（reinterpret_cast 进阶阶段再碰）
class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const { std::cout << "动物发出声音" << std::endl; }
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "汪汪！" << std::endl; }
    void fetch() const { std::cout << "（狗去捡球了）" << std::endl; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "喵～" << std::endl; }
};

int main() {
    Dog dog;
    Cat cat;
    Animal* a1 = &dog;      // 基类指针指向狗
    Animal* a2 = &cat;      // 基类指针指向猫

    // ① static_cast：编译期转换——"我担保它是狗"（不检查）
    std::cout << "== static_cast ==" << std::endl;
    Dog* d1 = static_cast<Dog*>(a1);       // a1 确实指狗 → 合法
    d1->fetch();                             // ✔ 调到狗的专属函数
    // Dog* bad = static_cast<Dog*>(a2);    // a2 是猫！担保错了 → UB（不报错）
    // bad->fetch();                        // 可能崩可能不崩——最危险

    // ② dynamic_cast：运行期检查——"先验货再进门"
    std::cout << "== dynamic_cast ==" << std::endl;
    if (Dog* d2 = dynamic_cast<Dog*>(a1)) {  // 验货：a1 是狗吗？
        std::cout << "a1 是狗："; d2->fetch();
    }
    if (Dog* d3 = dynamic_cast<Dog*>(a2)) {   // a2 是猫 → 指针为 nullptr
        d3->fetch();
    } else {
        std::cout << "a2 不是狗（dynamic_cast 返回 nullptr）" << std::endl;
    }

    // ③ const_cast：摘掉 const（合法场景很少）
    std::cout << "== const_cast ==" << std::endl;
    const int original = 42;
    const int* cp = &original;
    int* mutable_p = const_cast<int*>(cp);    // 编译过了……
    // *mutable_p = 99;                        // 但写它仍是 UB（original 是 const 对象！）
    std::cout << "original = " << original << "（没敢写——const 对象本来就不许改）" << std::endl;
    return 0;
}
