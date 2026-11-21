#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 组合 vs 继承的最终对表：同一需求的两种设计
// 需求：程序员会写代码、会调试；资深程序员还会带徒弟、评审代码

// ===== 方案 A：继承（资深程序员 是一个 程序员） =====
class ProgrammerA {
public:
    virtual ~ProgrammerA() = default;
    void code() const { std::cout << "  写代码" << std::endl; }
    void debug() const { std::cout << "  调试" << std::endl; }
};

class SeniorA : public ProgrammerA {
public:
    void mentor() const { std::cout << "  带徒弟" << std::endl; }
    void review() const { std::cout << "  评审代码" << std::endl; }
};

// ===== 方案 B：组合（资深程序员 有 导师能力 + 评审能力） =====
class Mentor {
public:
    void mentor() const { std::cout << "  带徒弟" << std::endl; }
};

class Reviewer {
public:
    void review() const { std::cout << "  评审代码" << std::endl; }
};

class SeniorB {
private:
    Mentor mentor_;        // 组合：有导师能力
    Reviewer reviewer_;    // 组合：有评审能力

public:
    // 转发接口（组合的代价：多写几行转发）
    void mentor() const { mentor_.mentor(); }
    void review() const { reviewer_.review(); }
    // 想要写代码能力？再组合一个 Programmer——按需拼装
};

int main() {
    std::cout << "方案 A（继承）：" << std::endl;
    SeniorA sa;
    sa.code(); sa.debug(); sa.mentor(); sa.review();

    std::cout << "\n方案 B（组合）：" << std::endl;
    SeniorB sb;
    sb.mentor(); sb.review();

    std::cout << "\n选型分析：" << std::endl;
    std::cout << "  A：一行不写白得 code/debug（复用强）；但SeniorA永远绑死 ProgrammerA" << std::endl;
    std::cout << "  B：能力按需拼装（灵活）；但每个能力要写转发（样板代码）" << std::endl;
    std::cout << "  判断：Senior IS-A Programmer 成立且稳定 → A" << std::endl;
    std::cout << "        未来可能只有 Mentor 没有 coding → B" << std::endl;
    return 0;
}
