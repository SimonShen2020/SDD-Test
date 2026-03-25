---
name: docx
description: "当用户需要创建、读取、编辑或处理 Word 文档 (.docx) 时使用此技能。触发词包括：Word 文档、.docx、目录、页眉页脚、图片替换、批量查找替换、接受/拒绝修订、合并/拆分文档等。"
---

# DOCX 创建、编辑与分析（中文译本）

## 概述

.docx 文件本质上是包含一组 XML 文件的 ZIP 压缩包。本技能提供对创建、解析、编辑及验证 .docx 的实践指南与要点。

## 快速参考

| 任务 | 方法 |
|------|------|
| 读取/分析内容 | 使用 `pandoc` 导出或解包以查看原始 XML |
| 新建文档 | 推荐使用 `docx` / docx-js 等库生成 |
| 编辑现有文档 | 解包 → 修改 XML 或使用高层库 → 重新打包 |

### 将旧 `.doc` 转换为 `.docx`

使用 LibreOffice（soffice）无头模式批量转换：

```bash
python scripts/office/soffice.py --headless --convert-to docx document.doc
```

### 读取与提取

带修订记录导出或提取纯文本：

```bash
pandoc --track-changes=all document.docx -o output.md
python scripts/office/unpack.py document.docx unpacked/
```

### 生成与验证

使用 `docx`（JavaScript）生成后，务必运行验证脚本：

```bash
python scripts/office/validate.py doc.docx
```

### 页面与样式要点

- 明确设置页面尺寸（DXA 单位），不要依赖默认 A4/Letter 假定。
- 覆盖内置样式需使用准确的样式 ID（例如 `Heading1`）。
- 表格必须同时设置表格宽度与每个单元格宽度，优先使用 `WidthType.DXA`。
- 图片必须声明类型（png/jpg 等）并指定尺寸与替代文本。

### 列表与编号

不要硬编码 Unicode 项目符号。使用 numbering 配置并提供 LevelFormat（如 BULLET/DECIMAL）。

### 表格

表格渲染跨平台敏感：始终设置 `columnWidths` 并在每个 `TableCell` 上设置匹配的 `width`；使用 `ShadingType.CLEAR` 避免黑背景问题；单元格内边距可提高可读性。

### 分栏、页眉页脚与目录

- 使用节（Section）与 `TableOfContents` 生成目录，标题必须使用 `HeadingLevel`。
- 页眉/页脚请显式设置页边距并使用 PageNumber API 插入页码。

### 编辑流程（建议）

1. 解包原始文件到临时目录。
2. 使用高层库（docx / python-docx 等）或直接修改 XML。
3. 运行验证脚本、接受/清理修订、确保样式/表格一致后重新打包。

### 常见注意事项

- 避免在段落中直接使用换行符 `\n`，应使用独立的 Paragraph 元素。
- 覆盖样式时要同时设置 `outlineLevel` 以确保目录正确。
- 不要用表格做布局分隔线，使用段落边框或背景色替代。

---

如需针对具体文档（模板迁移、图片替换、接受修订、导出为 PDF 等）进行操作，我可以基于这个指南生成相应的脚本或代码示例。
