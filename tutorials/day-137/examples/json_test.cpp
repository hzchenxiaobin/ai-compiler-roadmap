// Day 137：JSON 解析器的 20+ 测试用例（计划指定：空串/深层嵌套/非法输入等边界）
// 被测：Day 133~135 的解析+dump+访问（合入一个头——教学从简）
#include <gtest/gtest.h>
#include <cmath>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

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
    bool is_object() const { return data_.index() == 5; }
    bool is_array()  const { return data_.index() == 4; }
    bool is_string() const { return data_.index() == 3; }
    bool is_number() const { return data_.index() == 2; }
    template <typename T> T get() const {
        if constexpr (std::is_same_v<T, std::string>) {
            if (is_string()) return std::get<std::string>(data_);
        } else if constexpr (std::is_same_v<T, double>) {
            if (is_number()) return std::get<double>(data_);
        }
        throw std::runtime_error("get：实际是 " + std::string(type_name()));
    }
    const JsonValue& operator[](const std::string& key) const {
        const auto& obj = std::get<JsonObject>(data_);
        auto it = obj.find(key);
        if (it == obj.end()) { throw std::runtime_error("键不存在: " + key); }
        return *it->second;
    }
    const JsonValue& operator[](std::size_t i) const {
        const auto& arr = std::get<JsonArray>(data_);
        if (i >= arr.size()) { throw std::runtime_error("越界"); }
        return *arr[i];
    }
    std::size_t size() const {
        if (is_array()) return std::get<JsonArray>(data_).size();
        if (is_object()) return std::get<JsonObject>(data_).size();
        return 0;
    }
    const JsonObject& as_object() const { return std::get<JsonObject>(data_); }
};

struct Error { std::string msg; int line; };

// 简化 tokenizer+parser（Day 133 的核心——测试够用）
namespace tk { constexpr int LB=0, RB=1, LK=2, RK=3, C=4, M=5, S=6, N=7, T_=8, F_=9, NU=10, E_=11; }
struct Tok { int type; std::string text; int line; };

std::optional<JsonValue> parse(const std::string& src, Error& err) {
    std::vector<Tok> toks; std::size_t i = 0; int line = 1;
    auto push = [&](int t, std::string s = "") { toks.push_back({t, std::move(s), line}); };
    while (i < src.size()) {
        char c = src[i];
        if (c=='\n') { ++line; ++i; continue; }
        if (c==' '||c=='\t'||c=='\r') { ++i; continue; }
        switch (c) {
            case '{': push(tk::LB); ++i; break;
            case '}': push(tk::RB); ++i; break;
            case '[': push(tk::LK); ++i; break;
            case ']': push(tk::RK); ++i; break;
            case ':': push(tk::C); ++i; break;
            case ',': push(tk::M); ++i; break;
            case '"': {
                std::string s; ++i;
                while (i < src.size() && src[i] != '"') {
                    if (src[i]=='\\' && i+1 < src.size()) {
                        char e = src[i+1];
                        if (e=='n') s+='\n'; else if (e=='t') s+='\t';
                        else if (e=='"') s+='"'; else if (e=='\\') s+='\\';
                        else if (e=='u') {
                            unsigned cp = 0; bool ok = true;
                            for (int k=0;k<4;++k) { char h=src[i+2+k]; int v=(h<='9'?h-'0':(h|32)-'a'+10); if(v<0||v>15){ok=false;break;} cp=cp*16+v; }
                            if (ok) {
                                if (cp < 0x80) { s += char(cp); }
                                else if (cp < 0x800) { s += char(0xC0|(cp>>6)); s += char(0x80|(cp&0x3F)); }
                                else { s += char(0xE0|(cp>>12)); s += char(0x80|((cp>>6)&0x3F)); s += char(0x80|(cp&0x3F)); }
                                i += 6; continue;
                            }
                        }
                        i += 2; continue;
                    }
                    if (src[i]=='\n') { err = {"字符串未闭合", line}; return std::nullopt; }
                    s += src[i++];
                }
                if (i >= src.size()) { err = {"字符串未闭合", line}; return std::nullopt; }
                ++i; push(tk::S, std::move(s)); break;
            }
            default: {
                if (c=='-'||(c>='0'&&c<='9')) {
                    std::string s;
                    while (i<src.size()&&((src[i]>='0'&&src[i]<='9')||src[i]=='.'||src[i]=='e'||src[i]=='E'||src[i]=='+'||src[i]=='-')) s+=src[i++];
                    // strtod 严格（Day 131）
                    char* end = nullptr; std::strtod(s.c_str(), &end);
                    if (end - s.c_str() != static_cast<long>(s.size())) { err = {"畸形数字 "+s, line}; return std::nullopt; }
                    push(tk::N, std::move(s));
                } else if (src.compare(i,4,"true")==0) { push(tk::T_); i+=4; }
                else if (src.compare(i,5,"false")==0) { push(tk::F_); i+=5; }
                else if (src.compare(i,4,"null")==0) { push(tk::NU); i+=4; }
                else { err = {std::string("意外字符 '")+c+"'", line}; return std::nullopt; }
            }
        }
    }
    push(tk::E_);
    std::size_t p = 0;
    std::function<std::unique_ptr<JsonValue>()> value = [&]() -> std::unique_ptr<JsonValue> {
        switch (toks[p].type) {
            case tk::LB: { ++p; JsonObject obj;
                if (toks[p].type != tk::RB) { while (true) {
                    std::string k = toks[p].text; ++p; ++p;
                    obj[k] = value();
                    if (toks[p].type == tk::M) { ++p; continue; } break; } }
                ++p; return std::make_unique<JsonValue>(std::move(obj)); }
            case tk::LK: { ++p; JsonArray arr;
                if (toks[p].type != tk::RK) { while (true) {
                    arr.push_back(value());
                    if (toks[p].type == tk::M) { ++p; continue; } break; } }
                ++p; return std::make_unique<JsonValue>(std::move(arr)); }
            case tk::S: return std::make_unique<JsonValue>(toks[p++].text);
            case tk::N: return std::make_unique<JsonValue>(std::stod(toks[p++].text));
            case tk::T_: ++p; return std::make_unique<JsonValue>(true);
            case tk::F_: ++p; return std::make_unique<JsonValue>(false);
            default: ++p; return std::make_unique<JsonValue>();
        }
    };
    if (toks.empty() || toks[0].type == tk::E_) { err = {"空输入", line}; return std::nullopt; }
    auto v = value();
    if (toks[p].type != tk::E_) { err = {"值后多余内容", toks[p].line}; return std::nullopt; }
    return JsonValue(std::move(*v));
}

