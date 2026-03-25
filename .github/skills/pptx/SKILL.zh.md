---
name: pptx
description: "当涉及 .pptx 文件（创建、读取、编辑、拆分/合并、模板应用、演示稿设计、导出图像、质量检查等）时使用此技能。"
---

# PPTX 使用指南（中文译本）

## 快速参考

| 任务 | 指南 |
|------|------|
| 读取/分析 | 使用 `python -m markitdown presentation.pptx` |
| 编辑/从模板生成 | 参见 `editing.md` |
| 从零创建 | 参见 `pptxgenjs.md` |

## 读取内容

```bash
# 文本提取
python -m markitdown presentation.pptx

# 缩略图/视觉预览
python scripts/thumbnail.py presentation.pptx

# 获取原始 XML
python scripts/office/unpack.py presentation.pptx unpacked/
```

## 编辑与创建工作流（建议）

编辑流程：解包 → 修改幻灯片元素 → 清理占位符 → 重新打包并验证。

从零创建：若无模板可用，使用 `pptxgenjs` 或类似库，从模块化组件（标题、图表、图片占位）构建幻灯片。

## 设计建议（要点）

- 选择与主题匹配的主色调与配色方案，确保视觉一致性与主次分明。
- 每张幻灯片应包含至少一个视觉元素（图片、图表、图标）；避免纯文本幻灯片。
- 字体配对要有层次：标题与正文字体互补。
- 保持边距（至少 0.5"）与模块间距（0.3-0.5"）。

### 数据呈现

- 关键数字需大字号醒目显示（示例：60-72pt）。
- 对比表/前后对比/流程图常用对齐与网格布局。

### QA（必做）

将幻灯片导出为图片进行人工/自动化视觉检查，查找：重叠元素、文本溢出、占位符残留、对齐与间距不一致、低对比度文本等问题。

检查示例命令：

```bash
python -m markitdown output.pptx | grep -iE "xxxx|lorem|ipsum"
```

## 转换为图片（可视化 QA）

```bash
python scripts/office/soffice.py --headless --convert-to pdf output.pptx
pdftoppm -jpeg -r 150 output.pdf slide
```

## 依赖

- `pip install "markitdown[pptx]"`
- `pip install Pillow`
- `npm install -g pptxgenjs`
- LibreOffice (`soffice`) 与 Poppler (`pdftoppm`) 用于渲染与导出

---

需要我把某个具体的 `.pptx` 转换为图片并做自动化 QA 检查吗？提供文件路径或样例即可。
