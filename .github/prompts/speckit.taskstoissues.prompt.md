---
agent: speckit.taskstoissues
---

用于将 `tasks.md` 中的任务同步为 GitHub Issue。

- 适用时机：任务已稳定，需要外部协作或看板追踪。
- 前置条件：当前 feature 已存在 `tasks.md`，且仓库远程是有效的 GitHub URL。
- 执行结果：先展示 dry-run 摘要，用户确认后再创建 Issue。
- 回退行为：若远程不是 GitHub，输出 Markdown backlog 摘要而不写入远程仓库。
- 典型输入：`把当前 tasks 转成 GitHub issues，先给我看 dry-run`。