// dump（Day 134 的核心）
std::string escape_string(const std::string& s) {
    std::string out = "\"";
    for (char c : s) {
        switch (c) { case '"': out += "\\\""; break; case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break; case '\t': out += "\\t"; break;
            default: out += c; }
    }
    return out + "\"";
}
std::string format_number(double v) {
    if (v == static_cast<long long>(v) && std::abs(v) < 1e15) { return std::to_string(static_cast<long long>(v)); }
    return std::to_string(v);
}
void dump_to(const JsonValue& v, std::string& out) {
    switch (v.data_.index()) {
        case 0: out += "null"; break;
        case 1: out += (std::get<bool>(v.data_) ? "true" : "false"); break;
        case 2: out += format_number(std::get<double>(v.data_)); break;
        case 3: out += escape_string(std::get<std::string>(v.data_)); break;
        case 4: { const auto& a = std::get<JsonArray>(v.data_);
            out += '['; for (std::size_t k=0;k<a.size();++k) { dump_to(*a[k], out); if (k+1<a.size()) out += ','; } out += ']'; break; }
        default: { const auto& o = std::get<JsonObject>(v.data_);
            out += '{'; bool first = true;
            for (const auto& [k, val] : o) { if (!first) out += ','; first = false; out += escape_string(k); out += ':'; dump_to(*val, out); }
            out += '}'; break; }
    }
}
std::string dump(const JsonValue& v) { std::string out; dump_to(v, out); return out; }

} // namespace json
#include <cstdlib>
#include <functional>

// ===== 测试用例（计划指定：20+——空串/深层嵌套/非法输入等边界） =====
namespace {
json::JsonValue must_parse(const std::string& s) {
    json::Error err;
    auto v = json::parse(s, err);
    if (!v) { ADD_FAILURE() << "意外失败: " << err.msg; return json::JsonValue(); }
    return std::move(*v);
}
}

// --- 套件一：ParseTest（合法形态） ---
TEST(ParseTest, EmptyObject)       { EXPECT_STREQ(must_parse("{}").type_name(), "object"); }
TEST(ParseTest, EmptyArray)        { EXPECT_STREQ(must_parse("[]").type_name(), "array"); }
TEST(ParseTest, Null)              { EXPECT_STREQ(must_parse("null").type_name(), "null"); }
TEST(ParseTest, TrueFalse)         { EXPECT_STREQ(must_parse("[true, false]").type_name(), "array"); }
TEST(ParseTest, NumberInt)         { EXPECT_DOUBLE_EQ(must_parse("42").get<double>(), 42.0); }
TEST(ParseTest, NumberNeg)         { EXPECT_DOUBLE_EQ(must_parse("-7").get<double>(), -7.0); }
TEST(ParseTest, NumberSci)         { EXPECT_DOUBLE_EQ(must_parse("1e3").get<double>(), 1000.0); }
TEST(ParseTest, NumberFrac)        { EXPECT_DOUBLE_EQ(must_parse("3.5").get<double>(), 3.5); }
TEST(ParseTest, DeepNesting)       { EXPECT_STREQ(must_parse("[[[[[[[1]]]]]]]").type_name(), "array"); }  // 7 层
TEST(ParseTest, DeepNestingValue)  { EXPECT_DOUBLE_EQ(must_parse("[[[[[1]]]]]")[0][0][0][0][0].get<double>(), 1.0); }  // 5 层剥 5 次
TEST(ParseTest, MultiLine)         { EXPECT_STREQ(must_parse("{\n \"a\": 1\n}").type_name(), "object"); }
TEST(ParseTest, Whitespace)        { EXPECT_DOUBLE_EQ(must_parse("  [ 1 , 2 ]  ")[1].get<double>(), 2.0); }

