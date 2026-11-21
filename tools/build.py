#!/usr/bin/env python3
"""Build the daily tutorials as a static GitHub Pages website."""

import html
import re
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
TUTORIALS_DIR = ROOT / "tutorials"
PLAN_PATH = ROOT / "C++按天学习计划.md"
PUBLIC_DIR = ROOT / "public"
STATIC_DIR = ROOT / "static"
GITHUB_REPO_URL = "https://github.com/hzchenxiaobin/ai-compiler-roadmap"
SITE_URL = "https://hzchenxiaobin.github.io/ai-compiler-roadmap"
SITE_NAME = "C++ 从基础到深入学习计划"
SITE_TITLE = "C++ 144 天学习计划"
TITLE_RE = re.compile(r"^#\s+第\s*(\d+)\s*天[：:]\s*(.+)$")
IMAGE_RE = re.compile(r"!\[[^\]]*\]\(([^)]+)\)")
PHASE_RE = re.compile(r"^## 阶段([一二三四五六])：(.+?)（第 (\d+)~(\d+) 周）\s*$")
WEEK_RE = re.compile(r"^### 第 (\d+)(?:~(\d+))? 周 · (.+?)\s*$")
DAY_ITEM_RE = re.compile(r"^- \[.\] \*\*Day \d+\*\*[^\n]*?[｜|](.+)$")
MILESTONE_RE = re.compile(
    r"^\| ([一二三四五六])、(.+?) \| 第 \d+~\d+ 周 \| Day (\d+)~(\d+) \| (.+?) \|$", re.MULTILINE
)


def escape_for_template_string(text):
    return (
        text.replace("\\", "\\\\")
        .replace("`", "\\`")
        .replace("${", "\\${")
        .replace("</script>", "\\x3c/script>")
    )


def load_tutorials():
    tutorials = []
    for readme in sorted(TUTORIALS_DIR.glob("day-*/README.md")):
        day_dir = readme.parent
        try:
            directory_day = int(day_dir.name.removeprefix("day-"))
        except ValueError as exc:
            raise ValueError(f"教程目录名称无效：{day_dir.name}") from exc

        text = readme.read_text(encoding="utf-8")
        lines = text.strip().splitlines()
        match = TITLE_RE.match(lines[0]) if lines else None
        if not match:
            raise ValueError(f"无法从首行解析教程标题：{readme}")
        if int(match.group(1)) != directory_day:
            raise ValueError(f"教程目录与标题天数不一致：{readme}")

        for target in IMAGE_RE.findall(text):
            target = target.strip().split()[0].strip("<>")
            if re.match(r"^[a-zA-Z][a-zA-Z0-9+.-]*:", target):
                continue
            if not (day_dir / target).is_file():
                raise FileNotFoundError(f"教程图片不存在：{day_dir / target}")

        tutorials.append(
            {
                "number": directory_day,
                "slug": day_dir.name,
                "title": f"第 {directory_day} 天：{match.group(2).strip()}",
                "markdown": "\n".join(lines[1:]).strip(),
                "source": day_dir,
            }
        )

    if not tutorials:
        raise ValueError("没有找到 tutorials/day-N/README.md")
    return tutorials


def load_plan_phases():
    """从计划文档解析阶段、周与里程碑，驱动首页学习路线区。"""
    plan_text = PLAN_PATH.read_text(encoding="utf-8")
    milestones = {
        ordinal: output.strip()
        for ordinal, _, _, _, output in MILESTONE_RE.findall(plan_text)
    }

    phases = []
    current = None
    for line in plan_text.splitlines():
        phase_match = PHASE_RE.match(line)
        if phase_match:
            ordinal, title, start_week, _ = phase_match.groups()
            current = {
                "label": f"阶段{ordinal}",
                "title": title.strip(),
                "weeks": [],
                "milestone": milestones.get(ordinal, ""),
            }
            phases.append(current)
            continue
        week_match = WEEK_RE.match(line)
        if week_match and current is not None:
            start, end, title = week_match.groups()
            current["weeks"].append(
                {
                    "start": int(start),
                    "end": int(end) if end else int(start),
                    "title": title.strip(),
                }
            )
    if not phases:
        raise ValueError("无法从计划文档解析阶段结构")
    return plan_text, phases


