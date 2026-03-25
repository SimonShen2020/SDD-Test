---
name: docx
description: "Use this skill whenever the user wants to create, read, edit, or manipulate Word documents (.docx files). Triggers include: any mention of 'Word doc', 'word document', '.docx', or requests to produce professional documents with formatting like tables of contents, headings, page numbers, or letterheads. Also use when extracting or reorganizing content from .docx files, inserting or replacing images in documents, performing find-and-replace in Word files, working with tracked changes or comments, or converting content into a polished Word document. If the user asks for a 'report', 'memo', 'letter', 'template', or similar deliverable as a Word or .docx file, use this skill. Do NOT use for PDFs, spreadsheets, Google Docs, or general coding tasks unrelated to document generation."
license: Proprietary. LICENSE.txt has complete terms
---

# DOCX creation, editing, and analysis

## Overview

一个 .docx 文件是包含 XML 文件的 ZIP 存档。

## 快速参考

| 任务 | 方法 |
|------|------|
| 读取/分析内容 | 使用 `pandoc` 或解包以获取原始 XML |
| 新建文档 | 使用 `docx-js`（见下方“创建新文档”部分） |
| 编辑现有文档 | 解包 → 编辑 XML → 重新打包（见下方“编辑现有文档”部分） |

### 将 .doc 转换为 .docx

旧版 `.doc` 文件在编辑前必须先转换为 .docx：

```bash
python scripts/office/soffice.py --headless --convert-to docx document.doc
```

### 读取内容

```bash
# 带修订记录的文本提取
pandoc --track-changes=all document.docx -o output.md

# 访问原始 XML
python scripts/office/unpack.py document.docx unpacked/
```

### 转换为图片

```bash
python scripts/office/soffice.py --headless --convert-to pdf document.docx
pdftoppm -jpeg -r 150 document.pdf page
```

### 接受修订（Accept Tracked Changes）

若需要生成已接受所有修订的干净文档（需要 LibreOffice），可运行：

```bash
python scripts/accept_changes.py input.docx output.docx
```

---

## 创建新文档

使用 JavaScript 生成 .docx 文件，然后进行验证。安装：`npm install -g docx`

### 环境配置
```javascript
const { Document, Packer, Paragraph, TextRun, Table, TableRow, TableCell, ImageRun,
        Header, Footer, AlignmentType, PageOrientation, LevelFormat, ExternalHyperlink,
        InternalHyperlink, Bookmark, FootnoteReferenceRun, PositionalTab,
        PositionalTabAlignment, PositionalTabRelativeTo, PositionalTabLeader,
        TabStopType, TabStopPosition, Column, SectionType,
        TableOfContents, HeadingLevel, BorderStyle, WidthType, ShadingType,
        VerticalAlign, PageNumber, PageBreak } = require('docx');

const doc = new Document({ sections: [{ children: [/* content */] }] });
Packer.toBuffer(doc).then(buffer => fs.writeFileSync("doc.docx", buffer));
```

### 验证
创建文件后请进行验证。如验证失败，解包、修复 XML 再重新打包。
```bash
python scripts/office/validate.py doc.docx
```

### 页面尺寸

```javascript
// 重要：docx-js 默认纸张为 A4，而非 US Letter
// 为保证一致性，请始终显式设置页面尺寸
sections: [{
  properties: {
    page: {
      size: {
        width: 12240,   // DXA 中的 8.5 英寸
        height: 15840   // DXA 中的 11 英寸
      },
      margin: { top: 1440, right: 1440, bottom: 1440, left: 1440 } // 1 英寸边距
    }
  },
  children: [/* content */]
}]
```

**常用页面尺寸（DXA 单位，1440 DXA = 1 英寸）：**

| 纸张 | 宽度 | 高度 | 可用内容宽度（1" 边距） |
|------|------|------|------------------------|
| US Letter | 12,240 | 15,840 | 9,360 |
| A4（默认） | 11,906 | 16,838 | 9,026 |

**横向（Landscape）说明：** docx-js 会在内部交换宽高，所以请传入纵向（portrait）尺寸，库会处理交换：
```javascript
size: {
  width: 12240,   // 将短边作为 width
  height: 15840,  // 将长边作为 height
  orientation: PageOrientation.LANDSCAPE  // docx-js 会在 XML 中交换
},
// 内容宽度 = 15840 - 左边距 - 右边距（使用长边）
```

### 样式（覆盖内置标题）

建议使用 Arial 作为默认字体（跨平台支持良好）。标题保留为黑色以确保可读性。

