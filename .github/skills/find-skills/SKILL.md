---
name: find-skills
description: 帮助用户发现和安装 agent 技能。当用户问"如何做 X"、"有没有做 X 的技能"、"是否有技能可以……"，或者表达希望扩展能力时，使用此技能。
---

# Find Skills（技能发现）

这个技能帮助你从开放 agent 技能生态中发现和安装技能。

## 使用时机

在以下情况使用此技能：

- 用户问"如何做 X"，而 X 可能已有现成技能
- 用户说"帮我找一个做 X 的技能"或"有没有 X 的技能"
- 用户问"你能做 X 吗"，而 X 是某种专项能力
- 用户希望扩展 agent 的能力
- 用户想搜索工具、模板或工作流
- 用户提到某个专业领域（设计、测试、部署等）有帮助需要

## 什么是 Skills CLI？

Skills CLI（`npx skills`）是开放 agent 技能生态的包管理器。技能是可模块化组合的包，能为 agent 提供专项知识、工作流和工具能力。

**常用命令：**

- `npx skills find [query]` — 交互式搜索或按关键词查找技能
- `npx skills add <package>` — 从 GitHub 或其他来源安装技能
- `npx skills check` — 检查技能更新
- `npx skills update` — 更新所有已安装技能

**在线浏览技能：** https://skills.sh/

## 如何帮助用户找技能

### 第一步：理解用户需求

当用户提出帮助请求时，识别：

1. 领域（如 React、测试、设计、部署）
2. 具体任务（如编写测试、创建动画、审查 PR）
3. 这是否是一个足够常见、可能已有技能的需求

### 第二步：先查排行榜

在运行 CLI 搜索前，先查看 [skills.sh 排行榜](https://skills.sh/)，看该领域是否已有知名技能。排行榜按累计安装量排序，展示最流行和最成熟的选项。

例如，网页开发领域的热门技能：
- `vercel-labs/agent-skills` — React、Next.js、网页设计（各 10 万+ 安装）
- `anthropics/skills` — 前端设计、文档处理（10 万+ 安装）

### 第三步：搜索技能

如果排行榜未覆盖用户需求，运行查找命令：

```bash
npx skills find [query]
```

示例：

- 用户问"如何让 React 应用更快？" → `npx skills find react performance`
- 用户问"能帮我审查 PR 吗？" → `npx skills find pr review`
- 用户问"我需要生成变更日志" → `npx skills find changelog`

### 第四步：推荐前先验证质量

**不要仅凭搜索结果推荐技能。** 始终验证以下内容：

1. **安装量** — 优先选择 1000+ 安装量的技能。低于 100 安装量需谨慎。
2. **来源可信度** — 官方来源（`vercel-labs`、`anthropics`、`microsoft`）比未知作者更可靠。
3. **GitHub 星标** — 检查源仓库，来自不足 100 颗星的仓库应持怀疑态度。

### 第五步：向用户展示选项

找到相关技能后，向用户展示：

1. 技能名称及功能
2. 安装量和来源
3. 可运行的安装命令
4. 了解更多的链接

示例回复：

```
我找到一个可能有帮助的技能！"react-best-practices" 技能提供来自 Vercel 工程团队的
React 和 Next.js 性能优化指南。（185K 安装）

安装方式：
npx skills add vercel-labs/agent-skills@react-best-practices

了解更多：https://skills.sh/vercel-labs/agent-skills/react-best-practices
```

### 第六步：提供安装帮助

如果用户想继续，可以帮用户安装：

```bash
npx skills add <owner/repo@skill> -g -y
```

`-g` 标志表示全局（用户级）安装，`-y` 跳过确认提示。

## 常见技能分类

搜索时，可参考以下常见分类：

| 分类     | 示例关键词                                 |
| -------- | ------------------------------------------ |
| 网页开发 | react, nextjs, typescript, css, tailwind   |
| 测试     | testing, jest, playwright, e2e             |
| DevOps   | deploy, docker, kubernetes, ci-cd          |
| 文档     | docs, readme, changelog, api-docs          |
| 代码质量 | review, lint, refactor, best-practices     |
| 设计     | ui, ux, design-system, accessibility       |
| 生产力   | workflow, automation, git                  |

## 搜索技巧

1. **使用具体关键词**：`react testing` 比单独的 `testing` 效果更好
2. **尝试同义词**：`deploy` 找不到时，试试 `deployment` 或 `ci-cd`
3. **关注热门来源**：很多技能来自 `vercel-labs/agent-skills` 或 `ComposioHQ/awesome-claude-skills`

## 未找到技能时的处理

如果没有找到相关技能：

1. 告知用户未找到现有技能
2. 提供用通用能力直接帮用户完成任务的方案
3. 建议用户用 `npx skills init` 创建自己的技能

示例：

```
我搜索了与 "xyz" 相关的技能，但没有找到匹配项。
我仍然可以直接帮助你完成这个任务！你想继续吗？

如果这是你经常需要做的事，也可以创建自己的技能：
npx skills init my-xyz-skill
```
