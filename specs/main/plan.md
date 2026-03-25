# 实施计划：相机模块 SDD 重构与插件化

**Branch**: main | **Date**: 2026-03-25 | **Spec**: specs/main/spec.md  
**Input**: Feature specification from specs/main/spec.md and evidence from specs/main/research.md

## Summary

本计划以“可扩展、可测试、低耦合”为主线，先构建 CameraCore + CameraNull 的最小闭环，再接入 CameraHuarui，最终形成可持续扩展的 Core + Plugin 动态库体系。实现策略遵循宪章：SOLID、C++17、RAII、现代 CMake、中文文档与可追溯测试证据。

## Technical Context

**Language/Version**: C++17  
**Primary Dependencies**: CMake 3.16+、GTest、厂商 SDK（仅 Huarui，路径 SDK/05_Huarui）  
**Storage**: 文件（XML 配置、日志文件）  
**Testing**: GTest（unit + integration）  
**Target Platform**: Windows(MSVC 2022) + Linux(GCC/Clang)  
**Project Type**: C++ 多动态库工程（Core + Plugins + Tests）  
**Performance Goals**: 回调处理 p99 < 50ms；监控线程周期 2s；重连间隔 1s 最多 10 次  
**Constraints**: Core 禁止依赖厂商 SDK；新增厂家 Core 修改量应为 0  
**Scale/Scope**: 单 feature 首版交付 1 个真实插件(Huarui) + 1 个虚拟插件(Null) + 3x8 配置与测试闭环

## Constitution Check

### Gate A：架构与职责

- 结论：通过。
- 依据：采用 Core + Plugin 分库、抽象先行、单向依赖，符合 SOLID 与 OCP。

### Gate B：适度设计

- 结论：通过。
- 依据：首版只落地 Huarui + Null，避免一次性引入多厂商复杂性。

### Gate C：质量与性能

- 结论：通过（含跟踪项）。
- 跟踪项：回调性能验证依赖后续压测样例；计划中已纳入性能测试任务。

### Gate D：C++ 与构建规范

- 结论：通过。
- 依据：C++17、RAII、现代 CMake、GTest 作为强制实施项。

## Project Structure

### Documentation (this feature)

```text
specs/main/
├── spec.md
├── research.md
├── plan.md
├── data-model.md          # Phase 1 输出
├── quickstart.md          # Phase 1 输出
├── contracts/             # Phase 1 输出
└── tasks.md               # Phase 2 输出
```

### Source Code (repository root)

```text
cmake/
├── CompilerOptions.cmake
├── Dependencies.cmake
└── Sanitizers.cmake

src/
├── core/
│   ├── CMakeLists.txt
│   ├── include/camera/core/
│   │   ├── ICamera.hpp
│   │   ├── ICameraGroup.hpp
│   │   ├── ICameraService.hpp
│   │   ├── CameraBase.hpp
│   │   ├── CameraFactory.hpp
│   │   ├── CameraLayout.hpp
│   │   ├── PluginLoader.hpp
│   │   └── CoreResult.hpp
│   └── src/
├── plugins/
│   ├── huarui/
│   │   ├── CMakeLists.txt
│   │   ├── include/camera/plugins/huarui/
│   │   └── src/
│   └── null/
│       ├── CMakeLists.txt
│       ├── include/camera/plugins/null/
│       └── src/
├── common/
│   ├── CMakeLists.txt
│   ├── include/camera/common/
│   │   ├── Log.hpp
│   │   ├── FileUtils.hpp
│   │   └── StringUtils.hpp
│   └── src/
└── app/
    ├── CMakeLists.txt
    └── smoke_main.cpp

tests/
├── CMakeLists.txt
├── unit/
│   ├── test_camera_layout.cpp
│   ├── test_factory_registration.cpp
│   └── test_error_semantics.cpp
└── integration/
    ├── test_plugin_loading.cpp
    ├── test_null_capture_flow.cpp
    └── test_3x8_xml_config.cpp

doc/
└── camera-config-3x8.xml

SDK/
└── 05_Huarui/
```

**Structure Decision**: 采用 C++ 多动态库结构。Core/Plugin 分离，公共工具独立到 common，测试按 unit/integration 分层。

## 分阶段实施方案

### Phase 0：工程骨架与构建闭环（强制项）

- 建立顶层与分模块 CMakeLists。
- 输出目标：CameraCore、CameraNull、CameraHuarui、tests 可执行。
- 引入 GTest，配置基本编译选项（C++17、警告级别、导出符号宏）。

### Phase 1：核心抽象与注册加载机制（强制项）

- 定义 ICamera/CameraBase/CameraFactory/PluginLoader/CoreResult。
- 完成插件注册协议（导出入口 + 工厂注册）。
- 完成日志宏抽象与简易文件日志实现。
- 明确源码文件头边界注释模板并强制落地。

### Phase 2：布局模型与配置解析（强制项）

- 实现 CameraLayout（1..10）及 userId 双向映射校验。
- 实现 XML 解析器并支持 vendorType、rows、cols、camera 列表。
- 交付 3x8 配置示例：doc/camera-config-3x8.xml。

### Phase 3：Null 插件与无硬件测试（强制项）

- 实现 NullCamera 与 NullCameraGroup。
- 跑通初始化、拍照模拟、状态监控、重连路径。
- 交付单元与集成测试，覆盖 FR-005/FR-012/FR-017/FR-020/SC-001。

### Phase 4：Huarui 插件封装（按需项）

- 在插件层封装 SDK 调用，避免业务层直接触达 SDK API。
- 对齐 CoreResult 错误语义并透传 vendorCode。
- 验证最小真实链路：加载->注册->实例化->拍照回调。

### Phase 5：业务服务层接口对齐（按需项）

- 对齐质量检测、嫁接、畸变矫正、四点定位接口契约。
- 默认同步调用，支持内存与文件输入，默认 500ms 超时。

### Phase 6：增强项（可延后项）

- 高级日志后端适配（如 spdlog）。
- 性能压测与长期稳定性测试。
- 配置版本迁移与高级诊断工具。

## 强制项 / 按需项 / 可延后项

### 强制项（必须在首版完成）

- Core + Plugin 单向依赖与动态加载。
- 单例工厂自注册机制。
- CameraLayout MxN（1..10）与 3x8 配置示例。
- Null 插件与 GTest 无硬件测试闭环。
- 日志宏抽象与最小可用日志实现。

### 按需项（建议首版后半段完成）

- Huarui 插件深层 SDK 封装细节。
- 四类算法业务接口参数细化与更多契约测试。

### 可延后项（不阻塞首版验收）

- 高级性能优化与压测平台化。
- 更复杂的配置兼容迁移能力。

## 风险与缓解

- 动态库加载差异风险：以 IPluginLoader 屏蔽平台 API，统一错误语义。
- 回调并发风险：统一状态机 + 原子状态 + 最小锁域。
- SDK 侵入风险：插件适配层隔离，Core 仅依赖抽象。
- 测试环境差异风险：Null 插件优先，真实 SDK 测试独立分层。

## 验收与里程碑

- M1：构建闭环完成（Core/Null/tests 可编译运行）。
- M2：注册加载 + XML + 3x8 拓扑通过测试。
- M3：Huarui 插件最小链路打通。
- M4：服务接口契约与关键回归测试达标。

## Complexity Tracking

当前无宪章违例，无需额外复杂度豁免记录。
