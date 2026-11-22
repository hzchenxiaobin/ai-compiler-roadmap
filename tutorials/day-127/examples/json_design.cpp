#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Day 127：JSON 规范学习 + Tokenizer/Parser/Value 三模块总体设计
// 本日产出：设计文档式的代码骨架（模块边界先定）+ 一个最小 Tokenizer 原型

// ===== Value 模块：JSON 值的表示（设计稿） =====
// JSON 的六种值：object/array/string/number/bool/null
class JsonValue {
public:
    enum class Type { Null, Bool, Number, String, Array, Object };
    explicit JsonValue(Type t) : type_(t) {}

    Type type() const { return type_; }
    static const char* type_name(Type t) {
        switch (t) {
            case Type::Null: return "null";
            case Type::Bool: return "bool";
            case Type::Number: return "number";
            case Type::String: return "string";
            case Type::Array: return "array";
            case Type::Object: return "object";
        }
        return "?";
    }
private:
    Type type_;
};

// ===== Tokenizer 模块：字符流 → 记号流（原型） =====
enum class Tok {
    LeftBrace, RightBrace,        // { }
    LeftBracket, RightBracket,    // [ ]
    Colon, Comma,                 // : ,
    String, Number, True, False, Null,
    End
};

const char* tok_name(Tok t) {
    switch (t) {
        case Tok::LeftBrace: return "{";
        case Tok::RightBrace: return "}";
        case Tok::LeftBracket: return "[";
        case Tok::RightBracket: return "]";
        case Tok::Colon: return ":";
        case Tok::Comma: return ",";
        case Tok::String: return "string";
        case Tok::Number: return "number";
        case Tok::True: return "true";
        case Tok::False: return "false";
        case Tok::Null: return "null";
        case Tok::End: return "EOF";
    }
    return "?";
}

struct Token {
    Tok type;
    std::string text;    // string/number 的原文
    int line;            // 出错报告用（Day 48 的老习惯）
};

// 最小 tokenizer：跳空白、认单字符记号、认字符串/数字/字面量
std::vector<Token> tokenize(const std::string& src) {
    std::vector<Token> out;
    std::size_t i = 0;
    int line = 1;
    auto push = [&](Tok t, std::string s = "") { out.push_back({t, std::move(s), line}); };
    while (i < src.size()) {
        char c = src[i];
        if (c == '\n') { ++line; ++i; continue; }
        if (c == ' ' || c == '\t' || c == '\r') { ++i; continue; }
        switch (c) {
            case '{': push(Tok::LeftBrace); ++i; break;
            case '}': push(Tok::RightBrace); ++i; break;
            case '[': push(Tok::LeftBracket); ++i; break;
            case ']': push(Tok::RightBracket); ++i; break;
            case ':': push(Tok::Colon); ++i; break;
            case ',': push(Tok::Comma); ++i; break;
            case '"': {
                std::string s;
                ++i;
                while (i < src.size() && src[i] != '"') {
                    if (src[i] == '\\') { /* 转义——完整版 Day 129 */ }
                    s += src[i++];
                }
                ++i;  // 跳过闭引号（完整版：报未闭合错误）
                push(Tok::String, std::move(s));
                break;
            }
            default: {
                if (isdigit(c) || c == '-') {        // 数字（简化版）
                    std::string s;
                    while (i < src.size() && (isdigit(src[i]) || src[i]=='.' || src[i]=='-' || src[i]=='+' || src[i]=='e' || src[i]=='E')) { s += src[i++]; }
                    push(Tok::Number, std::move(s));
                } else if (src.compare(i, 4, "true") == 0)  { push(Tok::True);  i += 4; }
                else if (src.compare(i, 5, "false") == 0) { push(Tok::False); i += 5; }
                else if (src.compare(i, 4, "null") == 0)  { push(Tok::Null);  i += 4; }
                else {
                    std::cout << "  [tokenizer 错误] 第 " << line << " 行：意外字符 '" << c << "'" << std::endl;
                    ++i;
                }
            }
        }
    }
    push(Tok::End);
    return out;
}

int main() {
    std::cout << "== JSON 规范速览（六种值） ==" << std::endl;
    std::cout << "  对象 {\"k\":v,…}｜数组 [v,…]｜字符串 \"…\"" << std::endl;
    std::cout << "  数字（-12.5e3）｜布尔 true/false｜null" << std::endl;
    std::cout << "  （值可嵌套——树形结构的文本表示）" << std::endl;

    std::cout << "\n== 三模块的分工（总体设计） ==" << std::endl;
    std::cout << "  Tokenizer：字符流 → 记号流（今天原型 ✔）" << std::endl;
    std::cout << "  Parser：记号流 → JsonValue 树（Day 132 主场）" << std::endl;
    std::cout << "  Value：六种值的表示 + 访问/序列化" << std::endl;
    std::cout << "  （三层流水线——各管一段，接口清晰）" << std::endl;

    std::cout << "\n== Tokenizer 原型实测 ==" << std::endl;
    std::string demo = "{\"name\": \"Ada\", \"age\": 36, \"tags\": [\"math\", true], \"score\": 99.5, \"note\": null}";
    std::cout << "  输入：" << demo << std::endl;
    auto toks = tokenize(demo);
    std::cout << "  输出记号流（" << toks.size() << " 个）：" << std::endl;
    for (const auto& t : toks) {
        if (t.type == Tok::End) { std::cout << "  EOF" << std::endl; break; }
        std::cout << "    " << tok_name(t.type)
                  << (t.text.empty() ? "" : "(\"" + t.text + "\")")
                  << "  [第" << t.line << "行]" << std::endl;
    }

    std::cout << "\n== 出错报告的预演 ==" << std::endl;
    auto bad = tokenize("{\"k\": @}");
    std::cout << "  （'@' 被拒——第 1 行意外字符 ✔）" << std::endl;
    std::cout << "  （完整版还要：未闭合字符串/非法数字——Day 129 的错误处理专场）" << std::endl;

    std::cout << "\n== 与旧知识的连接 ==" << std::endl;
    std::cout << "  Tokenizer ≈ Day 77 工厂（一行一对象）+ Day 17 istringstream" << std::endl;
    std::cout << "  Value 树 ≈ Day 76 图形库的多态层次（variant 也行——Day 121）" << std::endl;
    std::cout << "  Parser ≈ Day 121 的 optional<Json>（失败表达）" << std::endl;
    return 0;
}
