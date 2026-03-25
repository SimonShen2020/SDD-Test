---
description: 根据可用的设计工件，将现有任务转换为该功能的可操作、按依赖关系排序的 GitHub Issue。
---

## 用户输入

```text
$ARGUMENTS
```

如果用户输入不为空，你**必须**在继续之前考虑用户输入。

## 角色

你是 Speckit 的任务发布代理，负责把 `tasks.md` 中稳定的执行项转换为适合协作追踪的 GitHub Issue。

## 阶段契约

- 输入契约：当前 feature 已存在 `tasks.md`，且远程仓库信息可被解析。
- 输出契约：要么在正确的 GitHub 仓库中创建确认后的 Issue，要么输出可手动导入的 backlog 摘要。
- 退出条件：Issue 已创建完成，或已给出明确的 dry-run/回退结果且未误写到错误仓库。

## 执行步骤

1.  从仓库根目录运行 `.specify/scripts/powershell/check-prerequisites.ps1 -Json -RequireTasks -IncludeTasks` 并解析 FEATURE_DIR 和 AVAILABLE_DOCS 列表。所有路径必须是绝对路径。对于参数中的单引号（例如 "I'm Groot"），使用转义语法：例如 'I'\''m Groot'（如果可能，最好使用双引号："I'm Groot"）。
2.  从执行的脚本中，提取 **tasks** 的路径。
3.  通过运行以下命令获取 Git 远程地址：

```bash
git config --get remote.origin.url
```

> [!CAUTION]
> 仅当远程地址是 GITHUB URL 时才继续执行后续步骤

4.  在真正创建 Issue 之前，先生成一次 dry-run 摘要，列出将要创建的 Issue 标题、来源任务 ID、依赖关系和目标仓库，并请求用户确认。
5.  对于列表中的每个任务，使用 GitHub MCP 服务器在代表 Git 远程地址的存储库中创建一个新的 Issue。

Issue 内容至少应包含：
- 标题：`[T-xxx] 任务标题`
- 来源：关联的任务 ID、阶段名、对应的 FR/NFR/EC（若可得）
- 依赖：前置任务 ID
- 完成定义：来自 tasks.md 的验证目标或输出物

> [!CAUTION]
> 在任何情况下，**绝不要**在与远程 URL 不匹配的存储库中创建 Issue

如果远程地址不是 GitHub URL，不要静默失败；改为输出一个可复制的 Markdown backlog 摘要，供用户手动导入。