# 相机配置说明

本文档说明相机 XML 配置结构、字段约束和 3x8 示例文件。

## 1. 文件位置

- 示例文件：doc/camera-config-3x8.xml

## 2. 根节点

- 根节点：`cameraConfig`
- 推荐属性：`version`（例如 `1.0`）

## 3. 必填字段

1. `vendorType`
	- 含义：目标厂家插件类型
	- 示例：`null`、`huarui`
2. `layout`
	- 属性：`rows`、`cols`
	- 取值范围：`1..10`
3. `cameras`
	- 子节点：若干 `camera`
	- 每个 `camera` 必填属性：`userId`、`deviceId`、`row`、`col`

## 4. userId 命名规则

- 规则：厂家简称 + 行字母 + 列数字
- 示例：`DHAA0`、`DHAB7`、`DHAC3`

其中：
- `DHA`：厂家简称示例
- `A/B/C`：行标识（0/1/2）
- `0..7`：列索引

## 5. 一致性约束

1. `userId` 全局唯一。
2. `(row, col)` 位置唯一。
3. 每个 `camera` 的 `row/col` 必须落在 `layout` 定义范围内。
4. 相机数量应与布局一致（3x8 时建议 24 台）。

## 6. 3x8 示例说明

- 布局：`rows=3`、`cols=8`
- 行范围：`0..2`
- 列范围：`0..7`
- 示例中给出 24 个相机槽位，覆盖全部位置。