```javascript
const doc = new Document({
  styles: {
    default: { document: { run: { font: "Arial", size: 24 } } }, // 默认 12pt
    paragraphStyles: [
      // 重要：使用精确 ID 来覆盖内置样式
      { id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 32, bold: true, font: "Arial" },
        paragraph: { spacing: { before: 240, after: 240 }, outlineLevel: 0 } }, // outlineLevel 对目录必需
      { id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 28, bold: true, font: "Arial" },
        paragraph: { spacing: { before: 180, after: 180 }, outlineLevel: 1 } },
    ]
  },
  sections: [{
    children: [
      new Paragraph({ heading: HeadingLevel.HEADING_1, children: [new TextRun("Title")] }),
    ]
  }]
});
```

### 列表（切勿使用 unicode 项目符号）

```javascript
// ❌ 错误示例 - 不要手动插入符号字符
new Paragraph({ children: [new TextRun("• Item")] })  // 错误
new Paragraph({ children: [new TextRun("\u2022 Item")] })  // 错误

// ✅ 正确示例 - 使用 numbering 配置与 LevelFormat.BULLET
const doc = new Document({
  numbering: {
    config: [
      { reference: "bullets",
        levels: [{ level: 0, format: LevelFormat.BULLET, text: "•", alignment: AlignmentType.LEFT,
          style: { paragraph: { indent: { left: 720, hanging: 360 } } } }] },
      { reference: "numbers",
        levels: [{ level: 0, format: LevelFormat.DECIMAL, text: "%1.", alignment: AlignmentType.LEFT,
          style: { paragraph: { indent: { left: 720, hanging: 360 } } } }] },
    ]
  },
  sections: [{
    children: [
      new Paragraph({ numbering: { reference: "bullets", level: 0 },
        children: [new TextRun("Bullet item")] }),
      new Paragraph({ numbering: { reference: "numbers", level: 0 },
        children: [new TextRun("Numbered item")] }),
    ]
  }]
});

// ⚠️ 注意：每个 reference 都会创建独立的编号序列
// 相同 reference = 连续编号（1,2,3 接着 4,5,6）
// 不同 reference = 重置编号（1,2,3 然后再次 1,2,3）
```

### 表格

**重要：表格需要双重宽度设置** — 在表格上设置 `columnWidths`，并在每个单元格上设置 `width`。缺少任一项可能导致某些平台上渲染错误。

```javascript
// 重要：始终设置表格宽度以保证一致渲染
// 重要：使用 ShadingType.CLEAR（不要用 SOLID），以避免黑色背景
const border = { style: BorderStyle.SINGLE, size: 1, color: "CCCCCC" };
const borders = { top: border, bottom: border, left: border, right: border };

new Table({
  width: { size: 9360, type: WidthType.DXA }, // 始终使用 DXA（百分比会在 Google Docs 中出问题）
  columnWidths: [4680, 4680], // 必须等于表格总宽度（DXA: 1440 = 1 英寸）
  rows: [
    new TableRow({
      children: [
        new TableCell({
          borders,
          width: { size: 4680, type: WidthType.DXA }, // 每个单元格也必须设置
          shading: { fill: "D5E8F0", type: ShadingType.CLEAR }, // 使用 CLEAR 而非 SOLID
          margins: { top: 80, bottom: 80, left: 120, right: 120 }, // 单元格内边距（不计入宽度）
          children: [new Paragraph({ children: [new TextRun("Cell")] })]
        })
      ]
    })
  ]
})
```

**表格宽度计算：**

始终使用 `WidthType.DXA` — `WidthType.PERCENTAGE` 会在 Google Docs 中失效。

```javascript
// 表格宽度 = columnWidths 的和 = 内容宽度
// US Letter（1" 边距）：12240 - 2880 = 9360 DXA
width: { size: 9360, type: WidthType.DXA },
columnWidths: [7000, 2360]  // 必须等于表格宽度之和
```

**宽度规则：**
- **始终使用 `WidthType.DXA`** — 切勿使用 `WidthType.PERCENTAGE`（与 Google Docs 不兼容）
- 表格宽度必须等于 `columnWidths` 的和
- 单元格的 `width` 必须与对应的 `columnWidth` 匹配
- 单元格的 `margins` 是内部内边距，会缩小可用内容区，而不是增加单元格宽度
- 若要创建全宽表格：使用内容宽度（页面宽度减去左右边距）
### 图片

```javascript
// 重要：必须提供 type 参数
new Paragraph({
  children: [new ImageRun({
    type: "png", // 必填：png, jpg, jpeg, gif, bmp, svg
    data: fs.readFileSync("image.png"),
    transformation: { width: 200, height: 150 },
    altText: { title: "Title", description: "Desc", name: "Name" } // 三项均需提供
  })]
})
```

