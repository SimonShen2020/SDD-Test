# Release Checklist - v0.1.0

日期：2026-03-25

## 1. 构建与测试

- [x] Debug 构建通过（cmake --build build --config Debug）
- [x] 自动化测试通过（ctest，15/15 通过）
- [x] 关键测试覆盖插件加载、布局解析、Null 流程、重连策略、服务契约

## 2. 二进制产物

- [x] build/src/core/Debug/CameraCore.dll
- [x] build/src/core/Debug/CameraCore.lib
- [x] build/src/plugins/null/Debug/CameraNull.dll
- [x] build/src/plugins/huarui/Debug/CameraHuarui.dll
- [x] build/tests/Debug/camera_core_tests.exe

## 3. 配置与文档

- [x] doc/camera-config-3x8.xml
- [x] doc/README-camera-config.md
- [x] doc/README.md（构建/测试/发布文档入口）
- [x] doc/CHANGELOG.md
- [x] doc/RELEASE_NOTES.md
- [x] doc/DELIVERY_MANIFEST.md
- [x] doc/RISK_REGISTER.md
- [x] doc/VERSION

## 4. SDD 工件

- [x] specs/main/spec.md
- [x] specs/main/research.md
- [x] specs/main/plan.md
- [x] specs/main/tasks.md
- [x] specs/main/quickstart.md
- [x] specs/main/data-model.md
- [x] specs/main/contracts/core-api.md
- [x] specs/main/analyze.md
- [x] specs/main/review.md

## 5. 架构与合规

- [x] Core 与 Plugin 单向依赖边界已建立
- [x] Core 不依赖厂商 SDK 头文件
- [x] 插件注册导出入口已实现
- [x] 代码与文档保持中文输出

## 6. 已知限制（发布声明）

- [x] Huarui SDK 适配层为最小骨架，真实设备行为需在 v0.2 加强验证
- [x] 服务层算法接口当前为契约实现，业务算法细节待后续迭代
