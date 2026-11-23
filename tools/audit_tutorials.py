#!/usr/bin/env python3
"""Use CANNBot to audit and repair daily tutorials one day at a time."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any
from xml.sax.saxutils import escape as xml_escape

ROOT = Path(__file__).resolve().parent.parent
TUTORIALS_DIR = ROOT / "tutorials"
QUALITY_CHECKER = ROOT / "skills/ai-compiler-daily-tutorial/scripts/tutorial_tools.py"
DAY_DIR_RE = re.compile(r"day-(\d+)")
VALID_STATUSES = {"passed", "fixed", "failed"}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog=(
            "示例：\n"
            "  python3 tools/audit_tutorials.py\n"
            "  python3 tools/audit_tutorials.py --day 1 --day 2\n"
            "  python3 tools/audit_tutorials.py --start 1 --end 7 --resume\n"
            "  python3 tools/audit_tutorials.py --day 1 --check-only\n"
            "  python3 tools/audit_tutorials.py --dry-run"
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("--day", dest="days", type=int, action="append", help="只处理指定天数，可重复传入")
    parser.add_argument("--start", type=int, help="处理的起始天数（包含）")
    parser.add_argument("--end", type=int, help="处理的结束天数（包含）")
    parser.add_argument("--tutorials-dir", type=Path, default=TUTORIALS_DIR, help="教程根目录")
    parser.add_argument("--output-dir", type=Path, default=ROOT / "build/tutorial-audit", help="日志、渲染图和报告目录")
    parser.add_argument("--cannbot", default="cannbot", help="cannbot 可执行文件路径")
    parser.add_argument(
        "--model",
        default="cannbot/qwen3.8-max",
        help="支持图片的 provider/model（默认：cannbot/qwen3.8-max）",
    )
    parser.add_argument("--agent", help="传给 cannbot 的 agent 名称")
    parser.add_argument("--font-file", type=Path, help="用于 SVG 渲染的中文字体文件（如 NotoSansCJKsc-Regular.otf）")
    parser.add_argument("--timeout", type=int, default=1800, help="每天超时秒数，0 表示不限制（默认：1800）")
    parser.add_argument("--render-width", type=int, default=1920, help="SVG 检查图的 PNG 宽度（默认：1920）")
    parser.add_argument("--check-only", action="store_true", help="只审计，不允许 cannbot 修改教程")
    parser.add_argument("--resume", action="store_true", help="跳过已有 passed/fixed 报告的天数")
    parser.add_argument("--stop-on-error", action="store_true", help="某天失败后停止；默认继续处理后续天数")
    parser.add_argument("--dry-run", action="store_true", help="只列出将处理的天数，不调用 cannbot")
    args = parser.parse_args()

    if args.start is not None and args.end is not None and args.start > args.end:
        parser.error("--start 不能大于 --end")
    if args.timeout < 0:
        parser.error("--timeout 不能小于 0")
    if args.render_width <= 0:
        parser.error("--render-width 必须大于 0")
    return args


def discover_tutorials(root: Path) -> list[Path]:
    if not root.is_dir():
        raise ValueError(f"教程目录不存在：{root}")
    days = []
    for path in root.iterdir():
        match = DAY_DIR_RE.fullmatch(path.name)
        if path.is_dir() and match:
            if not (path / "README.md").is_file():
                raise ValueError(f"教程缺少 README.md：{path}")
            days.append(path)
    return sorted(days, key=lambda path: int(DAY_DIR_RE.fullmatch(path.name).group(1)))


def select_tutorials(all_days: list[Path], args: argparse.Namespace) -> list[Path]:
    by_number = {int(DAY_DIR_RE.fullmatch(path.name).group(1)): path for path in all_days}
    if args.days:
        requested = sorted(set(args.days))
        missing = [number for number in requested if number not in by_number]
        if missing:
            raise ValueError(f"找不到指定教程：{', '.join(f'Day {n}' for n in missing)}")
        selected = [by_number[number] for number in requested]
    else:
        selected = all_days

    if args.start is not None:
        selected = [path for path in selected if day_number(path) >= args.start]
    if args.end is not None:
        selected = [path for path in selected if day_number(path) <= args.end]
    if not selected:
        raise ValueError("筛选后没有需要处理的教程")
    return selected


def day_number(day_dir: Path) -> int:
    return int(DAY_DIR_RE.fullmatch(day_dir.name).group(1))


def run_quality_check(day_dir: Path) -> list[str]:
    completed = subprocess.run(
        [sys.executable, str(QUALITY_CHECKER), "check", str(day_dir / "README.md")],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        timeout=60,
        check=False,
    )
    if completed.returncode == 0:
        return []
    output = completed.stdout.strip()
    return [output or f"教程结构检查器退出码为 {completed.returncode}"]


def has_chinese_font() -> bool:
    fc_list = shutil.which("fc-list")
    if not fc_list:
        return True
    completed = subprocess.run(
        [fc_list, ":lang=zh", "file"],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        timeout=30,
        check=False,
    )
    return bool(completed.stdout.strip())


def renderer_environment(font_file: Path | None, output_dir: Path) -> dict[str, str] | None:
    if font_file is None:
        return None
    config_path = output_dir / "fontconfig.xml"
    cache_dir = output_dir / "font-cache"
    cache_dir.mkdir(parents=True, exist_ok=True)
    config_path.write_text(
        "<?xml version=\"1.0\"?>\n"
        "<!DOCTYPE fontconfig SYSTEM \"fonts.dtd\">\n"
        "<fontconfig>\n"
        "  <include ignore_missing=\"yes\">/etc/fonts/fonts.conf</include>\n"
        f"  <dir>{xml_escape(str(font_file.parent))}</dir>\n"
        f"  <cachedir>{xml_escape(str(cache_dir))}</cachedir>\n"
        "</fontconfig>\n",
        encoding="utf-8",
    )
    environment = os.environ.copy()
    environment["FONTCONFIG_FILE"] = str(config_path)
    return environment


def render_svgs(
    day_dir: Path,
    render_dir: Path,
    width: int,
    renderer: str,
    environment: dict[str, str] | None,
) -> list[str]:
    errors = []
    svg_files = sorted(day_dir.rglob("*.svg"))
    if not svg_files:
        return ["未找到 SVG 图片"]

    for svg in svg_files:
        output = render_dir / svg.relative_to(day_dir).with_suffix(".png")
        output.parent.mkdir(parents=True, exist_ok=True)
        output.unlink(missing_ok=True)
        completed = subprocess.run(
            [renderer, "--format", "png", "--width", str(width), "--output", str(output), str(svg)],
            cwd=ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            timeout=60,
            check=False,
            env=environment,
        )
        if completed.returncode != 0 or not output.is_file() or output.stat().st_size == 0:
            detail = completed.stdout.strip() or "没有生成有效的 PNG"
            errors.append(f"{svg.relative_to(ROOT)}：{detail}")
    return errors


def snapshot_files(day_dir: Path) -> dict[str, str]:
    snapshot = {}
    for path in sorted(day_dir.rglob("*")):
        if path.is_file():
            snapshot[str(path.relative_to(day_dir))] = hashlib.sha256(path.read_bytes()).hexdigest()
    return snapshot


def build_prompt(
    day_dir: Path,
    render_dir: Path,
    result_path: Path,
    check_only: bool,
    precheck_errors: list[str],
    render_errors: list[str],
) -> str:
    number = day_number(day_dir)
    relative_day = str(day_dir.relative_to(ROOT)) if day_dir.is_relative_to(ROOT) else str(day_dir)
    precheck = "\n".join(f"- {error}" for error in precheck_errors) or "- 无"
    rendering = "\n".join(f"- {error}" for error in render_errors) or "- 无；rendered/ 下已有每张 SVG 的高清 PNG"

    if check_only:
        edit_rule = (
            "本次为只检查模式：严禁修改任何教程或项目文件。只能写入下方指定的 JSON 报告，"
            "以及使用临时目录执行验证并在结束后清理。"
        )
    else:
        edit_rule = (
            f"只允许修改 {relative_day}/ 内确有问题的文件，以及写入下方指定的 JSON 报告。"
            "不要修改其他天数、学习计划、站点配置或工具；不要执行 git commit/push。"
        )

    return f"""请对仓库中的 Day {number} 教程做一次完整、独立的质量审计。工作目录是 {ROOT}，目标目录是 {day_dir}。
{edit_rule}

