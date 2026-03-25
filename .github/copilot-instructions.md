# Project Guidelines

## Code Style
- 主要语言是 C++（目标标准 C++17），构建系统使用 CMake。
- 当前仓库处于设计与规范阶段；在无明确要求时，优先更新文档与设计说明，不擅自生成大规模脚手架。
- 变更遵循小步提交原则：仅修改与任务直接相关的文件，避免无关重构。
- 公共接口注释保持契约化，说明输入输出、失败语义与线程安全假设。

## Architecture
- 目标架构是 Core + Plugin 动态库模式：
  - Core 负责抽象接口、基础类、工厂、服务编排、日志与辅助工具。
  - Plugin 负责厂商实现与 SDK 封装。
- 关键依赖边界：Core 不能依赖任一厂商 SDK 头文件；Plugin 可以依赖 Core；禁止 Core 与 Plugin 循环依赖。
- 相机扩展方式采用自注册工厂，不使用集中式 switch-case 扩展点。
- 相机布局应支持 M x N 矩阵配置，并以用户自定义物理 ID 建立拓扑映射。
- 与该目标相关的设计上下文见 doc/design.md。

## Build and Test
- 当前仓库尚未提供统一的顶层 CMakeLists.txt 与可执行构建入口。
- 在新增构建或测试命令前，先检查仓库是否已存在可复用脚本或约定；不要臆造命令。
- 若任务要求补齐工程，应优先提供：
  - CameraCore 动态库
  - 至少一个厂商 Plugin 动态库（例如 Huarui）
  - 一个 Null Plugin 动态库用于无硬件测试
  - 一个基于 GTest 的测试可执行程序

## Conventions
- 工作流与维护边界优先参考以下文档：
  - .github/README.md
  - .github/SPECKIT-WORKFLOW.md
  - .github/SPECKIT-DESIGN.md
  - .github/SPECKIT-MAINTENANCE-CHECKLIST.md
- 如果实现涉及 Core 或 Plugin 源码，文件头必须显式声明模块层级与依赖红线。
- 日志调用通过宏抽象，不在业务代码中绑定具体日志库实现。
- 设计或实现新增厂商时，应复用 Null Plugin 作为最小样板并保持接口一致性。