def week_day_topics(plan_text, week):
    """收集某一周各天的主题（每个任务行 ｜ 与首个 ； 之间的片段）。"""
    topics = []
    capturing = False
    for line in plan_text.splitlines():
        if WEEK_RE.match(line):
            match = WEEK_RE.match(line)
            start = int(match.group(1))
            end = int(match.group(2) or match.group(1))
            capturing = week >= start and week <= end
            continue
        if capturing:
            item = DAY_ITEM_RE.match(line)
            if item:
                topic = item.group(1).split("；")[0].split("（")[0]
                topic = topic.replace("**", "").replace("`", "").strip()
                if topic:
                    topics.append(topic)
            elif line.startswith("#"):
                capturing = False
    return topics


def day_pills(tutorials, root_prefix="", current_day=None):
    return [
        {
            "num": day["number"],
            "label": f"Day {day['number']}",
            "title": day["title"],
            "href": f"{root_prefix}tutorials/{day['slug']}/",
            "active": current_day == day["number"],
        }
        for day in tutorials
    ]


def page_template(
    title,
    markdown,
    *,
    root_prefix="",
    pills=None,
    back_link=None,
    prev_link=None,
    next_link=None,
    home=False,
):
    escaped_title = html.escape(title)
    escaped_markdown = escape_for_template_string(markdown)

    pills_html = ""
    body_class = "vp-page vp-home" if home else "vp-page"
    if pills:
        def pill_link(item, css_class="vp-pill"):
            active = " active" if item.get("active") else ""
            current = ' aria-current="page"' if item.get("active") else ""
            return (
                f'<a class="{css_class}{active}" href="{item["href"]}"'
                f' title="{html.escape(item["title"], quote=True)}"{current}>'
                f'{html.escape(str(item["label"]))}</a>'
            )

        if len(pills) > 10:
            active_day = next((item for item in pills if item.get("active")), None)
            toggle_label = active_day["label"] if active_day else f"{len(pills)} 天教程"
            weeks = {}
            for item in pills:
                weeks.setdefault((item["num"] - 1) // 7 + 1, []).append(item)
            groups = []
            for week, items in weeks.items():
                links = "".join(pill_link(item, "vp-day-link") for item in items)
                groups.append(
                    f'<section class="vp-day-week"><div class="vp-day-week-title">'
                    f'<strong>第 {week} 周</strong><span>{items[0]["label"]}–{items[-1]["label"]}</span>'
                    f'</div><div class="vp-day-week-links">{links}</div></section>'
                )
            pills_html = (
                '<nav class="vp-nav-pills vp-nav-pills-grouped" aria-label="每日教程导航">'
                f'<details class="vp-day-nav"><summary class="vp-day-nav-toggle">'
                f'<span>{toggle_label}</span><span class="vp-day-nav-chevron" aria-hidden="true">⌄</span>'
                f'</summary><div class="vp-day-menu"><div class="vp-day-menu-header">'
                f'<strong>按周学习</strong><span>已发布 {len(pills)} 天</span></div>'
                f'<div class="vp-day-weeks">{"".join(groups)}</div></div></details></nav>'
            )
            body_class += " vp-page-grouped-nav"
        else:
            pills_html = '<nav class="vp-nav-pills">' + "".join(pill_link(item) for item in pills) + "</nav>"

    back_html = ""
    if back_link:
        href, label = back_link
        back_html = f'<nav class="back-nav"><a href="{href}">← {html.escape(label)}</a></nav>'

    def pager(link, css_class, label):
        if not link:
            return "<span></span>"
        href, text = link
        return (
            f'<a class="pager-link {css_class}" href="{href}"><span class="desc">{label}</span>'
            f'<span class="title">{html.escape(text)}</span></a>'
        )

    pager_html = ""
    if prev_link or next_link:
        pager_html = (
            '<div class="prev-next">'
            + pager(prev_link, "prev", "上一篇")
            + pager(next_link, "next", "下一篇")
            + "</div>"
        )

    if home:
        main_html = f'<main class="home-main">{markdown}</main>'
        render_call = "VPPage.init();"
        escaped_markdown = ""  # home 页 HTML 已直接嵌入——不需要 JS 变量
    else:
        main_html = f"""<div class="vp-main"><div class="vp-doc-wrap"><div class="vp-container">
        <div class="vp-content"><div class="vp-content-container">
            {back_html}
            <main class="main"><article class="vp-doc">
                <h1>{escaped_title}</h1>
                <div id="doc-content"></div>
            </article></main>
            <footer class="vp-doc-footer">{pager_html}</footer>
        </div></div>
        <div class="vp-aside"><div class="vp-aside-container"><div class="vp-aside-content">
            <nav class="vp-outline" aria-labelledby="outline-title">
                <div class="outline-title" id="outline-title">本页目录</div>
                <div class="outline-content"><div class="outline-marker"></div>
                    <ul class="outline-root" id="outline-root"></ul>
                </div>
            </nav>
        </div></div></div>
    </div></div></div>"""
        render_call = "VPPage.render(pageMarkdown);"

    return f"""<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="零基础 24 周、144 天 C++ 实战学习计划：基础语法、指针与内存、面向对象、STL 与泛型、现代 C++ 与综合项目实战。">
    <meta property="og:title" content="{escaped_title} · {SITE_NAME}">
    <meta property="og:description" content="从第一行 Hello World 到亲手交付 JSON 解析器的 144 天 C++ 实战教程，覆盖基础语法、内存、面向对象、STL 与现代 C++。">
    <meta property="og:type" content="article">
    <meta property="og:site_name" content="{SITE_NAME}">
    <meta name="twitter:card" content="summary">
    <meta name="twitter:title" content="{escaped_title}">
    <meta name="twitter:description" content="零基础 24 周、144 天 C++ 实战学习计划。">
    <link rel="icon" href="{root_prefix}favicon.svg" type="image/svg+xml">
    <title>{escaped_title} · {SITE_NAME}</title>
    <script>
    (function() {{
        try {{
            var t = localStorage.getItem('vp-theme') || 'dark';
            if (t === 'dark') document.documentElement.classList.add('dark');
        }} catch (e) {{ document.documentElement.classList.add('dark'); }}
    }})();
    </script>
    <link rel="stylesheet" href="{root_prefix}css/vp-solution.css?v=7">
    <script src="{root_prefix}js/marked.min.js"></script>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.css">
    <script src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.js"></script>
    <script src="{root_prefix}js/markdown-math.js"></script>
    <script src="{root_prefix}js/prism.min.js"></script>
    <script src="{root_prefix}js/prism-c.min.js"></script>
    <script src="{root_prefix}js/prism-cpp.min.js"></script>
    <script src="{root_prefix}js/prism-bash.min.js"></script>
    <script src="{root_prefix}js/prism-python.min.js"></script>
</head>
<body class="{body_class}">
    <header class="vp-navbar">
        <div class="vp-navbar-inner">
            <a class="vp-brand" href="{root_prefix}index.html">C++ <span>Roadmap</span></a>
            {pills_html}
            <div class="vp-navbar-spacer"></div>
            <nav class="vp-menu" aria-label="站点导航">
                <a href="{root_prefix}plan.html">144 天计划</a>
                <a class="vp-roadmap-link" href="{root_prefix}index.html#roadmap">学习路线</a>
                <a href="{root_prefix}index.html#tutorials">每日教程</a>
                <a class="vp-github-link" href="{GITHUB_REPO_URL}" target="_blank" rel="noopener noreferrer">GitHub ↗</a>
            </nav>
            <div class="vp-appearance">
                <button class="vp-switch" type="button" aria-label="切换深色/浅色外观">
                    <span class="check"><span class="icon sun">☀</span><span class="icon moon">☾</span></span>
                </button>
            </div>
        </div>
    </header>
    {main_html}
    <footer class="vp-footer"><span>{SITE_NAME} · Deployed on GitHub Pages</span></footer>
    <button class="vp-back-to-top" aria-label="回到顶部">↑</button>
    <script src="{root_prefix}js/vp-solution.js?v=6"></script>
    <script>
        {'const pageMarkdown = `' + escaped_markdown + '`;' if escaped_markdown else 'const pageMarkdown = "";'}
        try {{
            if (typeof marked === 'undefined' || !window.VPPage) {{
                throw new Error('页面脚本加载失败。');
            }}
            {render_call}
        }} catch (err) {{
            const content = document.getElementById('doc-content');
            if (content) content.textContent = '页面渲染失败：' + err.message;
            console.error('Markdown render error:', err);
        }}
    </script>
</body>
</html>
"""


def plan_heading_slug(text):
    slug = re.sub(r"[^\w\s-]", "", text.lower(), flags=re.UNICODE)
    return re.sub(r"-+", "-", re.sub(r"\s+", "-", slug.strip())).strip("-")


def build_home(tutorials, plan_text, phases):
    week_anchor = {}
    for phase in phases:
        for week in phase["weeks"]:
            heading = f"第 {week['start']} 周 · {week['title']}" if week["start"] == week["end"] else f"第 {week['start']}~{week['end']} 周 · {week['title']}"
            week_anchor[week["start"]] = plan_heading_slug(heading)

    roadmap_html = []
    for phase in phases:
        week_cards = []
        for week in phase["weeks"]:
            for number in range(week["start"], week["end"] + 1):
                topics = week_day_topics(plan_text, number)
                summary = " · ".join(topics) if topics else week["title"]
                if len(summary) > 84:
                    summary = summary[:81] + "..."
                week_cards.append(
                    f'<a class="week-card" href="plan.html#{week_anchor[week["start"]]}">'
                    f'<div class="week-card-top"><span class="week-number">W{number}</span>'
                    '<span class="week-arrow" aria-hidden="true">→</span></div>'
                    f'<h4>{html.escape(week["title"])}</h4><p>{html.escape(summary)}</p></a>'
                )
        description = f"里程碑：{phase['milestone']}" if phase["milestone"] else "每周一个可运行的小项目"
        roadmap_html.append(
            '<section class="phase-block">'
            f'<div class="phase-heading"><span class="phase-badge">{phase["label"]}</span>'
            f'<h3>{html.escape(phase["title"])}</h3><p>{html.escape(description)}</p></div>'
            f'<div class="week-cards">{"".join(week_cards)}</div></section>'
        )

    # 按阶段分组教程卡片（阶段天数区间来自计划进度总览）
    stage_ranges = [
        (int(day_start), int(day_end))
        for _, _, day_start, day_end, _ in MILESTONE_RE.findall(plan_text)
    ]
    latest_day = tutorials[-1]["number"]
    phase_groups = []
    for pi, phase in enumerate(phases):
        if pi < len(stage_ranges):
            first_day, last_day = stage_ranges[pi]
        else:
            first_day, last_day = pi * 24 + 1, (pi + 1) * 24
        days = [d for d in tutorials if first_day <= d["number"] <= last_day]
        if not days:
            continue
        cards = []
        for day in days:
            title = re.sub(r"^第\s*\d+\s*天[：:]\s*", "", day["title"])
            goal = re.search(r"^> \*\*今日目标\*\*：(.+)$", day["markdown"], re.MULTILINE)
            summary = re.sub(r"[`*_]", "", goal.group(1)) if goal else "概念讲解、技术图解、动手实现与结果验证。"
            if len(summary) > 120:
                summary = summary[:117] + "..."
            cards.append(
                f'<a class="tutorial-card" href="tutorials/{day["slug"]}/">'
                f'<div class="week-card-top"><span class="week-number">Day {day["number"]}</span>'
                '<span class="week-arrow" aria-hidden="true">\u2192</span></div>'
                f'<h3>{html.escape(title)}</h3><p>{html.escape(summary)}</p>'
                '<span class="tutorial-card-link">开始阅读 <span aria-hidden="true">\u2197</span></span></a>'
            )
        contains_latest = first_day <= latest_day <= last_day
        is_first = pi == 0
        phase_groups.append(
            f'<details class="phase-tutorials"{" open" if contains_latest or is_first else ""}>'
            f'<summary><span class="phase-badge">{phase["label"]}</span>'
            f'<span class="phase-tut-title">{html.escape(phase["title"])}</span>'
            f'<span class="phase-tut-count">{len(days)} 天</span>'
            '<span class="vp-day-nav-chevron" aria-hidden="true">\u29c9</span></summary>'
            f'<div class="tutorial-cards">{"".join(cards)}</div></details>'
        )

    first_day = tutorials[0]
    content = f"""
<section class="home-hero">
    <div class="home-hero-inner">
        <div class="hero-badge">零基础起步 · 24 周渐进式路线</div>
        <h1>C++ 从基础到深入 <span>144 天学习计划</span></h1>
        <p class="hero-subtitle">从「写下第一行 Hello World」到「亲手交付 JSON 解析器」</p>
        <p class="hero-description">贯通 C++ 基础语法、指针与内存、面向对象、STL 与泛型、现代 C++ 与综合项目实战<br>工作日每天 1.5~2 小时，周六项目日，每周一个里程碑</p>
        <div class="hero-actions">
            <a class="hero-button hero-button-primary" href="tutorials/{first_day["slug"]}/">开始 Day {first_day["number"]} <span aria-hidden="true">→</span></a>
            <a class="hero-button hero-button-secondary" href="plan.html">查看完整计划</a>
        </div>
        <div class="hero-stats" aria-label="学习计划概览">
            <div><strong>24</strong><span>周学习路线</span></div>
            <div><strong>144</strong><span>每日实战任务</span></div>
            <div><strong>6</strong><span>阶段能力进阶</span></div>
            <div><strong>6</strong><span>个里程碑项目</span></div>
        </div>
    </div>
</section>
<section class="home-section roadmap-section" id="roadmap">
    <div class="section-heading"><h2>学习路线</h2><p>六个阶段、24 周，从第一行代码一路走到能交付完整项目的 C++ 开发者。</p></div>
    {"".join(roadmap_html)}
</section>
<section class="home-section tutorials-section" id="tutorials">
    <div class="section-heading section-heading-row"><div><h2>每日教程</h2><p>已发布 {len(tutorials)} 篇图文教程，每篇都包含讲解、实践、练习与验收。</p></div><span class="section-status">持续更新</span></div>
    <div class="tutorial-phases">{"".join(phase_groups)}</div>
</section>
"""
    output = page_template(SITE_TITLE, content, home=True)
    (PUBLIC_DIR / "index.html").write_text(output, encoding="utf-8")


def build_plan(tutorials):
    text = PLAN_PATH.read_text(encoding="utf-8")
    text = re.sub(r"^\s*#\s+.+\n", "", text, count=1)
    output = page_template(
        "C++ 144 天完整计划",
        text,
        pills=day_pills(tutorials),
        back_link=("index.html", "返回首页"),
    )
    (PUBLIC_DIR / "plan.html").write_text(output, encoding="utf-8")


def build_tutorials(tutorials):
    for index, day in enumerate(tutorials):
        output_dir = PUBLIC_DIR / "tutorials" / day["slug"]
        output_dir.mkdir(parents=True, exist_ok=True)
        images = day["source"] / "images"
        if images.exists():
            shutil.copytree(images, output_dir / "images", dirs_exist_ok=True)

        prev_link = None
        next_link = None
        if index > 0:
            previous = tutorials[index - 1]
            prev_link = (f"../{previous['slug']}/", previous["title"])
        if index + 1 < len(tutorials):
            following = tutorials[index + 1]
            next_link = (f"../{following['slug']}/", following["title"])

        output = page_template(
            day["title"],
            day["markdown"],
            root_prefix="../../",
            pills=day_pills(tutorials, root_prefix="../../", current_day=day["number"]),
            back_link=("../../index.html#tutorials", "返回每日教程"),
            prev_link=prev_link,
            next_link=next_link,
        )
        (output_dir / "index.html").write_text(output, encoding="utf-8")
        print(f"Generated tutorials/{day['slug']}/index.html")


def build_sitemap(tutorials):
    urls = [
        f"<url><loc>{SITE_URL}/</loc><changefreq>weekly</changefreq><priority>1.0</priority></url>",
        f"<url><loc>{SITE_URL}/plan.html</loc><changefreq>monthly</changefreq><priority>0.9</priority></url>",
    ]
    for day in tutorials:
        loc = f"{SITE_URL}/tutorials/{day['slug']}/"
        urls.append(f"<url><loc>{loc}</loc><changefreq>monthly</changefreq><priority>0.8</priority></url>")
    xml = '<?xml version="1.0" encoding="UTF-8"?>\n'
    xml += '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">\n'
    xml += "\n".join(urls) + "\n</urlset>\n"
    (PUBLIC_DIR / "sitemap.xml").write_text(xml, encoding="utf-8")


def build_robots():
    content = f"User-agent: *\nAllow: /\nSitemap: {SITE_URL}/sitemap.xml\n"
    (PUBLIC_DIR / "robots.txt").write_text(content, encoding="utf-8")


def main():
    tutorials = load_tutorials()
    plan_text, phases = load_plan_phases()
    if PUBLIC_DIR.exists():
        shutil.rmtree(PUBLIC_DIR)
    PUBLIC_DIR.mkdir()
    shutil.copytree(STATIC_DIR / "css", PUBLIC_DIR / "css")
    shutil.copytree(STATIC_DIR / "js", PUBLIC_DIR / "js")
    if (STATIC_DIR / "favicon.svg").exists():
        shutil.copy2(STATIC_DIR / "favicon.svg", PUBLIC_DIR / "favicon.svg")
    (PUBLIC_DIR / ".nojekyll").write_text("", encoding="utf-8")

    build_home(tutorials, plan_text, phases)
    build_plan(tutorials)
    build_tutorials(tutorials)
    build_sitemap(tutorials)
    build_robots()
    print(f"Website built successfully: {len(tutorials)} tutorials in {PUBLIC_DIR}")


if __name__ == "__main__":
    main()
