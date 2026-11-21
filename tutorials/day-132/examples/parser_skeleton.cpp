#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

// Day 132：Parser 骨架——object/array 的递归下降（计划指定）
// 三栈齐备（127 符号/129-130 字符串/131 数字）——今天组装

// ===== 复用前几天的成果 =====
enum class Tok { LeftBrace, RightBrace, LeftBracket, RightBracket,
                 Colon, Comma, String, Number, True, False, Null, End };
struct Token { Tok type; std::string text; int line; };
const char* tok_name(Tok t);

// ===== 复用 Day 128 的 JsonValue =====
class JsonValue;
using JsonArray  = std::vector<std::unique_ptr<JsonValue>>;
using JsonObject = std::map<std::string, std::unique_ptr<JsonValue>>;

class JsonValue {
public:
    using Storage = std::variant<std::monostate, bool, double, std::string, JsonArray, JsonObject>;
    Storage data_;
public:
    JsonValue() : data_(std::monostate{}) {}
    JsonValue(bool b) : data_(b) {}
    JsonValue(double n) : data_(n) {}
    JsonValue(std::string s) : data_(std::move(s)) {}
    JsonValue(JsonArray a) : data_(std::move(a)) {}
    JsonValue(JsonObject o) : data_(std::move(o)) {}

    const char* type_name() const {
        switch (data_.index()) {
            case 0: return "null"; case 1: return "bool"; case 2: return "number";
            case 3: return "string"; case 4: return "array"; default: return "object";
        }
    }
    const Storage& raw() const { return data_; }
};

// ===== Parser：递归下降的核心 =====
struct ParseError { std::string msg; int line; };

class Parser {
    const std::vector<Token>& toks_;
    std::size_t pos_ = 0;

    const Token& cur() const { return toks_[pos_]; }
    const Token& advance() { return toks_[pos_++]; }
    bool check(Tok t) const { return cur().type == t; }
    bool match(Tok t) { if (check(t)) { ++pos_; return true; } return false; }

    Token expect(Tok t, const char* what) {
        if (!check(t)) {
            throw ParseError{std::string("期望 ") + what + "，遇到 " + tok_name(cur().type), cur().line};
        }
        return advance();
    }

    // 文法的根：value := object | array | string | number | true | false | null
    std::unique_ptr<JsonValue> parse_value() {
        switch (cur().type) {
            case Tok::LeftBrace:  return parse_object();
            case Tok::LeftBracket: return parse_array();
            case Tok::String:     return std::make_unique<JsonValue>(advance().text);
            case Tok::Number:     return std::make_unique<JsonValue>(std::stod(advance().text));
            case Tok::True:       advance(); return std::make_unique<JsonValue>(true);
            case Tok::False:      advance(); return std::make_unique<JsonValue>(false);
            case Tok::Null:       advance(); return std::make_unique<JsonValue>();
            default:
                throw ParseError{std::string("此处应是值，遇到 ") + tok_name(cur().type), cur().line};
        }
    }

    // object := '{' (string ':' value (',' string ':' value)*)? '}'
    std::unique_ptr<JsonValue> parse_object() {
        expect(Tok::LeftBrace, "'{'");
        JsonObject obj;
        if (!check(Tok::RightBrace)) {
            while (true) {
                Token key = expect(Tok::String, "键（字符串）");
                expect(Tok::Colon, "':'");
                obj[key.text] = parse_value();               // ★ 递归下降的心脏
                if (!match(Tok::Comma)) { break; }
            }
        }
        expect(Tok::RightBrace, "'}'");
        return std::make_unique<JsonValue>(std::move(obj));
    }

    // array := '[' (value (',' value)*)? ']'
    std::unique_ptr<JsonValue> parse_array() {
        expect(Tok::LeftBracket, "'['");
        JsonArray arr;
        if (!check(Tok::RightBracket)) {
            while (true) {
                arr.push_back(parse_value());                // ★ 递归的心脏
                if (!match(Tok::Comma)) { break; }
            }
        }
        expect(Tok::RightBracket, "']'");
        return std::make_unique<JsonValue>(std::move(arr));
    }

public:
    explicit Parser(const std::vector<Token>& toks) : toks_(toks) {}