一、逐张检查并优化 SVG
1. 先阅读 {relative_day}/README.md，再逐张查看 images/ 下的 SVG 源码。
2. 渲染图位于 {render_dir}/，目录结构与教程目录对应。必须用图片查看工具逐张打开实际渲染图，并结合 SVG 源码检查；不得仅凭 XML 可解析就判断通过。
3. 重点检查文字互相重叠、文字与边框/箭头/图形重叠、内容裁切或越界、间距不足、字号过小、对齐混乱，以及图中技术关系是否错误。正常的背景、边框包含文字不算重叠。
4. 默认修复模式下，直接优化确认有问题的 SVG，保持项目现有手绘风格、配色和技术含义，不做无意义的全图重写。每次修改后重新渲染并再次打开图片复查，确保原问题消失且没有引入新重叠。
5. 如果预渲染失败，先根据 SVG 源码修复，再自行渲染复查。

二、校验教程内容是否正确
1. 对照 {ROOT / 'C++按天学习计划.md'} 中 Day {number} 的任务；需要时查看前一天和后一天，确认范围与衔接。阅读 {ROOT / 'skills/ai-compiler-daily-tutorial/references/quality-checklist.md'}。
2. 核查技术定义、代码、公式、命令、路径、图表数字、正文和输出是否一致。不能凭猜测把预期输出写成实测结果。
3. 在临时目录实际编译、运行当天可执行示例，并核对教程描述。教程明确用于演示编译/运行失败的示例，应确认它按描述失败，不要把故意失败误判为教程错误。验证后清理临时产物，不在教程目录留下二进制、缓存或测试输出。
4. 修复确认存在的技术错误、过时命令、图文矛盾和实践不可执行问题，保持原有教学结构，不做无关润色。无法确认或无法修复的问题必须列入 remaining_issues，不能隐瞒。
5. 不安装系统依赖，不执行有破坏性、会修改仓库历史或会发布内容的命令。

