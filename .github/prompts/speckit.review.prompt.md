---
agent: speckit.review
---

用于 Speckit 的第 8 阶段，对当前 feature 执行最终验收复核。

- 适用时机：实现阶段已结束，需要判断该功能是否可以交付。
- 前置条件：当前 feature 通常已存在 `spec.md`、`plan.md`、`tasks.md`，并且实现与验证已基本完成；若缺少关键证据，会被视为风险或失败因素。
- 执行结果：输出 `PASS`、`PASS WITH RISKS` 或 `FAIL` 的最终交付结论及后续建议，并说明结论依据。
- 边界：该阶段默认只读，不直接替代实现、Analyze 一致性门禁或需求质量检查。
- 典型输入：`对当前 feature 做最终验收复核，告诉我能否交付`。