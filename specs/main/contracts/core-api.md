# Core API Contract（main）

## 1. Plugin 注册入口

- 符号名：RegisterCameraPlugin
- 签名：int RegisterCameraPlugin()
- 成功：返回 0
- 失败：返回非 0，对应 CoreErrorCode 或厂商错误码

## 2. CameraFactory

- Register(vendorType, creator) -> CoreResult
- Create(vendorType, outCamera) -> CoreResult

契约：
- 重复注册返回 kAlreadyExists
- 未注册 vendorType 返回 kNotFound

## 3. CameraConfigParser

- ParseFromFile(filePath, outConfig, outLayout) -> CoreResult
- ParseFromString(xmlText, outConfig, outLayout) -> CoreResult

契约：
- 缺失 vendorType/layout/camera 节点返回 kInvalidArgument
- 布局越界或 ID 冲突返回失败

## 4. CameraService

- Initialize(configPath) -> CoreResult
- Shutdown() -> CoreResult
- CaptureOnce(userId) -> CoreResult
- RunQualityInspect(frame) -> CoreResult
- RunSplice(frame) -> CoreResult
- RunDistortionCorrection(frame) -> CoreResult
- RunFourPointLocalization(imagePath) -> CoreResult

契约：
- 服务未初始化时，业务调用返回 kRuntimeError
- 输入为空返回 kInvalidArgument
- 错误需通过 CoreResult 返回，不依赖异常
