#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// Day 131：数字解析——整数/浮点/科学计数法（strtod 严格文法验证）
struct NumResult {
    double value = 0;
    bool ok = false;
    std::string error;
    std::size_t consumed = 0;   // strtod 吃掉的字符数
};

// 用 strtod 验证：能解析多少字符？剩余的是不是"畸形证据"？
NumResult parse_number(const std::string& s) {
    NumResult r;
    if (s.empty()) { r.error = "空串"; return r; }

    errno = 0;
    char* end = nullptr;
    double v = std::strtod(s.c_str(), &end);

    std::size_t used = static_cast<std::size_t>(end - s.c_str());
    r.consumed = used;

    if (used == 0) {
        r.error = "strtod 没吃任何字符（不是数字开头）";
        return r;
    }
    if (used < s.size()) {
        r.error = "吃了一半：剩 \"" + s.substr(used) + "\" 没消费——畸形数字";
        // 还是把已解析的值给出（教学展示）
        r.value = v;
        return r;
    }
    if (errno == ERANGE) {
        r.error = "溢出（超 double 范围）";
        r.value = v;
        return r;
    }
    r.value = v;
    r.ok = true;
    return r;
}

// 额外检查 JSON 文法的两条限制（比 strtod 更严）
std::string json_extra_check(const std::string& s) {
    // JSON 不允许：前导零（01）、加号开头（+1）、点开头（.5）、点结尾（5.）
    if (s.size() > 1 && s[0] == '0' && std::isdigit(static_cast<unsigned char>(s[1]))) {
        return "JSON 不允许前导零（01）";
    }
    if (!s.empty() && s[0] == '+') {
        return "JSON 不允许 + 开头";
    }
    if (!s.empty() && s[0] == '.') {
        return "JSON 不允许 . 开头（要 0.5）";
    }
    if (s.size() > 0 && s.back() == '.') {
        return "JSON 不允许 . 结尾";
    }
    return "";
}

int main() {
    std::cout << "Day 131：数字解析——严格文法\n" << std::endl;

    std::cout << "== 合法形态（strtod 全吃 ✔） ==" << std::endl;
    const char* goods[] = {"0", "-1", "3.14", "1e5", "2.5E-3", "42",
                            "-0.5", "1.602e-19", "6.02e23"};
    for (const char* g : goods) {
        NumResult r = parse_number(g);
        std::string extra = json_extra_check(g);
        if (r.ok && extra.empty()) {
            std::cout << "  \"" << g << "\" → " << r.value << "（吃 " << r.consumed << "/" << strlen(g) << "）✔" << std::endl;
        }
    }

    std::cout << "\n== 畸形：吃一半（Day 129 的遗留） ==" << std::endl;
    const char* halfs[] = {"1.2.3", "1e", "1e+", "12abc", "3.14.15"};
    for (const char* h : halfs) {
        NumResult r = parse_number(h);
        std::cout << "  \"" << h << "\" → [拒] " << r.error << std::endl;
    }

    std::cout << "\n== strtod 认但 JSON 不认 ==" << std::endl;
    const char* notjson[] = {"01", "+1", ".5", "5.", "0x1A", "infinity", "nan"};
    for (const char* n : notjson) {
        NumResult r = parse_number(n);
        std::string extra = json_extra_check(n);
        std::cout << "  \"" << n << "\"：strtod ";
        if (r.ok) { std::cout << "✔（" << r.value << "）"; } else { std::cout << "✘"; }
        if (!extra.empty()) { std::cout << "；但 JSON 拒：" << extra; }
        else if (r.ok) {
            std::string sn(n);
            if (sn == "0x1A" || sn == "infinity" || sn == "nan") {
                std::cout << "；JSON 文法没有这些（hex/inf/nan）";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "\n== 溢出边界 ==" << std::endl;
    const char* bigs[] = {"1e400", "-1e400", "1e-400"};
    for (const char* b : bigs) {
        NumResult r = parse_number(b);
        std::cout << "  \"" << b << "\" → ";
        if (r.ok) { std::cout << r.value << " ✔"; }
        else if (errno == ERANGE) { std::cout << r.value << "（" << r.error << "）"; }
        else { std::cout << "[拒] " << r.error; }
        std::cout << std::endl;
    }

    std::cout << "\n== 整数/浮点之分（存成 double 的取舍） ==" << std::endl;
    std::cout << "  42 → " << parse_number("42").value << "（整数也存 double）" << std::endl;
    std::cout << "  精度：2^53 = " << 9007199254740992.0 << "（之内整数无损）" << std::endl;
    std::cout << "  9007199254740993（2^53+1）→ " << parse_number("9007199254740993").value
              << "（！损失——double 的边界）" << std::endl;
    std::cout << "  （JSON 规范允许实现选 double——教学解析器的取舍）" << std::endl;

    std::cout << "\n== Day 129 遗留问题的解决 ==" << std::endl;
    std::cout << "  \"1.2.3\"：昨天字符集放行 → 今天 strtod 吃一半 → 拒收 ✔" << std::endl;
    std::cout << "  （词法层的第二道防线——严格文法）" << std::endl;
    return 0;
}
