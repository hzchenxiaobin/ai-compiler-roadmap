// Day 138：用公开测试集（JSONTestSuite）验证正确性（计划指定）
// 测试集：github.com/nst/JSONTestSuite 的 test_parsing（318 个文件）
//   y_*.json（95 个）：必须接受
//   n_*.json（188 个）：必须拒绝
//   i_*.json（35 个）：实现自定（边界模糊）
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <functional>
#include <cmath>
#include <limits>
#include <cstdlib>

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
};

struct Error { std::string msg; int line; };

namespace tk { constexpr int LB=0,RB=1,LK=2,RK=3,C=4,M=5,S=6,N=7,T_=8,F_=9,NU=10,E_=11; }
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
                        else if (e=='r') s+='\r'; else if (e=='b') s+='\b';
                        else if (e=='f') s+='\f';
                        else if (e=='"') s+='"'; else if (e=='\\') s+='\\';
                        else if (e=='/') s+='/';
                        else if (e=='u') {
                            if (i+6 <= src.size()) {
                                unsigned cp=0; bool ok=true;
                                for (int k=0;k<4;++k){char h=src[i+2+k];int v=(h<='9'&&h>='0')?h-'0':((h|32)>='a'&&(h|32)<='f')?(h|32)-'a'+10:-1;if(v<0){ok=false;break;}cp=cp*16+v;}
                                if (ok) {
                                    if (cp>=0xD800&&cp<=0xDBFF&&i+12<=src.size()&&src[i+6]=='\\'&&src[i+7]=='u') {
                                        unsigned lo=0; bool ok2=true;
                                        for(int k=0;k<4;++k){char h=src[i+8+k];int v=(h<='9'&&h>='0')?h-'0':((h|32)>='a'&&(h|32)<='f')?(h|32)-'a'+10:-1;if(v<0){ok2=false;break;}lo=lo*16+v;}
                                        if(ok2&&lo>=0xDC00&&lo<=0xDFFF){
                                            unsigned full=0x10000+((cp-0xD800)<<10)+(lo-0xDC00);
                                            s+=char(0xF0|(full>>18)); s+=char(0x80|((full>>12)&0x3F));
                                            s+=char(0x80|((full>>6)&0x3F)); s+=char(0x80|(full&0x3F));
                                            i+=12; continue;
                                        }
                                    }
                                    if (cp<0xD800||cp>0xDFFF) {
                                        if (cp<0x80) s+=char(cp);
                                        else if (cp<0x800){s+=char(0xC0|(cp>>6));s+=char(0x80|(cp&0x3F));}
                                        else {s+=char(0xE0|(cp>>12));s+=char(0x80|((cp>>6)&0x3F));s+=char(0x80|(cp&0x3F));}
                                        i+=6; continue;
                                    }
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
                    {   // 前导零：跳过可选负号后的首个数字是 0 且后续还有数字 → 非法
                        std::size_t first = (s[0]=='-') ? 1 : 0;
                        if (s.size() > first + 1 && s[first]=='0' && s[first+1]>='0' && s[first+1]<='9') {
                            err = {"前导零", line}; return std::nullopt;
                        }
                    }
                    if (s=="-"||s=="+") { err = {"数字缺数字", line}; return std::nullopt; }
                    {   // JSON 数字文法检查：. 前后都要数字；e 后要数字；- 后要数字
                        std::size_t dot = s.find('.');
                        if (dot != std::string::npos) {
                            if (dot == 0 || dot + 1 >= s.size() || !isdigit(static_cast<unsigned char>(s[dot+1]))
                                || !isdigit(static_cast<unsigned char>(s[dot-1]))) {
                                err = {"畸形数字 "+s, line}; return std::nullopt;
                            }
                        }
                        std::size_t epos = s.find_first_of("eE");
                        if (epos != std::string::npos) {
                            std::size_t d = epos + 1;
                            if (d < s.size() && (s[d]=='+'||s[d]=='-')) { ++d; }
                            if (d >= s.size() || !isdigit(static_cast<unsigned char>(s[d]))) {
                                err = {"畸形数字 "+s, line}; return std::nullopt;
                            }
                        }
                        if (s[0]=='-' && (s.size()<2 || !isdigit(static_cast<unsigned char>(s[1])))) {
                            err = {"畸形数字 "+s, line}; return std::nullopt;
                        }
                    }
                    char* end=nullptr; std::strtod(s.c_str(), &end);
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
    int depth = 0;
    constexpr int kMaxDepth = 500;   // 深度上限（生产级惯例——超深嵌套拒绝）
    std::function<std::unique_ptr<JsonValue>()> value_impl = [&]() -> std::unique_ptr<JsonValue> {
        if (p >= toks.size()) { err = {"意外结束", line}; return std::make_unique<JsonValue>(); }
        int cur = toks[p].type;
        if (cur == tk::LB || cur == tk::LK) {
            if (++depth > kMaxDepth) { err = {"嵌套过深（>" + std::to_string(kMaxDepth) + "层）", line}; return std::make_unique<JsonValue>(); }
        }
        if (cur == tk::LB) {
            ++p;
            JsonObject obj;
            while (p < toks.size() && toks[p].type != tk::RB) {
                if (toks[p].type != tk::S) { err = {"期望键", toks[p].line}; return std::make_unique<JsonValue>(); }
                std::string k = toks[p].text; ++p;
                if (p >= toks.size() || toks[p].type != tk::C) { err = {"期望:", line}; return std::make_unique<JsonValue>(); }
                ++p;
                obj[k] = value_impl();
                if (p < toks.size() && toks[p].type == tk::M) {
                    ++p;
                    if (p < toks.size() && toks[p].type == tk::RB) { err = {"尾逗号", line}; return std::make_unique<JsonValue>(); }
                    continue;
                }
                break;
            }
            if (p >= toks.size() || toks[p].type != tk::RB) { err = {"期望}", line}; return std::make_unique<JsonValue>(); }
            ++p; --depth;
            return std::make_unique<JsonValue>(std::move(obj));
        }
        if (cur == tk::LK) {
            ++p;
            JsonArray arr;
            while (p < toks.size() && toks[p].type != tk::RK) {
                arr.push_back(value_impl());
                if (err.msg == "意外结束") { return std::make_unique<JsonValue>(); }
                if (p < toks.size() && toks[p].type == tk::M) {
                    ++p;
                    if (p < toks.size() && toks[p].type == tk::RK) { err = {"尾逗号", line}; return std::make_unique<JsonValue>(); }
                    continue;
                }
                break;
            }
            if (p >= toks.size() || toks[p].type != tk::RK) { err = {"期望]", line}; return std::make_unique<JsonValue>(); }
            ++p; --depth;
            return std::make_unique<JsonValue>(std::move(arr));
        }
        if (cur == tk::S) { return std::make_unique<JsonValue>(toks[p++].text); }
        if (cur == tk::N) {
            std::string n = toks[p++].text;
            try { return std::make_unique<JsonValue>(std::stod(n)); }
            catch (...) { return std::make_unique<JsonValue>(std::numeric_limits<double>::infinity()); }
        }
        if (cur == tk::T_) { ++p; return std::make_unique<JsonValue>(true); }
        if (cur == tk::F_) { ++p; return std::make_unique<JsonValue>(false); }
        if (cur == tk::NU) { ++p; return std::make_unique<JsonValue>(); }
        err = {"此处应是值", toks[p].line};
        ++p;
        return std::make_unique<JsonValue>();
    };
    auto v = value_impl();
    if (!err.msg.empty() || toks[p].type != tk::E_) {
        err = err.msg.empty() ? Error{"值后多余", toks[p].line} : err;
        return std::nullopt;
    }
    return JsonValue(std::move(*v));
}
} // namespace json

int main(int argc, char* argv[]) {
    if (argc < 2) { std::cerr << "用法: " << argv[0] << " <测试目录>\n"; return 1; }
    std::string dir = argv[1];

    int y_pass=0, y_fail=0, n_pass=0, n_fail=0, i_pass=0, i_fail=0;
    std::vector<std::string> fails;

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::string name = entry.path().filename().string();
        if (!name.ends_with(".json")) { continue; }
        std::ifstream f(entry.path(), std::ios::binary);
        std::stringstream ss; ss << f.rdbuf();
        std::string src = ss.str();

        json::Error err;
        auto v = json::parse(src, err);
        bool accepted = v.has_value();
        bool err_reported = !err.msg.empty();

        char kind = name[0];   // y / n / i
        bool expected = (kind == 'y');
        if (kind == 'i') {
            // 实现自定——只统计不判
            if (accepted) { ++i_pass; } else { ++i_fail; }
            continue;
        }
        if (accepted == expected) {
            (kind=='y') ? ++y_pass : ++n_pass;
        } else {
            (kind=='y') ? ++y_fail : ++n_fail;
            fails.push_back(std::string(kind=='y' ? "[应受却拒] " : "[应拒却受] ") + name
                            + "（" + (err_reported ? err.msg : "-") + "）");
        }
    }

    std::cout << "== JSONTestSuite test_parsing 结果 ==\n";
    std::cout << "  y_（必须接受 95 个）：过 " << y_pass << " / 败 " << y_fail << "\n";
    std::cout << "  n_（必须拒绝 188 个）：过 " << n_pass << " / 败 " << n_fail << "\n";
    std::cout << "  i_（实现自定 35 个）：受 " << i_pass << " / 拒 " << i_fail << "（不判对错）\n";
    std::cout << "  总正确率（y+n 判分）："
              << (y_pass + n_pass) * 100 / (y_pass + y_fail + n_pass + n_fail) << "%\n";
    if (!fails.empty()) {
        std::cout << "\n失败清单（前 12）：\n";
        for (std::size_t k = 0; k < fails.size() && k < 12; ++k) { std::cout << "  " << fails[k] << "\n"; }
    }
    return 0;
}
