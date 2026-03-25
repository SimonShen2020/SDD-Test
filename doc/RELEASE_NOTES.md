# Release Notes - v0.1.0

发布日期：2026-03-25

## 版本定位

v0.1.0 为首个可运行的工程化基线版本，完成从需求工件到可构建、可测试代码资产的闭环。

## 交付内容

- 动态库：CameraCore、CameraNull、CameraHuarui
- 测试程序：camera_core_tests
- 配置资产：doc/camera-config-3x8.xml
- 文档资产：spec/research/plan/tasks + quickstart + contracts + data-model

## 关键能力

- 插件自注册与运行时动态加载
- MxN 布局（1..10）与 userId 双向映射
- Null 无硬件测试闭环
- Huarui 最小插件链路
- 服务层统一错误语义（CoreResult + vendorCode）

## 已知限制

- Huarui SDK 适配层为最小骨架，尚未接入真实 SDK 细节。
- 服务层算法接口为契约实现，业务算法内部逻辑未完整落地。

## 质量状态

- 构建：通过
- 测试：15/15 通过
