const fs = require('fs');
const path = require('path');
const puppeteer = require('puppeteer');
const MarkdownIt = require('markdown-it');

// 配置输入输出 (CLI Args)
const args = process.argv.slice(2);
const INPUT_FILE = args[0] || 'test_case.md';
const OUTPUT_FILE = args[1] || 'output.pdf';

if (!fs.existsSync(INPUT_FILE)) {
    console.error(`Error: File not found: ${INPUT_FILE}`);
    process.exit(1);
}

// 初始化 MarkdownIt
const md = new MarkdownIt({
    html: true,
    linkify: true,
    typographer: true
});

// 自定义渲染规则，将 mermaid 代码块转换为 div
const defaultFence = md.renderer.rules.fence;
md.renderer.rules.fence = (tokens, idx, options, env, self) => {
    const token = tokens[idx];
    const info = token.info.trim();
    
    if (info === 'mermaid') {
        return `<div class="mermaid">${token.content}</div>`;
    }
    
    // 其他语言代码块使用默认渲染
    return defaultFence(tokens, idx, options, env, self);
};

// CSS 样式 (模拟 GitHub Markdown 风格)
const cssStyles = `
<style>
    body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif;
        font-size: 16px;
        line-height: 1.5;
        word-wrap: break-word;
        padding: 40px;
        color: #24292e;
    }
    h1, h2, h3, h4, h5, h6 {
        margin-top: 24px;
        margin-bottom: 16px;
        font-weight: 600;
        line-height: 1.25;
    }
    h1 { font-size: 2em; border-bottom: 1px solid #eaecef; padding-bottom: 0.3em; }
    h2 { font-size: 1.5em; border-bottom: 1px solid #eaecef; padding-bottom: 0.3em; }
    pre {
        background-color: #f6f8fa;
        border-radius: 3px;
        font-size: 85%;
        line-height: 1.45;
        overflow: auto;
        padding: 16px;
    }
    code {
        background-color: rgba(27,31,35,.05);
        border-radius: 3px;
        font-size: 85%;
        margin: 0;
        padding: .2em .4em;
    }
    pre code {
        background-color: transparent;
        padding: 0;
    }
    blockquote {
        border-left: .25em solid #dfe2e5;
        color: #6a737d;
        padding: 0 1em;
        margin: 0;
    }
    table {
        border-collapse: collapse;
        width: 100%;
    }
    table th, table td {
        border: 1px solid #dfe2e5;
        padding: 6px 13px;
    }
    table tr:nth-child(2n) {
        background-color: #f6f8fa;
    }
    .mermaid {
        display: flex;
        justify-content: center;
        margin: 20px 0;
    }
</style>
`;

async function convert() {
    console.log(`正在读取文件: ${INPUT_FILE}...`);
    const markdown = fs.readFileSync(INPUT_FILE, 'utf-8');
    
    console.log('正在解析 Markdown...');
    const contentHtml = md.render(markdown);

    const fullHtml = `
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>Markdown to PDF</title>
        ${cssStyles}
        <!-- 引入 Mermaid -->
        <script src="https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js"></script>
        <script>
            mermaid.initialize({ 
                startOnLoad: true, 
                theme: 'default' 
            });
        </script>
    </head>
    <body>
        ${contentHtml}
    </body>
    </html>
    `;

    console.log('正在启动浏览器...');
    const browser = await puppeteer.launch({
        headless: "new"
    });
    const page = await browser.newPage();

    console.log('正在渲染页面...');
    // 使用 setContent 设置页面内容
    await page.setContent(fullHtml, { waitUntil: 'networkidle0' });

    // 等待 Mermaid 渲染完成 (简单等待)
    // 也可以通过检查 DOM 元素来判断，但 networkidle0 通常足够加载脚本
    // 强制等待一下以防万一
    await new Promise(r => setTimeout(r, 2000));

    console.log(`正在生成 PDF: ${OUTPUT_FILE}...`);
    await page.pdf({
        path: OUTPUT_FILE,
        format: 'A4',
        printBackground: true,
        margin: {
            top: '20mm',
            right: '20mm',
            bottom: '20mm',
            left: '20mm'
        }
    });

    await browser.close();
    console.log('转换完成！');
}

convert().catch(err => {
    console.error('转换失败:', err);
});