### 分页（Page Breaks）

```javascript
// 重要：PageBreak 必须放在 Paragraph 内
new Paragraph({ children: [new PageBreak()] })

// 或使用 pageBreakBefore
new Paragraph({ pageBreakBefore: true, children: [new TextRun("New page")] })
```

### 超链接

```javascript
// 外部链接
new Paragraph({
  children: [new ExternalHyperlink({
    children: [new TextRun({ text: "Click here", style: "Hyperlink" })],
    link: "https://example.com",
  })]
})

// 内部链接（书签 + 引用）
// 1. 在目标位置创建书签
new Paragraph({ heading: HeadingLevel.HEADING_1, children: [
  new Bookmark({ id: "chapter1", children: [new TextRun("Chapter 1")] }),
]})
// 2. 链接到书签
new Paragraph({ children: [new InternalHyperlink({
  children: [new TextRun({ text: "See Chapter 1", style: "Hyperlink" })],
  anchor: "chapter1",
})]})
```

### 脚注（Footnotes）

```javascript
const doc = new Document({
  footnotes: {
    1: { children: [new Paragraph("Source: Annual Report 2024")] },
    2: { children: [new Paragraph("See appendix for methodology")] },
  },
  sections: [{
    children: [new Paragraph({
      children: [
        new TextRun("Revenue grew 15%"),
        new FootnoteReferenceRun(1),
        new TextRun(" using adjusted metrics"),
        new FootnoteReferenceRun(2),
      ],
    })]
  }]
});
```

### 制表位（Tab Stops）

```javascript
// 右对齐同一行的文本（例如在标题右侧显示日期）
new Paragraph({
  children: [
    new TextRun("Company Name"),
    new TextRun("\tJanuary 2025"),
  ],
  tabStops: [{ type: TabStopType.RIGHT, position: TabStopPosition.MAX }],
})

// 点点引导（例如目录样式）
new Paragraph({
  children: [
    new TextRun("Introduction"),
    new TextRun({ children: [
      new PositionalTab({
        alignment: PositionalTabAlignment.RIGHT,
        relativeTo: PositionalTabRelativeTo.MARGIN,
        leader: PositionalTabLeader.DOT,
      }),
      "3",
    ]}),
  ],
})
```

### 多栏布局

```javascript
// 等宽列
sections: [{
  properties: {
    column: {
      count: 2,          // 列数
      space: 720,        // 列间距，DXA（720 = 0.5 英寸）
      equalWidth: true,
      separate: true,    // 列间竖线
    },
  },
  children: [/* 内容按栏自然流动 */]
}]

// 自定义宽度列（需将 equalWidth 设为 false）
sections: [{
  properties: {
    column: {
      equalWidth: false,
      children: [
        new Column({ width: 5400, space: 720 }),
        new Column({ width: 3240 }),
      ],
    },
  },
  children: [/* 内容 */]
}]
```

如需强制换栏，可通过新节并使用 `type: SectionType.NEXT_COLUMN`。

### 目录（Table of Contents）

```javascript
// 重要：标题必须使用 HeadingLevel，不要使用自定义样式
new TableOfContents("Table of Contents", { hyperlink: true, headingStyleRange: "1-3" })
```

### 页眉/页脚（Headers/Footers）

```javascript
sections: [{
  properties: {
    page: { margin: { top: 1440, right: 1440, bottom: 1440, left: 1440 } } // 1440 = 1 英寸
  },
  headers: {
    default: new Header({ children: [new Paragraph({ children: [new TextRun("Header")] })] })
  },
  footers: {
    default: new Footer({ children: [new Paragraph({
      children: [new TextRun("Page "), new TextRun({ children: [PageNumber.CURRENT] })]
    })] })
  },
  children: [/* content */]
}]
```

### docx-js 使用的关键规则