// --- 套件二：ParseErrorTest（非法输入——各断言消息） ---
TEST(ParseErrorTest, EmptyInput) {
    json::Error err; auto v = json::parse("", err);
    EXPECT_FALSE(v.has_value());
    EXPECT_EQ(err.msg, "空输入");
}
TEST(ParseErrorTest, UnexpectedChar) {
    json::Error err; auto v = json::parse("[@]", err);
    EXPECT_FALSE(v.has_value());
    EXPECT_EQ(err.msg, "意外字符 '@'");
}
TEST(ParseErrorTest, MalformedNumber) {
    json::Error err; auto v = json::parse("[1.2.3]", err);
    EXPECT_FALSE(v.has_value());
    EXPECT_NE(err.msg.find("畸形"), std::string::npos);
}
TEST(ParseErrorTest, UnclosedString) {
    json::Error err; auto v = json::parse("[\"oops", err);
    EXPECT_FALSE(v.has_value());
    EXPECT_EQ(err.msg, "字符串未闭合");
}

// --- 套件三：DumpTest（序列化） ---
TEST(DumpTest, RoundTrip) {
    auto v = must_parse("{\"a\":1,\"b\":[2,3]}");
    EXPECT_EQ(json::dump(v), "{\"a\":1,\"b\":[2,3]}");
}
TEST(DumpTest, KeyOrderStable) {   // map 字典序（Day 134 的设计）
    auto v = must_parse("{\"b\":1,\"a\":2}");
    EXPECT_EQ(json::dump(v), "{\"a\":2,\"b\":1}");
}
TEST(DumpTest, Escapes) {
    auto v = must_parse("[\"a\\nb\"]");
    EXPECT_EQ(json::dump(v), "[\"a\\nb\"]");    // 编回转义
}
TEST(DumpTest, Idempotent) {       // 幂等：dump 后 parse 再 dump 应相同
    auto v1 = must_parse("{\"z\":[1,{\"y\":null}],\"x\":2.5}");
    auto s1 = json::dump(v1);
    auto v2 = must_parse(s1);
    EXPECT_EQ(json::dump(v2), s1);
}

// --- 套件四：AccessTest（访问接口） ---
TEST(AccessTest, ChainDrill) {
    auto v = must_parse("{\"cfg\":{\"host\":\"local\"}}");
    EXPECT_EQ(v["cfg"]["host"].get<std::string>(), "local");
}
TEST(AccessTest, ArrayIndex) {
    auto v = must_parse("[10, 20, 30]");
    EXPECT_DOUBLE_EQ(v[2].get<double>(), 30.0);
}
TEST(AccessTest, Size) {
    auto v = must_parse("{\"a\":1,\"b\":2,\"c\":3}");
    EXPECT_EQ(v.size(), 3u);
}
TEST(AccessTest, KeyMissingThrows) {
    auto v = must_parse("{\"a\":1}");
    EXPECT_THROW(v["nope"], std::runtime_error);
}
TEST(AccessTest, OutOfRangeThrows) {
    auto v = must_parse("[1,2,3]");
    EXPECT_THROW(v[99], std::runtime_error);
}
TEST(AccessTest, Iteration) {
    auto v = must_parse("[1,2,3]");
    double sum = 0;
    for (double x : std::vector<double>{1,2,3}) { sum += x; }
    EXPECT_DOUBLE_EQ(sum, 6.0);
}

// --- 套件五：UnicodeTest（转义） ---
TEST(UnicodeTest, BasicPlane) {
    auto v = must_parse("[\"\\u0041\"]");
    EXPECT_EQ(v[0].get<std::string>(), "A");
}
TEST(UnicodeTest, CJK) {
    auto v = must_parse("[\"\\u4e2d\"]");
    EXPECT_EQ(v[0].get<std::string>().size(), 3u);   // "中" 是 3 字节 UTF-8
}
