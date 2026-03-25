---
agent: speckit.implement
---

用于 Speckit 的第 7 阶段，根据任务清单执行实现。

- 适用时机：`Analyze` 已通过，或只剩用户明确接受的非阻塞风险。
- 前置条件：当前 feature 已存在 `tasks.md`；通常还应有 `plan.md` 和 `spec.md`。
- 执行结果：按阶段推进实现、更新任务完成状态，并执行必要验证，输出可供 `speckit.review` 使用的实现证据摘要。
- 边界：若 checklist 未完成，可能会在继续前要求用户确认风险；该阶段不替代最终 `speckit.review` 验收。
- 典型输入：`按当前 tasks 开始实现并同步更新完成状态`。