- **显式设置页面尺寸** — docx-js 默认 A4；美国文档请使用 US Letter（12240 x 15840 DXA）
- **横向：请传入纵向尺寸** — docx-js 会在内部交换宽高；将短边作为 `width`、长边作为 `height`，并设置 `orientation: PageOrientation.LANDSCAPE`
- **切勿使用 `\n`** — 使用独立的 Paragraph 元素
- **切勿使用 unicode 项目符号** — 使用 `LevelFormat.BULLET` 并配置 numbering
- **PageBreak 必须在 Paragraph 中** — 独立的 PageBreak 会生成无效 XML
- **ImageRun 需要 `type` 参数** — 始终指定 png/jpg 等格式
- **始终使用 DXA 设置表格 `width`** — 切勿使用 `WidthType.PERCENTAGE`（会在 Google Docs 中失效）
- **表格需要双重宽度设置** — 同时设置 `columnWidths` 和单元格 `width`，二者必须匹配
- **表格宽度 = columnWidths 之和** — 使用 DXA 时确保总和精确
- **始终添加单元格内边距** — 使用 `margins: { top: 80, bottom: 80, left: 120, right: 120 }` 提高可读性
- **使用 `ShadingType.CLEAR`** — 不要使用 SOLID 作为表格底色
- **不要用表格做分隔线** — 单元格有最小高度，在头/脚中会渲染为空白盒；改用段落底部边框 `border: { bottom: { style: BorderStyle.SINGLE, size: 6, color: "2E75B6", space: 1 } }`。若需两栏页脚，请使用制表位而非表格（参见“制表位”）
- **目录仅支持 HeadingLevel** — 标题段落不能使用自定义样式
- **覆盖内置样式时使用精确 ID** — 如 "Heading1", "Heading2" 等
- **包含 `outlineLevel`** — 目录所需（H1 对应 0，H2 对应 1，依此类推）

---

## 编辑现有文档

**请按以下 3 个步骤顺序操作。**

### Step 1: Unpack
```bash
python scripts/office/unpack.py document.docx unpacked/
```
Extracts XML, pretty-prints, merges adjacent runs, and converts smart quotes to XML entities (`&#x201C;` etc.) so they survive editing. Use `--merge-runs false` to skip run merging.

### Step 2: Edit XML

Edit files in `unpacked/word/`. See XML Reference below for patterns.

**Use "Claude" as the author** for tracked changes and comments, unless the user explicitly requests use of a different name.

**Use the Edit tool directly for string replacement. Do not write Python scripts.** Scripts introduce unnecessary complexity. The Edit tool shows exactly what is being replaced.

**CRITICAL: Use smart quotes for new content.** When adding text with apostrophes or quotes, use XML entities to produce smart quotes:
```xml
<!-- Use these entities for professional typography -->
<w:t>Here&#x2019;s a quote: &#x201C;Hello&#x201D;</w:t>
```
| Entity | Character |
|--------|-----------|
| `&#x2018;` | ‘ (left single) |
| `&#x2019;` | ’ (right single / apostrophe) |
| `&#x201C;` | “ (left double) |
| `&#x201D;` | ” (right double) |

**Adding comments:** Use `comment.py` to handle boilerplate across multiple XML files (text must be pre-escaped XML):
```bash
python scripts/comment.py unpacked/ 0 "Comment text with &amp; and &#x2019;"
python scripts/comment.py unpacked/ 1 "Reply text" --parent 0  # reply to comment 0
python scripts/comment.py unpacked/ 0 "Text" --author "Custom Author"  # custom author name
```
Then add markers to document.xml (see Comments in XML Reference).

### Step 3: Pack
```bash
python scripts/office/pack.py unpacked/ output.docx --original document.docx
```
Validates with auto-repair, condenses XML, and creates DOCX. Use `--validate false` to skip.

**Auto-repair will fix:**
- `durableId` >= 0x7FFFFFFF (regenerates valid ID)
- Missing `xml:space="preserve"` on `<w:t>` with whitespace

**Auto-repair won't fix:**
- Malformed XML, invalid element nesting, missing relationships, schema violations

### Common Pitfalls

- **Replace entire `<w:r>` elements**: When adding tracked changes, replace the whole `<w:r>...</w:r>` block with `<w:del>...<w:ins>...` as siblings. Don't inject tracked change tags inside a run.
- **Preserve `<w:rPr>` formatting**: Copy the original run's `<w:rPr>` block into your tracked change runs to maintain bold, font size, etc.

---

## XML Reference

### Schema Compliance

- **Element order in `<w:pPr>`**: `<w:pStyle>`, `<w:numPr>`, `<w:spacing>`, `<w:ind>`, `<w:jc>`, `<w:rPr>` last
- **Whitespace**: Add `xml:space="preserve"` to `<w:t>` with leading/trailing spaces
- **RSIDs**: Must be 8-digit hex (e.g., `00AB1234`)

### Tracked Changes

**Insertion:**
```xml
<w:ins w:id="1" w:author="Claude" w:date="2025-01-01T00:00:00Z">
  <w:r><w:t>inserted text</w:t></w:r>
</w:ins>
```

**Deletion:**
```xml
<w:del w:id="2" w:author="Claude" w:date="2025-01-01T00:00:00Z">
  <w:r><w:delText>deleted text</w:delText></w:r>
</w:del>
```

