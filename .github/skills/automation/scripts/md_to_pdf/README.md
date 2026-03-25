# Markdown to PDF Converter

Requires Node.js installed on the system.

## Usage

### Via Python (Recommended for Copilot)
```bash
python .github/skills/automation/scripts/convert_md_to_pdf.py <input.md> <output.pdf>
```

### Via Node.js directly
```bash
cd .github/skills/automation/scripts/md_to_pdf
npm install  # First time only
node convert.js <absolute_path_to_input.md> <absolute_path_to_output.pdf>
```

## Features
- Supports GitHub Flavored Markdown
- Supports Mermaid Diagrams (Flowchart, Sequence, Gantt, Class, etc.)
- Outputs A4 PDF with margins
