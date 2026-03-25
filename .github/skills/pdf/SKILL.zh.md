---
name: pdf
description: "当用户需要处理 PDF（读取、提取文本/表格、合并、拆分、旋转、加水印、创建、填表、加密/解密、图片提取或对扫描件进行 OCR）时使用此技能。"
---

# PDF 处理指南（中文译本）

## 概述

本指南涵盖常见的 PDF 处理场景，包含 Python 库与命令行工具的实用示例。对于表单填充、复杂布局或 JS 库参考，请查看相应的 REFERENCE.md。

## 快速入门（Python）

使用 `pypdf` 进行基本读/写：

```python
from pypdf import PdfReader, PdfWriter

# 读取 PDF
reader = PdfReader("document.pdf")
print(f"Pages: {len(reader.pages)}")

# 提取文本
text = ""
for page in reader.pages:
    text += page.extract_text() or ""
```

## 常用库与用途

- `pypdf`：合并、拆分、旋转、添加水印、读取元数据等基本操作。
- `pdfplumber`：用于保留布局的文本与表格提取。
- `reportlab`：用于程序化创建 PDF（Canvas / Platypus）。

## 命令行工具

- `pdftotext`（poppler-utils）：文本提取（保留布局选项 `-layout`）。
- `qpdf`：合并、拆分、旋转、解密等。
- `pdftk`（如可用）：快速合并/拆分/旋转。

## 扫描件 OCR

将 PDF 每页转为图像后使用 `pytesseract` OCR：

```python
from pdf2image import convert_from_path
import pytesseract

images = convert_from_path('scanned.pdf')
text = ''
for image in images:
    text += pytesseract.image_to_string(image)
```

## 添加水印示例

```python
from pypdf import PdfReader, PdfWriter

watermark = PdfReader("watermark.pdf").pages[0]
reader = PdfReader("document.pdf")
writer = PdfWriter()

for page in reader.pages:
    page.merge_page(watermark)
    writer.add_page(page)

with open("watermarked.pdf", "wb") as out:
    writer.write(out)
```

## 提取图片

使用 `pdfimages`（poppler）可直接导出嵌入图像：

```bash
pdfimages -j input.pdf output_prefix
```

## 密码保护示例

使用 `pypdf` 添加密码：

```python
from pypdf import PdfReader, PdfWriter

reader = PdfReader('input.pdf')
writer = PdfWriter()
for page in reader.pages:
    writer.add_page(page)
writer.encrypt('userpassword','ownerpassword')
with open('encrypted.pdf','wb') as f:
    writer.write(f)
```

## 参考与下一步

如需对特定 PDF 做表单填写、批量 OCR、复杂表格提取或 JS 生态（pdf-lib）集成，我可以基于你的样例给出完整脚本或流水线。
