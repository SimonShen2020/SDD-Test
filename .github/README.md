# .github Documentation Index

本文档是 `.github` 目录下 Speckit 相关文档的统一入口页。

如果你刚进入这个仓库，不确定应该先看哪一份文档，请从这里开始。

## 1. 先看什么

### 如果你是新人使用者

建议按这个顺序阅读：

1. `SPECKIT-QUICKSTART.md`
2. `SPECKIT-WORKFLOW.md`
3. `SPECKIT-DESIGN.md`

### 如果你是维护者

建议按这个顺序阅读：

1. `SPECKIT-WORKFLOW.md`
2. `SPECKIT-DESIGN.md`
3. `SPECKIT-MAINTENANCE-CHECKLIST.md`

### 如果你准备修改工作流设计

建议按这个顺序阅读：

1. `SPECKIT-DESIGN.md`
2. `SPECKIT-WORKFLOW.md`
3. `SPECKIT-MAINTENANCE-CHECKLIST.md`

## 2. 文档导航

### `SPECKIT-QUICKSTART.md`

适合谁：
- 第一次接触 Speckit 的成员
- 想快速知道“什么时候进入 Speckit、每一步看什么”的人

主要内容：
- 进入 Speckit 的条件
- 8 个阶段的最短理解路径
- 新人最常见误区
- 一个最小通过样例应该具备哪些工件

### `SPECKIT-WORKFLOW.md`

适合谁：
- 想快速查结构分层的人
- 想知道某类改动应该改 `agent`、`prompt` 还是 `skill` 的人

主要内容：
- 总控入口
- 阶段代理
- 阶段入口文档
- 技能文档
- 阶段与 skill 的边界
- `product-workshop-*` 与 Speckit 的当前关系
- 维护原则与易混淆边界

### `SPECKIT-DESIGN.md`

适合谁：
- 想理解设计哲学和实现原理的人
- 准备做较大规则调整的人

主要内容：
- 工作流要解决的问题
- 设计哲学
- 总控、阶段、prompt、skill、脚本、宪章的实现机制
- 当前设计现状
- 维护与扩展时必须遵守的约束

### `SPECKIT-MAINTENANCE-CHECKLIST.md`

适合谁：
- 准备实际修改 Speckit 文档、规则或脚本的维护者

主要内容：
- 改动前先问的 3 个问题
- 按改动类型的联动检查表
- 按风险级别决定复查强度
- 推荐验证顺序
- 常见维护错误与最小交付标准

## 3. 推荐阅读路线

### 路线 A：10 分钟理解 Speckit

- 读 `SPECKIT-QUICKSTART.md`
- 再读 `SPECKIT-WORKFLOW.md`

### 路线 B：开始维护 Speckit

- 读 `SPECKIT-WORKFLOW.md`
- 再读 `SPECKIT-DESIGN.md`
- 最后对照 `SPECKIT-MAINTENANCE-CHECKLIST.md`

### 路线 C：准备扩展 Speckit

- 先读 `SPECKIT-DESIGN.md`
- 再读 `SPECKIT-WORKFLOW.md`
- 最后用 `SPECKIT-MAINTENANCE-CHECKLIST.md` 做联动核对

## 4. 一句话理解四份文档的分工

- `SPECKIT-QUICKSTART.md`：告诉你怎么用
- `SPECKIT-WORKFLOW.md`：告诉你结构怎么分
- `SPECKIT-DESIGN.md`：告诉你为什么这样设计
- `SPECKIT-MAINTENANCE-CHECKLIST.md`：告诉你改动时要一起检查什么

## 5. 相关目录

- `agents/`: 阶段代理与总控逻辑
- `prompts/`: 阶段入口说明
- `skills/`: 专业能力与标准

## 6. 建议

如果你只是阅读，不要直接从单个 `agent` 文件开始。

先从上面的入口文档理解整体，再下钻到具体 `agent` / `prompt` / `skill`，会更容易避免误解边界。