    // 解析入口：要求整个输入是一个值且消费到 EOF
    std::unique_ptr<JsonValue> parse() {
        auto v = parse_value();
        if (!check(Tok::End)) {
            throw ParseError{"值结束后还有多余内容（" + std::string(tok_name(cur().type)) + "）", cur().line};
        }
        return v;
    }
};

// ===== 简易 tokenizer（复用 Day 129 的核心——足够演示） =====
std::vector<Token> tokenize(const std::string& src) {
    std::vector<Token> out;
    std::size_t i = 0; int line = 1;
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
                std::string s; ++i;
                while (i < src.size() && src[i] != '"') { s += src[i++]; }
                ++i; push(Tok::String, std::move(s)); break;
            }
            default: {
                if (isdigit(static_cast<unsigned char>(c)) || c == '-') {
                    std::string s;
                    while (i < src.size() && (isdigit(static_cast<unsigned char>(src[i])) || src[i]=='.' || src[i]=='e' || src[i]=='E' || src[i]=='-' || src[i]=='+')) { s += src[i++]; }
                    push(Tok::Number, std::move(s));
                } else if (src.compare(i, 4, "true") == 0) { push(Tok::True); i += 4; }
                else if (src.compare(i, 5, "false") == 0) { push(Tok::False); i += 5; }
                else if (src.compare(i, 4, "null") == 0) { push(Tok::Null); i += 4; }
                else { push(Tok::End); ++i; }
            }
        }
    }
    push(Tok::End);
    return out;
}

const char* tok_name(Tok t) {
    switch (t) {
        case Tok::LeftBrace: return "{"; case Tok::RightBrace: return "}";
        case Tok::LeftBracket: return "["; case Tok::RightBracket: return "]";
        case Tok::Colon: return ":"; case Tok::Comma: return ",";
        case Tok::String: return "string"; case Tok::Number: return "number";
        case Tok::True: return "true"; case Tok::False: return "false";
        case Tok::Null: return "null"; case Tok::End: return "EOF";
    }
    return "?";
}

// ===== 测试驱动 =====
void try_parse(const char* label, const std::string& src) {
    std::cout << "== " << label << " ==" << std::endl;
    std::cout << "  输入：" << src << std::endl;
    try {
        auto toks = tokenize(src);
        Parser p(toks);
        auto v = p.parse();
        std::cout << "  ✔ 解析成功：类型 = " << v->type_name() << std::endl;
    } catch (const ParseError& e) {
        std::cout << "  ✘ [第" << e.line << "行] " << e.msg << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Day 132：Parser 骨架——递归下降\n" << std::endl;

    try_parse("空对象", "{}");
    try_parse("空数组", "[]");
    try_parse("简单对象", "{\"name\": \"Ada\", \"age\": 36}");
    try_parse("嵌套（对象含数组含对象）",
              "{\"team\": [{\"id\": 1}, {\"id\": 2}], \"ok\": true}");
    try_parse("深层嵌套", "[[[[1]]]]");
    try_parse("全类型", "[1, 2.5, \"three\", true, false, null, {\"k\": \"v\"}]");

    // 错误场景
    try_parse("错误：缺冒号", "{\"name\" \"Ada\"}");
    try_parse("错误：缺闭括号", "{\"a\": 1");
    try_parse("错误：尾逗号", "[1, 2,]");
    try_parse("错误：值后多余", "1 2");

    std::cout << "== 递归下降的骨架 ==" << std::endl;
    std::cout << "  parse_value 分派六种值（对象/数组递归进下一层）" << std::endl;
    std::cout << "  parse_object：'{' → 键:值对（逗号分隔）→ '}'" << std::endl;
    std::cout << "  parse_array：'[' → 值（逗号分隔）→ ']'" << std::endl;
    std::cout << "  （文法规则 = 函数结构——一一对应）" << std::endl;
    return 0;
}
