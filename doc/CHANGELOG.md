# Changelog

## [0.1.0] - 2026-03-25

### Added

- 建立 C++17 + CMake 的 Core + Plugin 工程骨架。
- 新增 CameraCore、CameraNull、CameraHuarui 三个动态库目标与测试可执行目标。
- 实现核心抽象与基础设施：
  - CoreResult、CameraBase、CameraFactory、PluginLoader、CameraRuntime
  - CameraLayout、CameraConfigParser、CameraMonitor、CameraService
  - 日志宏与公共工具（StringUtils、FileUtils）
- 新增 Null 插件与 Huarui 插件最小注册链路。
- 新增 3x8 XML 配置示例与配置说明文档。
- 新增完整测试集合（单元 + 集成），覆盖插件加载、布局解析、重连策略、服务契约。

### Verified

- Debug 构建通过。
- 测试结果：15/15 通过（ctest）。
