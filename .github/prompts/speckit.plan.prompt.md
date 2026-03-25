---
agent: speckit.plan
---

用于 Speckit 的第 4 阶段，把需求和调研结果转成实施方案。

- 适用时机：`spec.md` 已稳定，且 `research.md` 已产出。
- 前置条件：至少存在 `FEATURE_DIR/spec.md` 和 `FEATURE_DIR/research.md`。
- 执行结果：生成 `FEATURE_DIR/plan.md`，给出架构、阶段划分、风险与缓解策略，并标明哪些设计是强制项、按需项或可延后项。
- 边界：该阶段输出设计蓝图，不把计划细化成逐条开发任务；关键技术选择应能回指 `research.md` 中的证据。
- 典型输入：`为当前功能生成技术方案`。
