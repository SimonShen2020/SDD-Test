# Tasks: 相机模块 SDD 重构与插件化

**Input**: 设计工件来自 specs/main/  
**Prerequisites**: plan.md（必需）, spec.md（必需）, research.md（已提供）

**Tests**: 本需求明确要求单元测试与集成测试，以下任务包含测试任务并按“先失败后实现”的顺序组织。  
**Organization**: 任务按用户故事分组，支持独立实现与独立验证。

## 格式: `[ID] [P?] [Story] Description`

- **[P]**: 可并行（不同文件、无直接依赖）
- **[Story]**: 任务归属用户故事（US1/US2/US3/US4）
- 每条任务包含精确文件路径

## Phase 1: Setup（共享基础）

**Purpose**: 建立工程骨架、构建链路与文档占位

- [x] T001 创建顶层 CMake 与目录骨架：CMakeLists.txt, cmake/, src/, tests/, doc/
- [x] T002 [P] 新建 cmake/CompilerOptions.cmake（C++17、警告级别、导出宏）
- [x] T003 [P] 新建 cmake/Dependencies.cmake（GTest 与平台依赖入口）
- [x] T004 [P] 新建 cmake/Sanitizers.cmake（调试配置占位）
- [x] T005 新建 src/core/CMakeLists.txt、src/plugins/null/CMakeLists.txt、src/plugins/huarui/CMakeLists.txt、tests/CMakeLists.txt
- [x] T006 [P] 新建 docs 说明占位：doc/README-camera-config.md

---

## Phase 2: Foundational（阻塞前置）

**Purpose**: 所有用户故事共用的核心基础设施（完成前禁止进入 US 实现）