**Inside `<w:del>`**: Use `<w:delText>` instead of `<w:t>`, and `<w:delInstrText>` instead of `<w:instrText>`.

**Minimal edits** - only mark what changes:
```xml
<!-- Change "30 days" to "60 days" -->
<w:r><w:t>The term is </w:t></w:r>
<w:del w:id="1" w:author="Claude" w:date="...">
  <w:r><w:delText>30</w:delText></w:r>
</w:del>
<w:ins w:id="2" w:author="Claude" w:date="...">
  <w:r><w:t>60</w:t></w:r>
</w:ins>
<w:r><w:t> days.</w:t></w:r>
```

**Deleting entire paragraphs/list items** - when removing ALL content from a paragraph, also mark the paragraph mark as deleted so it merges with the next paragraph. Add `<w:del/>` inside `<w:pPr><w:rPr>`:
```xml
<w:p>
  <w:pPr>
    <w:numPr>...</w:numPr>  <!-- list numbering if present -->
    <w:rPr>
      <w:del w:id="1" w:author="Claude" w:date="2025-01-01T00:00:00Z"/>
    </w:rPr>
  </w:pPr>
  <w:del w:id="2" w:author="Claude" w:date="2025-01-01T00:00:00Z">
    <w:r><w:delText>Entire paragraph content being deleted...</w:delText></w:r>
  </w:del>
</w:p>
```
Without the `<w:del/>` in `<w:pPr><w:rPr>`, accepting changes leaves an empty paragraph/list item.

**Rejecting another author's insertion** - nest deletion inside their insertion:
```xml
<w:ins w:author="Jane" w:id="5">
  <w:del w:author="Claude" w:id="10">
    <w:r><w:delText>their inserted text</w:delText></w:r>
  </w:del>
</w:ins>
```

**Restoring another author's deletion** - add insertion after (don't modify their deletion):
```xml
<w:del w:author="Jane" w:id="5">
  <w:r><w:delText>deleted text</w:delText></w:r>
</w:del>
<w:ins w:author="Claude" w:id="10">
  <w:r><w:t>deleted text</w:t></w:r>
</w:ins>
```

### Comments

After running `comment.py` (see Step 2), add markers to document.xml. For replies, use `--parent` flag and nest markers inside the parent's.

**CRITICAL: `<w:commentRangeStart>` and `<w:commentRangeEnd>` are siblings of `<w:r>`, never inside `<w:r>`.**

```xml
<!-- Comment markers are direct children of w:p, never inside w:r -->
<w:commentRangeStart w:id="0"/>
<w:del w:id="1" w:author="Claude" w:date="2025-01-01T00:00:00Z">
  <w:r><w:delText>deleted</w:delText></w:r>
</w:del>
<w:r><w:t> more text</w:t></w:r>
<w:commentRangeEnd w:id="0"/>
<w:r><w:rPr><w:rStyle w:val="CommentReference"/></w:rPr><w:commentReference w:id="0"/></w:r>

<!-- Comment 0 with reply 1 nested inside -->
<w:commentRangeStart w:id="0"/>
  <w:commentRangeStart w:id="1"/>
  <w:r><w:t>text</w:t></w:r>
  <w:commentRangeEnd w:id="1"/>
<w:commentRangeEnd w:id="0"/>
<w:r><w:rPr><w:rStyle w:val="CommentReference"/></w:rPr><w:commentReference w:id="0"/></w:r>
<w:r><w:rPr><w:rStyle w:val="CommentReference"/></w:rPr><w:commentReference w:id="1"/></w:r>
```

### Images

1. Add image file to `word/media/`
2. Add relationship to `word/_rels/document.xml.rels`:
```xml
<Relationship Id="rId5" Type=".../image" Target="media/image1.png"/>
```
3. Add content type to `[Content_Types].xml`:
```xml
<Default Extension="png" ContentType="image/png"/>
```
4. Reference in document.xml:
```xml
<w:drawing>
  <wp:inline>
    <wp:extent cx="914400" cy="914400"/>  <!-- EMUs: 914400 = 1 inch -->
    <a:graphic>
      <a:graphicData uri=".../picture">
        <pic:pic>
          <pic:blipFill><a:blip r:embed="rId5"/></pic:blipFill>
        </pic:pic>
      </a:graphicData>
    </a:graphic>
  </wp:inline>
</w:drawing>
```

---

## Dependencies

- **pandoc**: Text extraction
- **docx**: `npm install -g docx` (new documents)
- **LibreOffice**: PDF conversion (auto-configured for sandboxed environments via `scripts/office/soffice.py`)
- **Poppler**: `pdftoppm` for images
