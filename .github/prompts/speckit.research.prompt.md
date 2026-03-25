---
agent: speckit.research
---

用于 Speckit 的第 3 阶段，在设计前完成代码库调研。

- 适用时机：`spec.md` 已澄清完成，需要识别可复用资产、集成点和技术风险。
- 前置条件：当前 feature 已存在 `FEATURE_DIR/spec.md`。
- 执行结果：生成 `FEATURE_DIR/research.md`，为后续 `plan` 提供证据基础，并明确风险、不可行项与是否可进入 Plan。
- 边界：该阶段重点是发现现状和约束，不直接锁定最终架构方案。
- 典型输入：`调研当前代码库里与该功能最相关的模块和复用点`。