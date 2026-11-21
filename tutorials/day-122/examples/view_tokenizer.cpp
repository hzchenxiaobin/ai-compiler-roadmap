#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Day 122：string_view——轻量分词器（计划指定）+ 悬空危险点
// 分词器：零拷贝地切分（每个 token 是原串的一个 view）
std::vector<std::string_view> tokenize(std::string_view text, char sep) {
    std::vector<std::string_view> tokens;
    std::size_t start = 0;
    while (start <= text.size()) {
        auto pos = text.find(sep, start);
        if (pos == std::string_view::npos) {
            tokens.push_back(text.substr(start));
            break;
        }
        tokens.push_back(text.substr(start, pos - start));   // view 切片——零拷贝！
        start = pos + 1;
    }
    return tokens;
}

int main() {
    std::cout << "== 计划指定：轻量分词器（零拷贝） ==" << std::endl;
    std::string line = "apple,banana,,cherry";
    for (auto tok : tokenize(line, ',')) {
        std::cout << "  [\"" << tok << "\"]（长度 " << tok.size() << "）" << std::endl;
    }
    std::cout << "  （每个 token 是原串的切片——没有一次字符串拷贝）" << std::endl;
    std::cout << "  sizeof(string_view) = " << sizeof(std::string_view)
              << " 字节（指针+长度——比 string 的 24 轻）" << std::endl;

    std::cout << "\n== 隐式转换的便利 ==" << std::endl;
    std::string s = "hello world";
    const char* cs = "C 字符串";
    // 一个 string_view 参数通吃三种来源：
    auto count_non_space = [](std::string_view v) {
        std::size_t n = 0;
        for (char c : v) { if (c != ' ') { ++n; } }
        return n;
    };
    std::cout << "  string → " << count_non_space(s) << std::endl;
    std::cout << "  字面量 → " << count_non_space("hello world") << std::endl;
    std::cout << "  const char* → " << count_non_space(cs) << std::endl;
    std::cout << "  （函数参数收 view——string/字面量/C 串全兼容）" << std::endl;

    std::cout << "\n== 危险点一：悬空 view（临时的命） ==" << std::endl;
    std::string_view bad;
    {
        std::string temp = "我是临时字符串";
        bad = temp;                                  // view 指向 temp 的内存
    }                                                // temp 死了——view 悬空！
    std::cout << "  temp 已析构——bad 悬空（输出未定义，别解引用）" << std::endl;
    std::cout << "  （正确做法：view 的命不能超过所看的数据）" << std::endl;

    std::cout << "\n== 危险点二：view 不保证 null 结尾 ==" << std::endl;
    std::string full = "full-string";                   // string 活着
    // 教学坑实录：full.substr(0,4) 返回临时 string——view 指它照样悬空（-Wdangling 抓到）！
    // 正解：先转 view 再切——view 的 substr 才是零拷贝切片
    std::string_view part = std::string_view(full).substr(0, 4);   // "full"——安全
    std::cout << "  part = \"" << part << "\"（长度 4）" << std::endl;
    std::cout << "  （view 不是 C 串！传给需要 const char* 的 API 前要确认）" << std::endl;
    // printf("%s", part.data());  // ✘ 危险：data() 不保证 null 结尾（切片时）
    std::cout << "  （要 C 串：先转 std::string 再 c_str()）" << std::endl;

    std::cout << "\n== 危险点三：存进容器的坑（跨调用持有） ==" << std::endl;
    std::cout << "  成员/容器/缓存里存 view——所看数据的生命期谁保证？" << std::endl;
    std::cout << "  （经验：view 只做'函数参数和局部'——不当长期所有者）" << std::endl;
    std::cout << "  （要长期持有：拷成 string）" << std::endl;

    std::cout << "\n== 安全使用三律 ==" << std::endl;
    std::cout << "  ①不做所有者（参数/局部用——不长期存）" << std::endl;
    std::cout << "  ②不看将死之物（临时 string 的 view 立刻悬空）" << std::endl;
    std::cout << "  ③不当 C 串用（data() 不保证 null 结尾）" << std::endl;
    return 0;
}
