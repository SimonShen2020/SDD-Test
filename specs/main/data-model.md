# Data Model（main）

## 1. CameraDescriptor

字段：
- userId: string
- deviceInternalId: string
- row: int
- col: int

说明：描述单个相机在逻辑拓扑中的标识和位置。

## 2. CameraLayout

字段：
- rows: int (1..10)
- cols: int (1..10)
- slots: list<CameraSlot>

约束：
- userId 全局唯一
- (row, col) 全局唯一
- 坐标必须在布局范围内

## 3. PluginRegistryEntry（逻辑）

字段：
- vendorType: string
- creator: function -> shared_ptr<ICamera>

说明：由 CameraFactory 维护，用于 vendorType 到创建函数的映射。

## 4. CoreResult

字段：
- code: CoreErrorCode
- message: string
- vendorCode: int

说明：统一错误语义，支持厂商错误码透传。
