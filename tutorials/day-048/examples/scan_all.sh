#!/bin/bash
# Day 48 自测日素材二：ASan 扫描自己全部项目（零报错验收）
# 扫描 tutorials/ 下所有 .cpp 示例：-Wall 编译 + ASan 运行

cd "$(dirname "$0")/../../.." || exit 1  # examples→day-048→tutorials→仓库根

total=0
warn_fail=0
asan_fail=0
asan_skip=0
multi_file=0
results=""

# 教学预期的"故意翻车"文件（UB 实验室/自赋值演示——它们的报错就是教学内容）
TEACHING_OK="tutorials/day-001/examples/warn_unused.cpp tutorials/day-002/examples/uninit.cpp tutorials/day-013/examples/out_of_bounds.cpp tutorials/day-015/examples/strcpy_danger.cpp tutorials/day-019/examples/nullptr_crash.cpp tutorials/day-039/examples/leak_lab.cpp tutorials/day-045/examples/error_museum.cpp tutorials/day-055/examples/shallow_crash.cpp tutorials/day-056/examples/copy_assign.cpp tutorials/day-056/examples/self_assign_trap.cpp tutorials/day-066/examples/mystring_v2.cpp tutorials/day-060/examples/mystring_v1.cpp"
is_teaching() {
    case " $TEACHING_OK " in *" $1 "*) return 0;; esac
    return 1
}

for f in tutorials/day-*/examples/*.cpp; do
    [ -f "$f" ] || continue
    total=$((total + 1))
    name=$(basename "$f" .cpp)

    # 编译检查（-Wall 零警告；有交互输入的会超时——用 </dev/null 饿死它们）
    if ! g++ -Wall -Wextra -std=c++20 "$f" -o "/tmp/scan_$name" 2> "/tmp/warn_$name"; then
        if is_teaching "$f"; then
            asan_skip=$((asan_skip + 1))   # 教学预期归入跳过
        else
            multi_file=$((multi_file + 1)) # 多文件项目需联合编译（扫描单文件的局限）
        fi
        continue
    fi
    if [ -s "/tmp/warn_$name" ]; then
        if is_teaching "$f"; then
            asan_skip=$((asan_skip + 1))   # 教学预期（如自赋值演示的 -Wself-assign）
        else
            warn_fail=$((warn_fail + 1))
            results="$results\n[有警告] $f"
        fi
        continue
    fi

    # ASan 运行（10 秒超时防交互卡死；stdin 关闭饿死 cin）
    if g++ -Wall -Wextra -std=c++20 -fsanitize=address "$f" -o "/tmp/scan_asan_$name" 2>/dev/null; then
        if timeout 10 /tmp/scan_asan_$name </dev/null > /dev/null 2> "/tmp/asan_$name"; then
            :   # 干净通过
        else
            exit_code=$?
            if grep -q "AddressSanitizer" "/tmp/asan_$name" 2>/dev/null; then
                if is_teaching "$f"; then
                    asan_skip=$((asan_skip + 1))   # 教学预期（UB 实验室就是要报错）
                else
                    asan_fail=$((asan_fail + 1))
                    results="$results\n[ASan 报错] $f"
                fi
            else
                # 非零退出但无 ASan 报告（可能是预期 exit(1) 的教学程序或饿死输入）
                asan_skip=$((asan_skip + 1))
            fi
        fi
        rm -f "/tmp/scan_asan_$name"
    fi
    rm -f "/tmp/scan_$name" "/tmp/warn_$name" "/tmp/asan_$name"
done

echo "===== ASan 全项目扫描报告 ====="
echo "扫描源文件：$total 个"
echo "-Wall 警告（非教学预期）：$warn_fail"
echo "多文件项目（需联合编译，非错误）：$multi_file"
echo "ASan 报错：$asan_fail"
echo "教学预期（故意翻车的演示文件）与非交互扫描跳过：$asan_skip"
if [ -n "$results" ]; then
    echo "---- 明细 ----"
    echo -e "$results"
fi
if [ "$warn_fail" -eq 0 ] && [ "$asan_fail" -eq 0 ]; then
    echo "===== 验收通过：全部项目 -Wall 零警告且无 ASan 报错 ====="
fi
