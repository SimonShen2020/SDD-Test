# Speckit Workflow Overview

本文档用于说明 `.github` 目录下 Speckit 工作流的维护边界，帮助后续快速理解各类文件的职责。

## 阅读建议

- 如果你是第一次接触 Speckit，建议按以下顺序阅读：
  1. `SPECKIT-QUICKSTART.md`：先理解怎么进入、怎么走、怎么判断下一步。
  2. `SPECKIT-WORKFLOW.md`：再理解文档分层、阶段职责和维护边界。
  3. `SPECKIT-DESIGN.md`：最后理解设计哲学、实现原理和扩展约束。
- 如果你只想快速查某一层文件应该改哪里，本文档是首选入口。

## 1. 总控入口

- `agents/master.agent.md`
  - 负责模式判定与路由。
  - 决定请求进入 Speckit 流水线，还是进入通用任务处理模式。
  - 在当前版本中，`product-workshop-*` 属于通用模式下的可选需求强化层，而不是默认必经前置。

## 2. 阶段代理

- `agents/speckit.*.agent.md`
  - 负责定义每个阶段内部怎么做。
  - 典型内容包括：角色、阶段契约、执行步骤、输出格式、约束。

当前主要阶段如下：

| 阶段 | Agent | 主要产物 |
|---|---|---|
| Specify | `speckit.specify` | `spec.md` |
| Clarify | `speckit.clarify` | 修订后的 `spec.md` |
| Research | `speckit.research` | `research.md` |
| Plan | `speckit.plan` | `plan.md` |
| Tasks | `speckit.tasks` | `tasks.md` |
| Analyze | `speckit.analyze` | 一致性门禁结论 |
| Implement | `speckit.implement` | 实现结果与任务状态 |
| Review | `speckit.review` | 最终交付结论 |

## 3. 阶段入口文档

- `prompts/speckit.*.prompt.md`
  - 负责说明什么时候使用该阶段。
  - 面向使用者，重点回答：
    - 适用时机
    - 前置条件
    - 执行结果
    - 边界
    - 典型输入

## 4. 技能文档

- `skills/**/*.md`
  - 负责提供某类能力的评审标准或专业知识。
  - 不直接承担 Speckit 工作流阶段控制。

示例：

- `skills/reviewer/SKILL.md`
  - 提供代码评审与质量检查的标准。
  - 在 Speckit 中可被 `speckit.review` 引用，但不能替代最终验收阶段本身。

## 5. 阶段与 Skill 的关系

- Speckit stage agent 负责阶段入口、工件产出、门禁和阶段出口。
- skill 负责提供领域能力、分析方法或专项执行手段，不替代 stage agent 的工件所有权。
- 当 stage agent 与 skill 同时出现时，应优先由 stage agent 控制当前 feature 的正式文档与放行结论。

## 6. 推荐维护原则

1. 修改阶段职责时，优先更新对应 `agent`。
2. 修改阶段入口说明时，优先更新对应 `prompt`。
3. 修改评审标准或领域知识时，优先更新对应 `skill`。
4. 修改总控路由逻辑时，优先更新 `master.agent.md`。
5. 若新增 Speckit 阶段，至少同时补齐：
   - 一个 `agent`
   - 一个 `prompt`
   - 与 `master` 的阶段说明衔接

## 7. 易混淆边界

- `speckit.checklist` 是需求质量检查，不是最终验收。
- `speckit.analyze` 给出一致性门禁结论，不做最终交付判断。
- `speckit.review` 是最终验收复核阶段，负责给出最终交付结论。
- `reviewer` skill 是评审标准来源，不是 Speckit 流程的独立阶段。
- `product-workshop-*` 是通用模式下的可选需求强化层，与 Speckit SDD 流水线是**并行关系**，而非 Speckit 的前置阶段。Workshop 产出物是草稿级需求文档，不能直接作为 Speckit 正式工件跳过 Specify/Tasks 阶段。

## 8. 相关阅读

- `SPECKIT-QUICKSTART.md`: 新人上手路径、常见入口和最短理解路线。
- `SPECKIT-DESIGN.md`: 设计思想、实现原理、设计现状、维护与扩展要求。
- `SPECKIT-MAINTENANCE-CHECKLIST.md`: 修改工作流时的联动检查清单。