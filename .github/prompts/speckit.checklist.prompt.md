---
agent: speckit.checklist
---

用于 Speckit 的辅助检查阶段，生成“针对需求质量”的检查表。

- 适用时机：需要评估需求是否完整、清晰、可测，而不是测试实现是否工作。
- 前置条件：至少存在 `FEATURE_DIR/spec.md`。
- 执行结果：在 `FEATURE_DIR/checklists/` 下创建新的需求质量检查表文件。
- 边界：这不是最终验收，也不替代 `speckit.analyze` 的一致性门禁结论。
- 典型输入：`为当前功能生成一份 API 需求质量检查表`。