- [x] T007 定义核心错误语义：src/core/include/camera/core/CoreResult.hpp（映射 FR-022, FR-025）
- [x] T008 定义核心接口：src/core/include/camera/core/ICamera.hpp, ICameraGroup.hpp, ICameraService.hpp（映射 FR-001, FR-015）
- [x] T009 定义基础抽象类：src/core/include/camera/core/CameraBase.hpp 与 src/core/src/CameraBase.cpp（映射 FR-009）
- [x] T010 [P] 实现工厂注册表：src/core/include/camera/core/CameraFactory.hpp 与 src/core/src/CameraFactory.cpp（映射 FR-005, FR-007）
- [x] T011 [P] 实现插件加载器抽象与平台分发：src/core/include/camera/core/PluginLoader.hpp, src/core/src/PluginLoader.cpp（映射 FR-006, FR-003）
- [x] T012 [P] 实现日志宏与最小文件日志：src/common/include/camera/common/Log.hpp, src/common/src/Log.cpp（映射 FR-018, FR-019）
- [x] T013 [P] 实现公共工具函数：src/common/include/camera/common/StringUtils.hpp, FileUtils.hpp 与对应 src/common/src/*.cpp（映射 FR-010）
- [x] T014 为 Core/Plugin 源码增加模块边界头注释模板，并落地到 src/core/src/*.cpp 与 src/plugins/*/src/*.cpp（映射 FR-004）
- [x] T015 定义监控与重连策略基础配置：src/core/include/camera/core/MonitorPolicy.hpp（2s 监控、1s*10 重连）（映射 FR-017, NFR-009）

**Checkpoint**: Foundation Ready（US 可并行启动）

---

## Phase 3: User Story 1 - 配置驱动加载厂家插件（Priority: P1）

**Goal**: 通过 XML 指定厂家并动态加载插件，自注册后创建实例（替代 switch-case）  
**Independent Test**: 在仅 Core+Null 插件环境，完成“加载->注册->实例化->初始化”闭环

### Tests for US1

- [x] T016 [P] [US1] 新建插件加载集成测试：tests/integration/test_plugin_loading.cpp（覆盖加载成功/插件缺失）
- [x] T017 [P] [US1] 新建工厂注册单元测试：tests/unit/test_factory_registration.cpp（覆盖重复注册冲突）
- [x] T018 [US1] 为缺失导出入口场景补充失败用例：tests/integration/test_plugin_loading.cpp（覆盖 EC-001）

### Implementation for US1

- [x] T019 [US1] 实现插件导出入口协议：src/core/include/camera/core/PluginEntry.hpp
- [x] T020 [US1] 在 Null 插件实现注册导出：src/plugins/null/src/NullPluginEntry.cpp
- [x] T021 [US1] 实现 Core 初始化流程：src/core/src/CameraRuntime.cpp（加载插件并触发注册）
- [x] T022 [US1] 实现注册冲突保护与日志：src/core/src/CameraFactory.cpp（覆盖 EC-002）
- [x] T023 [US1] 实现插件加载错误上报：src/core/src/PluginLoader.cpp（覆盖 EC-009）

**Checkpoint**: US1 可独立运行并通过测试

---

## Phase 4: User Story 2 - MxN 布局与物理 ID 拓扑（Priority: P1）

**Goal**: 支持 1..10 的 MxN 布局与 userId 双向映射，并完成 3x8 示例配置  
**Independent Test**: 解析 3x8 XML 后得到 24 个唯一 ID 且映射正确

### Tests for US2

- [x] T024 [P] [US2] 新建布局单元测试：tests/unit/test_camera_layout.cpp（范围边界、重复 ID、格式校验）
- [x] T025 [P] [US2] 新建 XML 配置集成测试：tests/integration/test_3x8_xml_config.cpp（24 个映射校验）
- [x] T026 [US2] 补充非法布局与缺字段失败测试：tests/integration/test_3x8_xml_config.cpp（覆盖 EC-003, EC-004, EC-005）

### Implementation for US2

- [x] T027 [US2] 实现布局实体：src/core/include/camera/core/CameraLayout.hpp, src/core/src/CameraLayout.cpp（映射 FR-012, FR-013）
- [x] T028 [US2] 实现配置模型：src/core/include/camera/core/CameraConfig.hpp
- [x] T029 [US2] 实现 XML 解析器：src/core/include/camera/core/CameraConfigParser.hpp, src/core/src/CameraConfigParser.cpp（映射 FR-011）
- [x] T030 [US2] 新增 3x8 示例配置：doc/camera-config-3x8.xml（映射 FR-014）
- [x] T031 [US2] 新增配置说明文档：doc/README-camera-config.md（示例字段与命名规则）

**Checkpoint**: US2 可独立解析并通过测试

---

## Phase 5: User Story 3 - 无硬件测试闭环（Priority: P2）

**Goal**: 通过 Null 插件模拟关键相机流程，支撑 CI 与本地无设备测试  
**Independent Test**: 不依赖 SDK 运行初始化、拍照、监控、重连全部关键测试

### Tests for US3

- [x] T032 [P] [US3] 新建 Null 流程集成测试：tests/integration/test_null_capture_flow.cpp
- [x] T033 [P] [US3] 新建错误语义测试：tests/unit/test_error_semantics.cpp（CoreResult + vendorCode）
- [x] T034 [US3] 新建重连策略测试：tests/integration/test_null_reconnect_policy.cpp（1s*10 语义）

### Implementation for US3

- [x] T035 [US3] 实现 NullCamera：src/plugins/null/include/camera/plugins/null/NullCamera.hpp, src/plugins/null/src/NullCamera.cpp
- [x] T036 [US3] 实现 NullCameraGroup：src/plugins/null/include/camera/plugins/null/NullCameraGroup.hpp, src/plugins/null/src/NullCameraGroup.cpp
- [x] T037 [US3] 实现监控线程与重连委托：src/core/src/CameraMonitor.cpp（映射 FR-017, NFR-009）
- [x] T038 [US3] 在 Null 插件中实现异步回调模拟：src/plugins/null/src/NullStreamSimulator.cpp（映射 FR-016）
- [x] T039 [US3] 接入 Core 运行时到 Null 插件：src/core/src/CameraRuntime.cpp

**Checkpoint**: US3 在无硬件环境可独立验证

---

## Phase 6: User Story 4 - 厂商封装与服务层能力对齐（Priority: P2）

**Goal**: 接入 Huarui 插件并对齐服务层核心业务接口契约  
**Independent Test**: 在可用 SDK 环境至少打通最小链路（加载->注册->拍照回调）

### Tests for US4

- [x] T040 [P] [US4] 新建 Huarui 最小链路测试：tests/integration/test_huarui_minimal_flow.cpp
- [x] T041 [P] [US4] 新建服务层契约测试：tests/unit/test_service_contracts.cpp（同步接口、输入双形态、500ms 超时）
- [x] T042 [US4] 新建跨平台编译矩阵测试入口：tests/integration/test_cross_platform_build.cpp（Windows/Linux）

### Implementation for US4

- [x] T043 [US4] 封装 Huarui SDK 适配层：src/plugins/huarui/include/camera/plugins/huarui/HuaruiSdkAdapter.hpp, src/plugins/huarui/src/HuaruiSdkAdapter.cpp
- [x] T044 [US4] 实现 HuaruiCamera：src/plugins/huarui/include/camera/plugins/huarui/HuaruiCamera.hpp, src/plugins/huarui/src/HuaruiCamera.cpp
- [x] T045 [US4] 实现 Huarui 注册导出：src/plugins/huarui/src/HuaruiPluginEntry.cpp
- [x] T046 [US4] 服务层接口实现：src/core/include/camera/core/CameraService.hpp, src/core/src/CameraService.cpp（映射 FR-015）
- [x] T047 [US4] 对齐四类算法接口契约：src/core/src/CameraServiceAlgorithms.cpp（映射 FR-023, FR-024, FR-025, NFR-010）
- [x] T048 [US4] 接入 vendorCode 透传日志：src/core/src/CoreResult.cpp 与 src/common/src/Log.cpp

**Checkpoint**: US4 合同能力与最小真实链路可验证

---

## Phase 7: Polish & Cross-Cutting

**Purpose**: 跨故事一致性、文档、质量门禁

- [x] T049 [P] 完善构建说明：README.md（新增构建、测试、插件加载示例）
- [x] T050 [P] 新增快速上手文档：specs/main/quickstart.md（从构建到运行 Null 测试）
- [x] T051 输出数据模型文档：specs/main/data-model.md（CameraDescriptor/Layout/RegistryEntry）
- [x] T052 [P] 补充接口契约文档：specs/main/contracts/core-api.md
- [x] T053 运行并记录单测与集成测试结果到 specs/main/quickstart.md
- [x] T054 宪章合规复核（SOLID、RAII、边界注释、中文文档）并记录于 specs/main/quickstart.md

---

## Dependencies & Execution Order

### Phase Dependencies

- Phase 1（Setup）: 无依赖，可立即启动
- Phase 2（Foundational）: 依赖 Phase 1，阻塞所有 US
- Phase 3~6（US）: 依赖 Phase 2 完成后可并行
- Phase 7（Polish）: 依赖已完成的 US

### User Story Dependencies

- US1（P1）: 仅依赖 Foundation，可优先作为 MVP
- US2（P1）: 仅依赖 Foundation，可与 US1 并行
- US3（P2）: 依赖 US1 的运行时基础与 US2 的配置能力
- US4（P2）: 依赖 US1/US2/US3 的核心能力，接入真实 SDK

### Within Each User Story

- 测试先行：相关测试任务先写并先失败
- 核心模型/抽象 -> 服务实现 -> 集成联调
- 每个 US 完成后必须通过其独立验证用例

## Parallel Opportunities

- [P] 标记任务可并行执行
- Phase 2 中 T010/T011/T012/T013 可并行
- US1 与 US2 可在 Foundation 后并行
- US3 的测试任务 T032/T033 可并行
- US4 的测试任务 T040/T041 可并行

## Implementation Strategy

### MVP First（优先交付）

1. 完成 Phase 1 + Phase 2
2. 完成 US1（插件加载与自注册）
3. 完成 US2（布局与配置）
4. 验证后可形成首个可演示 MVP

### Incremental Delivery

1. MVP（US1+US2）
2. 无硬件闭环（US3）
3. 真实厂商接入（US4）
4. 跨切面收口（Phase 7）

## 需求映射摘要

- US1: FR-005/FR-006/FR-007/FR-022 + EC-001/EC-002/EC-009
- US2: FR-011/FR-012/FR-013/FR-014 + EC-003/EC-004/EC-005
- US3: FR-016/FR-017/FR-020 + NFR-003/NFR-009 + SC-001
- US4: FR-015/FR-023/FR-024/FR-025 + NFR-008/NFR-010 + SC-002

## Notes

- 每个任务只改动与目标相关文件，避免跨故事无关重构。
- 任务执行过程中不得破坏 Core 不依赖厂商 SDK 的红线。
- 若 SDK 环境不可用，US4 可先完成接口与模拟验证，真实链路测试延后到有环境时执行。
