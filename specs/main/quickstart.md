# Quickstart（main）

## 1. 环境

- Windows 10+
- Visual Studio 2022 (MSVC)
- CMake 3.16+

## 2. 构建步骤

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## 3. 测试步骤

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

## 4. 本次验证结果（2026-03-25）

- 编译状态：通过
- 测试结果：15/15 通过
- 产物路径（Debug）：
  - build/src/core/Debug/CameraCore.dll
  - build/src/plugins/null/Debug/CameraNull.dll
  - build/src/plugins/huarui/Debug/CameraHuarui.dll
  - build/tests/Debug/camera_core_tests.exe
- 关键覆盖：
  - 插件加载与缺失处理
  - 工厂重复注册保护
  - 布局与 3x8 配置解析
  - Null 流程与重连策略
  - Huarui 插件最小加载链路
  - 服务层接口契约错误语义

## 5. 宪章合规复核摘要

- SOLID：已拆分 Core/Plugin 责任，避免集中式 switch 扩展。
- RAII：线程、文件、锁等资源使用标准对象管理。
- 依赖红线：Core 不依赖厂商 SDK，SDK 逻辑位于 Plugin。
- 文档语言：当前工程文档和注释均使用中文。
