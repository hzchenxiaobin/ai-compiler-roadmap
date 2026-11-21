// Day 140：ASan + UBSan 全绿检查（计划指定：修复所有报告的问题）
// 被检对象：Day 138 满分的解析器（含 Day 139 优化落点后的版本）
// 方法论：探针先行——故意埋两类问题各一个，先看报告形态，再删探针验证干净
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <functional>
#include <cstdlib>
#include <limits>
#include <cmath>

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
};
struct Error { std::string msg; int line; };
namespace tk { constexpr int LB=0,RB=1,LK=2,RK=3,C=4,M=5,S=6,N=7,T_=8,F_=9,NU=10,E_=11; }
struct Tok { int type; std::string text; int line; };

std::optional<JsonValue> parse(const std::string& src, Error& err) {
    std::vector<Tok> toks; toks.reserve(src.size() / 6 + 16);   // Day 139 的 reserve
    std::size_t i = 0; int line = 1;
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
                        else if (e=='r') s+='\r'; else if (e=='b') s+='\b';
                        else if (e=='f') s+='\f';
                        else if (e=='"') s+='"'; else if (e=='\\') s+='\\';
                        else if (e=='/') s+='/';
                        else if (e=='u') {
                            if (i+6 <= src.size()) {
                                unsigned cp=0; bool ok=true;
                                for (int k=0;k<4;++k){char h=src[i+2+k];int v=(h<='9'&&h>='0')?h-'0':((h|32)>='a'&&(h|32)<='f')?(h|32)-'a'+10:-1;if(v<0){ok=false;break;}cp=cp*16+v;}
                                if (ok && (cp<0xD800||cp>0xDFFF)) {
                                    if (cp<0x80) s+=char(cp);
                                    else if (cp<0x800){s+=char(0xC0|(cp>>6));s+=char(0x80|(cp&0x3F));}
                                    else {s+=char(0xE0|(cp>>12));s+=char(0x80|((cp>>6)&0x3F));s+=char(0x80|(cp&0x3F));}
                                    i+=6; continue;
                                }
                            }
                            err = {"\\u 非法", line}; return std::nullopt;
                        }
                        else { err = {std::string("非法转义 \\")+e, line}; return std::nullopt; }
                        i += 2; continue;
                    }
                    unsigned char uc = static_cast<unsigned char>(src[i]);
                    if (uc < 0x20) { err = {"字符串含控制字符", line}; return std::nullopt; }
                    s += src[i++];
                }
                if (i >= src.size()) { err = {"字符串未闭合", line}; return std::nullopt; }
                ++i; push(tk::S, std::move(s)); break;
            }
            default: {
                if (c=='-'||(c>='0'&&c<='9')) {
                    std::string s;
                    while (i<src.size()&&((src[i]>='0'&&src[i]<='9')||src[i]=='.'||src[i]=='e'||src[i]=='E'||src[i]=='+'||src[i]=='-')) s+=src[i++];
                    { std::size_t first = (s[0]=='-') ? 1 : 0;
                      if (s.size() > first+1 && s[first]=='0' && s[first+1]>='0' && s[first+1]<='9') { err={"前导零", line}; return std::nullopt; } }
                    { std::size_t dot=s.find('.');
                      if (dot!=std::string::npos && (dot==0||dot+1>=s.size()||!isdigit((unsigned char)s[dot+1])||!isdigit((unsigned char)s[dot-1]))) { err={"畸形数字", line}; return std::nullopt; }
                      std::size_t ep=s.find_first_of("eE");
                      if (ep!=std::string::npos) { std::size_t d=ep+1; if(d<s.size()&&(s[d]=='+'||s[d]=='-'))++d;
                        if(d>=s.size()||!isdigit((unsigned char)s[d])) { err={"畸形数字", line}; return std::nullopt; } } }
                    char* end=nullptr; std::strtod(s.c_str(), &end);
                    if (end - s.c_str() != (long)s.size()) { err={"畸形数字", line}; return std::nullopt; }
                    push(tk::N, std::move(s));
                } else if (src.compare(i,4,"true")==0) { push(tk::T_); i+=4; }
                else if (src.compare(i,5,"false")==0) { push(tk::F_); i+=5; }
                else if (src.compare(i,4,"null")==0) { push(tk::NU); i+=4; }
                else { err = {std::string("意外字符 '")+c+"'", line}; return std::nullopt; }
            }
        }
    }
    push(tk::E_);
    std::size_t p = 0; int depth = 0;
    constexpr int kMaxDepth = 500;
    std::function<std::unique_ptr<JsonValue>()> value_impl = [&]() -> std::unique_ptr<JsonValue> {
        if (p >= toks.size()) { err = {"意外结束", line}; return std::make_unique<JsonValue>(); }
        int cur = toks[p].type;
        if (cur == tk::LB || cur == tk::LK) { if (++depth > kMaxDepth) { err = {"嵌套过深", line}; return std::make_unique<JsonValue>(); } }
        if (cur == tk::LB) {
            ++p; JsonObject obj;
            while (p < toks.size() && toks[p].type != tk::RB) {
                if (toks[p].type != tk::S) { err = {"期望键", toks[p].line}; return std::make_unique<JsonValue>(); }
                std::string k = toks[p].text; ++p;
                if (p >= toks.size() || toks[p].type != tk::C) { err = {"期望:", line}; return std::make_unique<JsonValue>(); }
                ++p;
                obj[k] = value_impl();
                if (p < toks.size() && toks[p].type == tk::M) { ++p; if (p<toks.size()&&(toks[p].type==tk::RB)) { err={"尾逗号", line}; return std::make_unique<JsonValue>(); } continue; }
                break;
            }
            if (p >= toks.size() || toks[p].type != tk::RB) { err = {"期望}", line}; return std::make_unique<JsonValue>(); }
            ++p; --depth; return std::make_unique<JsonValue>(std::move(obj));
        }
        if (cur == tk::LK) {
            ++p; JsonArray arr;
            while (p < toks.size() && toks[p].type != tk::RK) {
                arr.push_back(value_impl());
                if (p >= toks.size()) { err = {"意外结束", line}; return std::make_unique<JsonValue>(); }
                if (toks[p].type == tk::M) { ++p; if (p<toks.size()&&toks[p].type==tk::RK) { err={"尾逗号", line}; return std::make_unique<JsonValue>(); } continue; }
                break;
            }
            if (p >= toks.size() || toks[p].type != tk::RK) { err = {"期望]", line}; return std::make_unique<JsonValue>(); }
            ++p; --depth; return std::make_unique<JsonValue>(std::move(arr));
        }
        if (cur == tk::S) return std::make_unique<JsonValue>(toks[p++].text);
        if (cur == tk::N) { std::string n=toks[p++].text;
            try { return std::make_unique<JsonValue>(std::stod(n)); }
            catch (...) { return std::make_unique<JsonValue>(std::numeric_limits<double>::infinity()); } }
        if (cur == tk::T_) { ++p; return std::make_unique<JsonValue>(true); }
        if (cur == tk::F_) { ++p; return std::make_unique<JsonValue>(false); }
        if (cur == tk::NU) { ++p; return std::make_unique<JsonValue>(); }
        err = {"此处应是值", toks[p].line}; ++p;
        return std::make_unique<JsonValue>();
    };
    if (toks.empty() || toks[0].type == tk::E_) { err = {"空输入", line}; return std::nullopt; }
    auto v = value_impl();
    if (!err.msg.empty() || p >= toks.size() || toks[p].type != tk::E_) {
        if (err.msg.empty()) { err = {"值后多余", line}; }
        return std::nullopt;
    }
    return JsonValue(std::move(*v));
}
} // namespace json

