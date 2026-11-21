#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// ===== Day 133：解析全流程打通——Tokenizer + Parser + Value + 错误处理 =====
// 一切合成一个文件：json::parse(string) -> optional<JsonValue>（失败带行号消息）

namespace json {

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

// ===== 错误类型 =====
struct Error {
    std::string msg;
    int line;
    std::string str() const { return "[第" + std::to_string(line) + "行] " + msg; }
};

// ===== Tokenizer（完整版：129+130+131 的合并） =====
enum class Tok { LBrace, RBrace, LBracket, RBracket, Colon, Comma,
                 Str, Num, True, False, Null, End };
const char* tok_name(Tok t) {
    switch (t) {
        case Tok::LBrace: return "{"; case Tok::RBrace: return "}";
        case Tok::LBracket: return "["; case Tok::RBracket: return "]";
        case Tok::Colon: return ":"; case Tok::Comma: return ",";
        case Tok::Str: return "string"; case Tok::Num: return "number";
        case Tok::True: return "true"; case Tok::False: return "false";
        case Tok::Null: return "null"; case Tok::End: return "EOF";
    }
    return "?";
}
struct Token { Tok type; std::string text; int line; };

int hex_val(char c) {
    if (c>='0'&&c<='9') return c-'0';
    if (c>='a'&&c<='f') return c-'a'+10;
    if (c>='A'&&c<='F') return c-'A'+10;
    return -1;
}
void append_utf8(std::string& out, unsigned cp) {
    if (cp<0x80) { out+=char(cp); }
    else if (cp<0x800) { out+=char(0xC0|(cp>>6)); out+=char(0x80|(cp&0x3F)); }
    else if (cp<0x10000) { out+=char(0xE0|(cp>>12)); out+=char(0x80|((cp>>6)&0x3F)); out+=char(0x80|(cp&0x3F)); }
    else { out+=char(0xF0|(cp>>18)); out+=char(0x80|((cp>>12)&0x3F)); out+=char(0x80|((cp>>6)&0x3F)); out+=char(0x80|(cp&0x3F)); }
}

std::optional<std::vector<Token>> tokenize(const std::string& src, Error& err) {
    std::vector<Token> out;
    std::size_t i = 0; int line = 1;
    auto push = [&](Tok t, std::string s = "") { out.push_back({t, std::move(s), line}); };
    while (i < src.size()) {
        char c = src[i];
        if (c=='\n') { ++line; ++i; continue; }
        if (c==' '||c=='\t'||c=='\r') { ++i; continue; }
        switch (c) {
            case '{': push(Tok::LBrace); ++i; break;
            case '}': push(Tok::RBrace); ++i; break;
            case '[': push(Tok::LBracket); ++i; break;
            case ']': push(Tok::RBracket); ++i; break;
            case ':': push(Tok::Colon); ++i; break;
            case ',': push(Tok::Comma); ++i; break;
            case '"': {
                std::string s; ++i; bool closed=false;
                while (i < src.size()) {
                    if (src[i]=='"') { closed=true; ++i; break; }
                    if (src[i]=='\\' && i+1 < src.size()) {
                        char e = src[i+1];
                        switch (e) {
                            case 'n': s+='\n'; break; case 't': s+='\t'; break;
                            case 'r': s+='\r'; break; case 'b': s+='\b'; break;
                            case 'f': s+='\f'; break; case '"': s+='"'; break;
                            case '\\': s+='\\'; break; case '/': s+='/'; break;
                            case 'u': {
                                if (i+5 < src.size()+1 && i+2+4 <= src.size()) {
                                    unsigned cp=0; bool ok=true;
                                    for (int k=0;k<4;++k) { int h=hex_val(src[i+2+k]); if(h<0){ok=false;break;} cp=cp*16+h; }
                                    if (ok) {
                                        if (cp>=0xD800 && cp<=0xDBFF && i+12 <= src.size()) {
                                            unsigned lo=0; bool ok2=true;
                                            for (int k=0;k<4;++k) { int h=hex_val(src[i+8+k]); if(h<0){ok2=false;break;} lo=lo*16+h; }
                                            if (ok2 && lo>=0xDC00 && lo<=0xDFFF) {
                                                append_utf8(s, 0x10000+((cp-0xD800)<<10)+(lo-0xDC00));
                                                i += 12; continue;
                                            }
                                        }
                                        if (!(cp>=0xD800)) {
                                            append_utf8(s, cp);
                                            i += 6; continue;
                                        }
                                    }
                                }
                                err = {"\\u 转义非法（四位/代理对）", line};
                                return std::nullopt;
                            }
                            default:
                                err = {std::string("非法转义 \\")+e, line};
                                return std::nullopt;
                        }
                        i += 2; continue;
                    }
                    if (src[i]=='\n') break;
                    s += src[i++];
                }
                if (!closed) { err = {"字符串未闭合", line}; return std::nullopt; }
                push(Tok::Str, std::move(s));
                break;
            }
            default: {
                if (c=='-' || (c>='0'&&c<='9')) {
                    std::string s;
                    while (i<src.size() && ((src[i]>='0'&&src[i]<='9')||src[i]=='.'||src[i]=='e'||src[i]=='E'||src[i]=='+'||src[i]=='-')) s+=src[i++];
                    errno=0; char* end=nullptr;
                    strtod(s.c_str(), &end);
                    std::size_t used = end - s.c_str();
                    if (used != s.size()) { err = {"畸形数字 \""+s+"\"", line}; return std::nullopt; }
                    push(Tok::Num, std::move(s));
                }
                else if (src.compare(i,4,"true")==0) { push(Tok::True); i+=4; }
                else if (src.compare(i,5,"false")==0) { push(Tok::False); i+=5; }
                else if (src.compare(i,4,"null")==0) { push(Tok::Null); i+=4; }
                else { err = {std::string("意外字符 '")+c+"'", line}; return std::nullopt; }
            }
        }
    }
    push(Tok::End);
    return out;
}

// ===== Parser（132 的骨架 + optional 失败表达） =====
class Parser {
    const std::vector<Token>& toks_;
    std::size_t pos_ = 0;
    const Token& cur() const { return toks_[pos_]; }
    const Token& adv() { return toks_[pos_++]; }
    bool check(Tok t) const { return cur().type == t; }
    bool match(Tok t) { if (check(t)) { ++pos_; return true; } return false; }
    bool expect(Tok t, const char* what, Error& err) {
        if (!check(t)) {
            err = {std::string("期望 ")+what+"，遇到 "+tok_name(cur().type), cur().line};
            return false;
        }
        adv(); return true;
    }
    std::unique_ptr<JsonValue> value(Error& err) {
        switch (cur().type) {
            case Tok::LBrace: return object(err);
            case Tok::LBracket: return array(err);
            case Tok::Str: return std::make_unique<JsonValue>(adv().text);
            case Tok::Num: return std::make_unique<JsonValue>(std::stod(adv().text));
            case Tok::True: adv(); return std::make_unique<JsonValue>(true);
            case Tok::False: adv(); return std::make_unique<JsonValue>(false);
            case Tok::Null: adv(); return std::make_unique<JsonValue>();
            default:
                err = {std::string("此处应是值，遇到 ")+tok_name(cur().type), cur().line};
                return nullptr;
        }
    }
    std::unique_ptr<JsonValue> object(Error& err) {
        if (!expect(Tok::LBrace, "'{'", err)) return nullptr;
        JsonObject obj;
        if (!check(Tok::RBrace)) {
            while (true) {
                if (!check(Tok::Str)) {
                    err = {"期望键（字符串），遇到 "+std::string(tok_name(cur().type)), cur().line};
                    return nullptr;
                }
                std::string key = adv().text;
                if (!expect(Tok::Colon, "':'", err)) return nullptr;
                auto v = value(err);
                if (!v) return nullptr;
                obj[key] = std::move(v);
                if (!match(Tok::Comma)) break;
            }
        }
        if (!expect(Tok::RBrace, "'}'", err)) return nullptr;
        return std::make_unique<JsonValue>(std::move(obj));
    }
    std::unique_ptr<JsonValue> array(Error& err) {
        if (!expect(Tok::LBracket, "'['", err)) return nullptr;
        JsonArray arr;
        if (!check(Tok::RBracket)) {
            while (true) {
                auto v = value(err);
                if (!v) return nullptr;
                arr.push_back(std::move(v));
                if (!match(Tok::Comma)) break;
            }
        }
        if (!expect(Tok::RBracket, "']'", err)) return nullptr;
        return std::make_unique<JsonValue>(std::move(arr));
    }
public:
    explicit Parser(const std::vector<Token>& t) : toks_(t) {}
    std::unique_ptr<JsonValue> parse(Error& err) {
        auto v = value(err);
        if (!v) return nullptr;
        if (!check(Tok::End)) {
            err = {"值结束后还有多余内容", cur().line};
            return nullptr;
        }
        return v;
    }
};

// ===== 用户 API：一个函数 =====
std::optional<JsonValue> parse(const std::string& src, Error& err) {
    auto toks = tokenize(src, err);
    if (!toks) return std::nullopt;                 // 词法错
    Parser p(*toks);
    auto v = p.parse(err);
    if (!v) return std::nullopt;                    // 语法错
    return JsonValue(std::move(*v));
}

} // namespace json

