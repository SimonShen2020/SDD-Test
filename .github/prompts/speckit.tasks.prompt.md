---
agent: speckit.tasks
---

用于 Speckit 的第 5 阶段，把实施方案拆成可执行任务。

- 适用时机：`plan.md` 已获确认，需要形成开发清单。
- 前置条件：当前 feature 已存在 `spec.md` 和 `plan.md`。
- 执行结果：生成 `FEATURE_DIR/tasks.md`，任务应可执行、可验证、可追溯，并尽可能明确依赖和需求映射。
- 边界：该阶段产出任务，不直接实现代码；核心实现任务和验证任务应回指 FR/NFR/EC 或对应 plan 要素。
- 典型输入：`将当前 plan 拆成可执行开发任务`。
