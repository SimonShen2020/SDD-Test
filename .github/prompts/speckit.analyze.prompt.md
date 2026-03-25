---
agent: speckit.analyze
---

用于 Speckit 的第 6 阶段，对 `spec.md`、`plan.md`、`tasks.md` 做只读一致性分析。

- 适用时机：`tasks.md` 已生成，准备进入实现前。
- 前置条件：当前 feature 已存在 `spec.md`、`plan.md`、`tasks.md`。
- 执行结果：输出一致性门禁结论与结构化分析报告，明确是否允许进入 `speckit.implement`，不会自动修改文件。
- 边界：该阶段只做只读一致性门禁分析，不会替你直接修正文档，也不替代最终 `speckit.review`。
- 典型输入：`检查 spec、plan、tasks 三者是否一致并可进入实现`。
