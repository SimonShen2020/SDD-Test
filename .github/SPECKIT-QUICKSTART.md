# Speckit Quick Start

本文档面向第一次使用 Speckit 的成员，目标不是解释所有设计细节，而是帮助你在 10 分钟内搞清楚三件事：

- 什么时候该进入 Speckit。
- 进入后每一步大概在做什么。
- 什么时候应该停下来修正，而不是继续往下走。

如果你想理解更深层的设计哲学，请继续阅读 `SPECKIT-DESIGN.md`。
如果你想查文件分层和维护边界，请阅读 `SPECKIT-WORKFLOW.md`。
如果你准备实际维护这些文档，请继续阅读 `SPECKIT-MAINTENANCE-CHECKLIST.md`。

## 1. 先记住一句话

Speckit 不是“多写几份文档”，而是把一个需求压缩成一条可交付、可追溯、可验收的工程链路。

## 2. 什么时候该进入 Speckit

只有在用户明确触发 `sdd`、`SDD` 或 `Speckit流水线` 时，才进入 Speckit 主流程。

如果没有显式触发：

- 纯解释、总结、问答，不进入 Speckit。
- 普通开发任务，默认走通用模式，而不是自动切到 Speckit。
- 对于高歧义、多角色权衡的复杂任务，可以在通用模式下可选进入 `product-workshop` 做前置强化，但它不是默认必经步骤。

这条规则来自 `master.agent.md`，是整个系统的第一道边界。

## 3. Speckit 最短流程图

```text
Specify
  -> Clarify
  -> Research
  -> Plan
  -> Tasks
  -> Analyze
  -> Implement
  -> Review
```

你可以把它理解成 8 个连续问题：

1. 需求到底是什么？
2. 哪些歧义必须先讲清楚？
3. 当前仓库里已有啥可复用？
4. 准备怎么设计？
5. 具体要做哪些任务？
6. 现在能不能安全开始实现？
7. 已经做出了什么？证据是什么？
8. 现在真的可以交付吗？

## 4. 每一步最该关注什么

### Specify

- 输出：`spec.md`
- 关注点：需求、边界、FR/NFR、边缘情况
- 常见错误：一上来就把实现方案写进 spec

### Clarify

- 输出：修订后的 `spec.md`
- 关注点：会影响实现和验收的歧义
- 常见错误：把技术设计问题误放到澄清阶段

### Research

- 输出：`research.md`
- 关注点：上下文、复用点、风险、不可行项
- 常见错误：把调研写成最终设计决策

### Plan

- 输出：`plan.md`
- 关注点：架构、阶段划分、风险缓解、决策分级
- 常见错误：没有证据回指，只给个人偏好方案

### Tasks

- 输出：`tasks.md`
- 关注点：任务粒度、依赖、需求映射、验证动作
- 常见错误：任务太大、太空泛、不能回指需求

### Analyze

- 输出：一致性结论
- 关注点：`spec` / `plan` / `tasks` 是否一致
- 常见错误：把 Analyze 当作普通建议，而不是实现前门禁

### Implement

- 输出：实现结果、任务状态、实现摘要
- 关注点：按任务推进、保留验证证据、记录残余风险
- 常见错误：只改代码，不维护任务状态和证据

### Review

- 输出：`PASS` / `PASS WITH RISKS` / `FAIL`
- 关注点：需求是否满足、证据是否够、能否交付
- 常见错误：把 Review 当成“看看代码有没有问题”

## 5. 这几个规则一定要记住

### 规则 1：没有显式触发词，不进 Speckit

Speckit 是显式触发工作流，不是默认工作流。

### 规则 2：`Analyze` 没通过，不能进入 `Implement`

这是实现前硬门禁。

### 规则 3：`Review` 没通过，不能宣称可交付

这是交付前硬门禁。

### 规则 4：agent 管阶段，skill 管能力

不要拿 skill 代替 stage agent，也不要把阶段规则偷偷写进 skill。

### 规则 5：所有判断都尽量回到工件和证据

Speckit 不鼓励“凭感觉说已经差不多了”。

## 6. 新人最常见的误区

- 误区 1：把 Speckit 当模板生成器。
  - 正解：它是阶段化收敛链路，不只是模板集合。

- 误区 2：看到复杂任务就自动想走 Speckit。
  - 正解：只有显式触发才进入 Speckit。

- 误区 3：觉得 `Analyze` 只是建议，不是门禁。
  - 正解：Analyze 是实现前门禁，不通过就要回退修正。

- 误区 4：觉得 `Review` 就是代码 review。
  - 正解：Review 关注的是能否交付，而不只是代码质量。

- 误区 5：觉得写完 `tasks.md` 就可以直接宣称流程完成。
  - 正解：后面还有 Analyze、Implement、Review 三段闭环。

## 7. 一个最小通过样例应该长什么样

如果要判断一个 feature 有没有基本走通 Speckit，至少应看到：

- `spec.md`
- `research.md`
- `plan.md`
- `tasks.md`
- Analyze 结论
- 实现摘要
- Review 结论

如果缺其中任何一个，通常说明链路还没有闭合。

## 8. 如果你准备开始维护文档

先不要急着改。

建议先做这三步：

1. 看 `SPECKIT-WORKFLOW.md`，确认你要改的是哪一层。
2. 看 `SPECKIT-DESIGN.md`，确认这次改动会不会破坏现有边界和门禁。
3. 再回到具体 `agent` / `prompt` / `skill` 文件做修改。

## 9. 阅读顺序建议

如果你现在是：

- 新人使用者：先读本文，再读 `SPECKIT-WORKFLOW.md`
- 维护者：先读 `SPECKIT-WORKFLOW.md`，再读 `SPECKIT-DESIGN.md`
- 设计修改者：直接读 `SPECKIT-DESIGN.md`

## 10. 一句话总结

Speckit 的价值，不在于多了几个阶段名字，而在于它把“需求、设计、实现、验收”这四件原本容易脱节的事情，压成了一条可复核的链路。

## 11. 下一步阅读

- 想理解分层和边界：读 `SPECKIT-WORKFLOW.md`
- 想理解设计哲学和实现原理：读 `SPECKIT-DESIGN.md`
- 想开始维护或扩展：读 `SPECKIT-MAINTENANCE-CHECKLIST.md`