// ===== 探针区（先埋后删——看报告形态） =====
void asan_probe() {
#ifdef ENABLE_PROBES
    int* heap = new int[3]{1,2,3};  // ASan 双探针：泄漏 + 越界（堆）
    volatile int x = heap[5];       // 越界读（运行时由 ASan 抓）
    (void)x;                         // （不 delete——泄漏也在报告里）
#endif
}
int ubsan_probe(int i) {
#ifdef ENABLE_PROBES
    return 1000000 * i;             // UBSan 有符号溢出探针
#else
    return i;
#endif
}

int main(int argc, char* argv[]) {
    bool probes = (argc > 1 && std::string(argv[1]) == "--probes");
    if (probes) {
        std::cout << "== 探针模式（故意埋雷看报告） ==" << std::endl;
        asan_probe();
        std::cout << "  ubsan_probe(1000000) = " << ubsan_probe(1000000) << std::endl;
        std::cout << "  （编译时 -DENABLE_PROBES + sanitizers——见报告）" << std::endl;
        return 0;
    }

    std::cout << "Day 140：ASan + UBSan 全绿检查\n" << std::endl;

    // 被检的输入集：正常 + 边界 + 畸形（覆盖各代码路径）
    const char* inputs[] = {
        "{\"name\": \"Ada\", \"scores\": [95, 88.5], \"ok\": true, \"tag\": null}",
        "[\"含\\n转义\", \"\\u4e2d\\u6587\"]",
        "[[[[[1]]]]]",
        "{}", "[]", "null", "true", "42", "-7.5e2",
        // 畸形（错误路径）
        "", "[@]", "[\"unclosed", "[1.2.3]", "{\"a\" 1}", "[1,2,]",
        "[\"\\uZZZZ\"]", "[\"\\x\"]",
    };
    int ok = 0, rejected = 0;
    for (const char* s : inputs) {
        json::Error err;
        auto v = json::parse(s, err);
        if (v) { ++ok; std::cout << "  [OK]  " << (strlen(s) < 40 ? s : std::string(s).substr(0,37)+"…") << " → " << v->type_name() << "\n"; }
        else { ++rejected; std::cout << "  [拒]  " << (strlen(s) < 40 ? s : std::string(s).substr(0,37)+"…") << "（" << err.msg << "）\n"; }
    }
    std::cout << "\n  合计：" << ok << " 接受 / " << rejected << " 拒绝（错误路径也跑）" << std::endl;

    // 大输入（Day 139 的基准负载——树构建/析构全路径）
    {
        std::string big = "{";
        for (int k = 0; k < 5000; ++k) {
            if (k) big += ',';
            big += "\"key" + std::to_string(k) + "\":" + std::to_string(k * 1.5);
        }
        big += '}';
        json::Error err;
        auto v = json::parse(big, err);
        std::cout << "  大输入（" << big.size() << " B / 5000 键）：" << (v ? "OK " + std::string(v->type_name()) : "失败 " + err.msg) << "\n";
    }

    std::cout << "\n== 检查的意义 ==\n";
    std::cout << "  ASan：内存错误（越界/泄漏/UAF）的运行时执法\n";
    std::cout << "  UBSan：未定义行为（有符号溢出/位移越界）的静态风格检查\n";
    std::cout << "  （两San合璧——错误路径全覆盖才敢说'全绿'）\n";
    return 0;
}
