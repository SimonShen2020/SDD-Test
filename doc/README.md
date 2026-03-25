# CameraSDD

CameraSDD 是一个 C++17 的相机模块重构工程，采用 Core + Plugin 动态库架构。

当前版本：v0.1.0（见 VERSION）

## 架构产物

- CameraCore.dll
- CameraNull.dll
- CameraHuarui.dll
- camera_core_tests.exe

## 构建

在仓库根目录执行：

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## 测试

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

## 插件加载约定

- 插件注册导出函数名：RegisterCameraPlugin
- 插件通过 Core 工厂注册 vendorType 到相机创建器
- Core 侧禁止包含任何厂商 SDK 头文件

## 配置示例

- 3x8 示例：doc/camera-config-3x8.xml
- 配置说明：doc/README-camera-config.md

## 发布文档

- 变更记录：doc/CHANGELOG.md
- 发布说明：doc/RELEASE_NOTES.md
- 交付清单：doc/DELIVERY_MANIFEST.md
- 风险登记：doc/RISK_REGISTER.md
- 阶段快速验证：specs/main/quickstart.md
