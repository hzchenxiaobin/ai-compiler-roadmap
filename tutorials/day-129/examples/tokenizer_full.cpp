#include <cctype>
#include <iostream>
#include <string>
#include <vector>

// Day 129：Tokenizer 完整版——数字/字符串/符号的切分（错误处理+行号）
enum class Tok { LeftBrace, RightBrace, LeftBracket, RightBracket,
                 Colon, Comma, String, Number, True, False, Null, Error, End };

const char* tok_name(Tok t) {
    switch (t) {
        case Tok::LeftBrace: return "{";    case Tok::RightBrace: return "}";
        case Tok::LeftBracket: return "[";  case Tok::RightBracket: return "]";
        case Tok::Colon: return ":";        case Tok::Comma: return ",";
        case Tok::String: return "string";  case Tok::Number: return "number";
        case Tok::True: return "true";      case Tok::False: return "false";
        case Tok::Null: return "null";
        case Tok::Error: return "ERROR";    case Tok::End: return "EOF";
    }
    return "?";
}

struct Token {
    Tok type;
    std::string text;    // String 存解码后内容 / Number 存原文
    int line;
};

struct TokError {
    std::string msg;
    int line;
};

// 完整版：错误收集（不中断——报出全部问题）+ 数字严格文法 + 字符串（转义明天 Day 130）
std::vector<Token> tokenize(const std::string& src, std::vector<TokError>& errors) {
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
                bool closed = false;
                while (i < src.size()) {
                    if (src[i] == '"') { closed = true; ++i; break; }
                    if (src[i] == '\\' && i + 1 < src.size()) {
                        // 转义：今天的简化处理（完整 \u 明天）
                        char e = src[i + 1];
                        switch (e) {
                            case 'n': s += '\n'; break;
                            case 't': s += '\t'; break;
                            case '"': s += '"';  break;
                            case '\\': s += '\\'; break;
                            case '/': s += '/';  break;
                            default:
                                errors.push_back({"非法转义 \\" + std::string(1, e), line});
                                break;
                        }
                        i += 2;
                        continue;
                    }
                    if (src[i] == '\n') { break; }  // 字符串内裸换行——不规范
                    s += src[i++];
                }
                if (!closed) {
                    errors.push_back({"字符串未闭合（到行尾/EOF）", line});
                }
                push(Tok::String, std::move(s));
                break;
            }

            default: {
                if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
                    // 数字文法：-?int(.frac)?([eE][+-]?exp)? —— 今天先扫字符集
                    std::string s;
                    auto is_num_char = [](char ch) {
                        return std::isdigit(static_cast<unsigned char>(ch)) || ch=='.' || ch=='e' || ch=='E' || ch=='+' || ch=='-';
                    };
                    while (i < src.size() && is_num_char(src[i])) { s += src[i++]; }
                    // 快速校验（完整 strtod 验证 Day 131）
                    bool has_digit = false;
                    for (char ch : s) { if (isdigit(static_cast<unsigned char>(ch))) { has_digit = true; break; } }
                    if (!has_digit) {
                        errors.push_back({"数字缺少数字字符：\"" + s + "\"", line});
                        push(Tok::Error, std::move(s));
                    } else {
                        push(Tok::Number, std::move(s));
                    }
                }
                else if (src.compare(i, 4, "true") == 0)  { push(Tok::True);  i += 4; }
                else if (src.compare(i, 5, "false") == 0) { push(Tok::False); i += 5; }
                else if (src.compare(i, 4, "null") == 0)  { push(Tok::Null);  i += 4; }
                else {
                    errors.push_back({std::string("意外字符 '") + c + "'", line});
                    push(Tok::Error, std::string(1, c));
                    ++i;
                }
            }
        }
    }
    push(Tok::End);
    return out;
}

void report(const std::string& title, const std::string& src) {
    std::cout << "== " << title << " ==" << std::endl;
    std::cout << "  输入：" << src << std::endl;
    std::vector<TokError> errors;
    auto toks = tokenize(src, errors);
    std::cout << "  记号（" << toks.size() << " 个）：";
    for (const auto& t : toks) {
        if (t.type == Tok::End) { std::cout << "EOF"; break; }
        std::cout << tok_name(t.type)
                  << (t.text.empty() ? "" : "[" + t.text + "]") << ' ';
    }
    std::cout << std::endl;
    if (errors.empty()) {
        std::cout << "  错误：无 ✔" << std::endl;
    } else {
        for (const auto& e : errors) {
            std::cout << "  [错误] 第" << e.line << "行：" << e.msg << std::endl;
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Day 129：Tokenizer 完整版——切分与错误处理\n" << std::endl;

    // 正常输入（多行——行号的展示）
    report("正常输入（多行）",
           "{\n  \"city\": \"杭州\",\n  \"pop\": 12620000,\n  \"codes\": [310, false]\n}");

    // 数字的各种形态
    report("数字形态", "[0, -1, 3.14, 1e5, 2.5E-3, 42]");

    // 字符串转义（今天的简化集）
    report("字符串转义", "[\"a\\nb\", \"c\\td\", \"d\\\\\", \"e\\\"f\"]");

    // 错误：非法字符 + 非法转义 + 未闭合
    report("错误：非法字符", "[1, @, 2]");
    report("错误：非法转义", "[\"bad\\x\"]");
    report("错误：未闭合", "[\"oops");
    report("错误：数字无数字字符", "[-.]");

    std::cout << "== 设计要点 ==" << std::endl;
    std::cout << "  错误不中断：收集全部再报（方便一次改对）" << std::endl;
    std::cout << "  行号跟踪：换行 ++（报告的锚点）" << std::endl;
    std::cout << "  转义简化集：n t \" \\\\ /（\\uXXXX 明天 Day 130）" << std::endl;
    std::cout << "  数字字符集扫描（严格文法 strtod 验证 Day 131）" << std::endl;
    return 0;
}
