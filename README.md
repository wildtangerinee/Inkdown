# Inkdown

**A lightweight, native, open-source Markdown editor.**

Inkdown 是一个计划使用 **C++17 + Qt 6 Widgets** 开发的免费、开源桌面 Markdown 编辑器。目标是在本地 `.md` 文件上提供接近 Typora 的**所见即所得**体验：输入完整 Markdown 格式后即时转换，并可以直接编辑渲染后的内容。

> **Status:** Planning / M0 technical validation. 当前仓库首先提交设计文档，应用代码尚未实现。下方功能是**计划**，不是已完成清单。

## Planned Features

- 单编辑区 Markdown 所见即所得编辑。
- `# `、`**text**`、`- ` 等语法即时转换。
- 标题、列表、引用、代码块、链接、图片与基础表格。
- 本地 Markdown 文件读写，不强制登录或联网。
- 查找替换、标题大纲、字数统计、多标签页。
- 浅色和深色主题。
- macOS 首发，后续支持 Windows。

## Tech Stack

| Item | Choice |
| --- | --- |
| Language | C++17 |
| UI | Qt 6 Widgets |
| Editor prototype | QTextEdit / QTextDocument |
| Build | CMake |
| Tests | Qt Test + CTest |
| Platforms | macOS first; Windows planned |

## Why Inkdown?

Inkdown 的工程重点不仅是“显示 Markdown”，而是解决 **Markdown 源码、文档语义、可编辑富文本视图、光标/选区和撤销历史之间的一致性**。这是一个开发者亲自编码、AI 辅助分析与 Review 的长期 C++ 项目。

## Roadmap

- [ ] M0：Qt 编辑器原型及即时转换可行性验证。
- [ ] M1：基础 Markdown 编辑和本地文件读写。
- [ ] M2：完整编辑核心、序列化与稳定的撤销重做。
- [ ] M3：扩展语法和桌面 UI。
- [ ] M4：macOS 稳定版。
- [ ] M5：Windows 版本。

详见 [`docs/TASKS.md`](docs/TASKS.md)。

## Documentation

- [`docs/PRD.md`](docs/PRD.md) — 产品需求与 UI 交互规范。
- [`docs/TECH_SPEC.md`](docs/TECH_SPEC.md) — 技术架构和 M0 可行性验证。
- [`docs/TASKS.md`](docs/TASKS.md) — 开发阶段与任务验收标准。
- [`AGENTS.md`](AGENTS.md) — AI 辅助开发规范。

## Build

当前为**文档初始化阶段**，尚未提交 `CMakeLists.txt` 和可运行代码。完成 `T001` 后将在此补充实际验证过的构建方式。

计划使用：CMake、支持 C++17 的编译器以及 Qt 6 Widgets。不要把本阶段文档视为已经可以编译的应用。

## License

**待定。** 首次公开发布源代码或二进制前，将选定许可证并添加 `LICENSE` 文件，同时核对 Qt 及第三方依赖的许可条件。在正式添加许可证前，不应将此仓库视为已经取得开源使用授权。

## Acknowledgements

编辑体验参考 Typora，并吸收 Notion 的简洁交互理念。Inkdown 与这两个项目及其开发者**无关联**。