三、验证并写报告
完成后运行：python3 {QUALITY_CHECKER} check {day_dir / 'README.md'}。修复模式还需确保所有修改后的 SVG 都能重新渲染。

预检查结果：
{precheck}

预渲染结果：
{rendering}

最后必须用 UTF-8 将结果写入 {result_path}，只写合法 JSON，不要写 Markdown 报告。格式如下：
{{
  "day": {number},
  "status": "passed | fixed | failed",
  "svg_overlap_found": true,
  "svg_optimized": ["images/实际修改过的文件.svg"],
  "content_correct": true,
  "changes": ["实际修改及原因；只检查模式为空数组"],
  "remaining_issues": ["未解决的问题；没有则为空数组"],
  "checks": ["实际执行的检查及结果"]
}}
status 含义：passed=未发现问题且全部通过；fixed=发现的问题均已修复且复检通过；failed=仍有错误、无法完成检查或验证失败。content_correct 表示审计/修复完成后的状态。不要虚报通过。"""


def cannbot_command(args: argparse.Namespace, number: int, prompt: str) -> list[str]:
    command = [
        args.cannbot,
        "run",
        "--dir",
        str(ROOT),
        "--title",
        f"Tutorial audit Day {number}",
        "--dangerously-skip-permissions",
    ]
    if args.model:
        command.extend(["--model", args.model])
    if args.agent:
        command.extend(["--agent", args.agent])
    command.append(prompt)
    return command


def invoke_cannbot(
    command: list[str],
    log_path: Path,
    timeout: int,
    environment: dict[str, str] | None,
) -> str | None:
    with log_path.open("w", encoding="utf-8") as log:
        try:
            completed = subprocess.run(
                command,
                cwd=ROOT,
                stdout=log,
                stderr=subprocess.STDOUT,
                text=True,
                timeout=timeout or None,
                check=False,
                env=environment,
            )
        except subprocess.TimeoutExpired:
            return f"cannbot 超过 {timeout} 秒，已终止"
        except OSError as exc:
            return f"无法启动 cannbot：{exc}"
    if completed.returncode != 0:
        return f"cannbot 退出码为 {completed.returncode}"
    return None


def load_result(result_path: Path, number: int) -> tuple[dict[str, Any] | None, str | None]:
    if not result_path.is_file():
        return None, "cannbot 未生成结果 JSON"
    try:
        result = json.loads(result_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        return None, f"结果 JSON 无法读取：{exc}"
    if not isinstance(result, dict):
        return None, "结果 JSON 顶层必须是对象"
    if result.get("day") != number:
        return None, f"结果中的 day 应为 {number}"
    if result.get("status") not in VALID_STATUSES:
        return None, "结果中的 status 必须是 passed、fixed 或 failed"
    for field in ("svg_optimized", "changes", "remaining_issues", "checks"):
        if not isinstance(result.get(field), list):
            return None, f"结果中的 {field} 必须是数组"
    if not isinstance(result.get("svg_overlap_found"), bool):
        return None, "结果中的 svg_overlap_found 必须是布尔值"
    if not isinstance(result.get("content_correct"), bool):
        return None, "结果中的 content_correct 必须是布尔值"
    return result, None


def empty_result(number: int) -> dict[str, Any]:
    return {
        "day": number,
        "status": "failed",
        "svg_overlap_found": False,
        "svg_optimized": [],
        "content_correct": False,
        "changes": [],
        "remaining_issues": [],
        "checks": [],
    }


def audit_day(
    day_dir: Path,
    args: argparse.Namespace,
    renderer: str,
    environment: dict[str, str] | None,
) -> dict[str, Any]:
    number = day_number(day_dir)
    work_dir = args.output_dir / day_dir.name
    render_dir = work_dir / "rendered"
    result_path = work_dir / "result.json"
    log_path = work_dir / "cannbot.log"
    work_dir.mkdir(parents=True, exist_ok=True)
    result_path.unlink(missing_ok=True)

    print(f"\n[{day_dir.name}] 结构检查并渲染 SVG...", flush=True)
    precheck_errors = run_quality_check(day_dir)
    pre_render_errors = render_svgs(day_dir, render_dir, args.render_width, renderer, environment)
    before_snapshot = snapshot_files(day_dir) if args.check_only else None

    prompt = build_prompt(
        day_dir,
        render_dir,
        result_path,
        args.check_only,
        precheck_errors,
        pre_render_errors,
    )
    command = cannbot_command(args, number, prompt)
    print(f"[{day_dir.name}] 调用 cannbot；日志：{log_path}", flush=True)
    runner_errors = []
    process_error = invoke_cannbot(command, log_path, args.timeout, environment)
    if process_error:
        runner_errors.append(process_error)

    print(f"[{day_dir.name}] 复跑结构与 SVG 渲染检查...", flush=True)
    postcheck_errors = run_quality_check(day_dir)
    post_render_errors = render_svgs(day_dir, render_dir, args.render_width, renderer, environment)
    runner_errors.extend(postcheck_errors)
    runner_errors.extend(post_render_errors)

    if before_snapshot is not None and before_snapshot != snapshot_files(day_dir):
        runner_errors.append("只检查模式下教程文件发生了变化")

    result, result_error = load_result(result_path, number)
    if result_error:
        runner_errors.append(result_error)
        result = empty_result(number)
    if args.check_only and result.get("status") == "fixed":
        runner_errors.append("只检查模式不能使用 fixed 状态")
    if result.get("status") in {"passed", "fixed"} and result.get("remaining_issues"):
        runner_errors.append("通过状态与 remaining_issues 不一致")
    if result.get("status") in {"passed", "fixed"} and not result.get("content_correct"):
        runner_errors.append("通过状态与 content_correct=false 不一致")

    if runner_errors:
        result["status"] = "failed"
        remaining = result.setdefault("remaining_issues", [])
        for error in runner_errors:
            if error not in remaining:
                remaining.append(error)

    result["runner"] = {
        "precheck_errors": precheck_errors,
        "pre_render_errors": pre_render_errors,
        "postcheck_errors": postcheck_errors,
        "post_render_errors": post_render_errors,
        "cannbot_log": str(log_path.relative_to(ROOT)) if log_path.is_relative_to(ROOT) else str(log_path),
    }
    result_path.write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"[{day_dir.name}] {result['status']}", flush=True)
    return result


def write_summary(output_dir: Path, results: list[dict[str, Any]], check_only: bool) -> None:
    counts = {status: sum(result["status"] == status for result in results) for status in VALID_STATUSES}
    summary = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "check_only": check_only,
        "total": len(results),
        "counts": counts,
        "tutorials": [
            {
                "day": result["day"],
                "status": result["status"],
                "remaining_issues": result.get("remaining_issues", []),
            }
            for result in results
        ],
    }
    (output_dir / "summary.json").write_text(
        json.dumps(summary, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )


def main() -> int:
    args = parse_args()
    args.tutorials_dir = args.tutorials_dir.resolve()
    args.output_dir = args.output_dir.resolve()
    if args.font_file is not None:
        args.font_file = args.font_file.resolve()
    try:
        selected = select_tutorials(discover_tutorials(args.tutorials_dir), args)
    except (OSError, ValueError) as exc:
        print(f"错误：{exc}", file=sys.stderr)
        return 2

    if args.dry_run:
        print(f"将按顺序处理 {len(selected)} 天：")
        for day_dir in selected:
            print(f"- {day_dir.name}: {day_dir / 'README.md'}")
        print("未调用 cannbot，未修改文件。")
        return 0

    cannbot = shutil.which(args.cannbot)
    renderer = shutil.which("rsvg-convert")
    if not cannbot:
        print(f"错误：找不到 cannbot：{args.cannbot}", file=sys.stderr)
        return 2
    if not renderer:
        print("错误：找不到 rsvg-convert；请先安装 librsvg（提供 SVG 渲染工具）。", file=sys.stderr)
        return 2
    if args.font_file is not None and not args.font_file.is_file():
        print(f"错误：字体文件不存在：{args.font_file}", file=sys.stderr)
        return 2
    if args.font_file is None and not has_chinese_font():
        print(
            "错误：未检测到中文字体，渲染图会出现乱码，无法可靠检查重叠。"
            "请安装中文字体，或通过 --font-file 指定 Noto Sans CJK 等字体文件。",
            file=sys.stderr,
        )
        return 2
    args.cannbot = cannbot

    args.output_dir.mkdir(parents=True, exist_ok=True)
    environment = renderer_environment(args.font_file, args.output_dir)
    results = []
    for day_dir in selected:
        number = day_number(day_dir)
        result_path = args.output_dir / day_dir.name / "result.json"
        if args.resume and result_path.is_file():
            previous, error = load_result(result_path, number)
            if previous and previous["status"] in {"passed", "fixed"}:
                print(f"[{day_dir.name}] 跳过已有 {previous['status']} 报告")
                results.append(previous)
                continue

        result = audit_day(day_dir, args, renderer, environment)
        results.append(result)
        write_summary(args.output_dir, results, args.check_only)
        if result["status"] == "failed" and args.stop_on_error:
            break

    write_summary(args.output_dir, results, args.check_only)
    counts = {status: sum(result["status"] == status for result in results) for status in VALID_STATUSES}
    print(
        f"\n完成：共 {len(results)} 天，passed={counts['passed']}，"
        f"fixed={counts['fixed']}，failed={counts['failed']}"
    )
    print(f"汇总报告：{args.output_dir / 'summary.json'}")
    return 1 if counts["failed"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