// ===== 演示 =====
void show(const char* label, const std::string& src) {
    std::cout << "== " << label << " ==" << std::endl;
    std::cout << "  输入：" << src << std::endl;
    json::Error err;
    auto v = json::parse(src, err);
    if (v) {
        std::cout << "  ✔ 解析成功（类型 = " << v->type_name() << "）" << std::endl;
    } else {
        std::cout << "  ✘ " << err.str() << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Day 133：JSON 解析全流程\n" << std::endl;

    // 合法（多层多类型）
    show("完整嵌套", "{\"name\": \"Ada\", \"scores\": [95, 88.5], \"meta\": {\"ok\": true, \"tag\": null}}");
    show("含转义", "[\"a\\nb\", \"\\u4e2d\\u6587\"]");
    show("emoji 代理对", "[\"\\uD83D\\uDE00\"]");
    show("多行", "{\n  \"a\": 1,\n  \"b\": [2, 3]\n}");

    // 各层错误
    show("词法错：意外字符", "{\"a\": @}");
    show("词法错：未闭合", "[\"oops");
    show("词法错：畸形数字", "[1.2.3]");
    show("语法错：缺冒号（第2行）", "{\n  \"a\" 1\n}");
    show("语法错：缺闭括", "{\"a\": [1, 2");
    show("语法错：尾逗号", "[1, 2,]");

    std::cout << "== API 的一句话总结 ==" << std::endl;
    std::cout << "  json::parse(src, err) -> optional<JsonValue>" << std::endl;
    std::cout << "  （失败：optional 空 + err 带行号消息）" << std::endl;
    std::cout << "  （词法错/语法错统一报——用户不用管哪层）" << std::endl;
    return 0;
}
