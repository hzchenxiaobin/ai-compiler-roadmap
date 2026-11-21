#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Day 130：字符串转义完整版——\n \" \\ \uXXXX（计划指定）
struct EscResult {
    std::string decoded;
    std::string error;
    int chars_consumed = 0;
};

int hex_val(char c) {
    if (c >= '0' && c <= '9') { return c - '0'; }
    if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
    if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
    return -1;
}

bool parse_u(const std::string& s, std::size_t pos, uint32_t& cp) {
    if (pos + 4 > s.size()) { return false; }
    uint32_t v = 0;
    for (int k = 0; k < 4; ++k) {
        int h = hex_val(s[pos + k]);
        if (h < 0) { return false; }
        v = v * 16 + static_cast<uint32_t>(h);
    }
    cp = v;
    return true;
}

void append_utf8(std::string& out, uint32_t cp) {
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    } else if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
}

EscResult decode_escape(const std::string& s, std::size_t p) {
    EscResult r;
    if (p + 1 >= s.size()) { r.error = "字符串以反斜杠结尾"; return r; }
    char e = s[p + 1];
    switch (e) {
        case 'n': r.decoded += '\n'; r.chars_consumed = 2; return r;
        case 't': r.decoded += '\t'; r.chars_consumed = 2; return r;
        case 'r': r.decoded += '\r'; r.chars_consumed = 2; return r;
        case 'b': r.decoded += '\b'; r.chars_consumed = 2; return r;
        case 'f': r.decoded += '\f'; r.chars_consumed = 2; return r;
        case '"': r.decoded += '"';  r.chars_consumed = 2; return r;
        case '\\': r.decoded += '\\'; r.chars_consumed = 2; return r;
        case '/': r.decoded += '/';  r.chars_consumed = 2; return r;
        case 'u': {
            uint32_t cp;
            if (!parse_u(s, p + 2, cp)) { r.error = "\\u 后不是四位十六进制"; return r; }
            if (cp >= 0xD800 && cp <= 0xDBFF) {
                if (p + 6 + 6 <= s.size() && s[p + 6] == '\\' && s[p + 7] == 'u') {
                    uint32_t lo;
                    if (parse_u(s, p + 8, lo) && lo >= 0xDC00 && lo <= 0xDFFF) {
                        uint32_t full = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                        append_utf8(r.decoded, full);
                        r.chars_consumed = 12;
                        return r;
                    }
                }
                r.error = "孤立的高代理 \\uD800-\\uDBFF";
                return r;
            }
            if (cp >= 0xDC00 && cp <= 0xDFFF) {
                r.error = "孤立的低代理 \\uDC00-\\uDFFF";
                return r;
            }
            append_utf8(r.decoded, cp);
            r.chars_consumed = 6;
            return r;
        }
        default:
            r.error = std::string("非法转义 \\") + e;
            return r;
    }
}

bool decode_string(const std::string& raw, std::string& out, std::string& err) {
    out.clear();  err.clear();
    std::size_t i = 0;
    while (i < raw.size()) {
        if (raw[i] == '\\') {
            EscResult r = decode_escape(raw, i);
            if (!r.error.empty()) { err = r.error; return false; }
            out += r.decoded;
            i += static_cast<std::size_t>(r.chars_consumed);
        } else {
            out += raw[i++];
        }
    }
    return true;
}

int main() {
    std::cout << "Day 130：字符串转义完整版\n" << std::endl;

    std::cout << "== 基础八转义 ==" << std::endl;
    const char* basic_inputs[] = {"\\n", "\\t", "\\r", "\\b", "\\f", "\\\"", "\\\\", "\\/"};
    const char* basic_names[]  = {"换行", "制表", "回车", "退格", "换页", "双引号", "反斜杠", "斜杠"};
    for (int k = 0; k < 8; ++k) {
        std::string o, e2;
        if (decode_string(basic_inputs[k], o, e2)) {
            std::cout << "  " << basic_names[k] << "：解出 " << o.size() << " 字节（首字节 0x"
                      << std::hex << (o.empty() ? 0 : static_cast<int>(static_cast<unsigned char>(o[0]))) << "）" << std::dec << std::endl;
        }
    }

    std::cout << "\n== \\uXXXX 码点（BMP） ==" << std::endl;
    const char* u_inputs[] = {"\\u0041", "\\u4e2d", "\\u00e9", "\\u4e16\\u754c"};
    const char* u_labels[] = {"字母 A", "汉字 中", "é（两字节）", "世界（两码点）"};
    for (int k = 0; k < 4; ++k) {
        std::string o, e2;
        if (decode_string(u_inputs[k], o, e2)) {
            std::cout << "  \"" << u_inputs[k] << "\" → \"" << o << "\"（" << u_labels[k]
                      << "，" << o.size() << " 字节）" << std::endl;
        }
    }

    std::cout << "\n== 代理对（emoji 的真相） ==" << std::endl;
    std::string out, err;
    if (decode_string("\\uD83D\\uDE00", out, err)) {
        std::cout << "  \\uD83D\\uDE00 → " << out.size() << " 字节 UTF-8：" << out << "（😀）" << std::endl;
        std::cout << "  （两个 \\u 代理对 → 一个 4 字节字符——UTF-16 的历史包袱）" << std::endl;
    }

    std::cout << "\n== 混合转义 ==" << std::endl;
    if (decode_string("line1\\n\\u4e2d\\t\"q\"\\\\e", out, err)) {
        std::cout << "  解码结果（" << out.size() << " 字节）：" << std::endl;
        std::cout << "  \"" << out << "\"" << std::endl;
    }

    std::cout << "\n== 错误四例 ==" << std::endl;
    const char* bads[] = {"\\x41", "\\u12", "\\uD83D", "\\uDE00"};
    for (const char* b : bads) {
        decode_string(b, out, err);
        std::cout << "  \"" << b << "\" → [错误] " << err << std::endl;
    }

    std::cout << "\n== 今天新增的深度 ==" << std::endl;
    std::cout << "  八转义 → \\uXXXX（BMP 三档 UTF-8）→ 代理对（4 字节档）" << std::endl;
    std::cout << "  （昨天的五转义 → 今天的全量 JSON 转义集 ✔）" << std::endl;
    return 0;
}
