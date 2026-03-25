import os
import argparse
from playwright.sync_api import sync_playwright

def html_to_pdf(input_path, output_path):
    # Ensure absolute paths
    input_file = os.path.abspath(input_path)
    output_file = os.path.abspath(output_path)

    if not os.path.exists(input_file):
        print(f"Error: Input file not found at {input_file}. Please check the path.")
        return

    # Ensure output directory exists
    output_dir = os.path.dirname(output_file)
    if not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)

    with sync_playwright() as p:
        print("Launching browser...")
        try:
            # Try bundling chromium first
            browser = p.chromium.launch()
        except:
            print("Bundled Chromium not found. Trying system Edge...")
            try:
                browser = p.chromium.launch(channel="msedge")
            except:
                print("System Edge not found. Trying system Chrome...")
                browser = p.chromium.launch(channel="chrome")
        
        # Suggestion 1: Explicit viewport setting
        # Setting a fixed desktop width ensures responsive Tailwind classes (md:, lg:) render as expected.
        viewport_width = 1280
        page = browser.new_page(viewport={"width": viewport_width, "height": 800})
        
        file_url = f"file:///{input_file.replace(os.sep, '/')}"
        
        # Suggestion 2: Better Tailwind/CSS handling
        # Force "screen" media type. By default, PDF generation uses "print" media,
        # which can cause elements to disappear or layouts to break.
        page.emulate_media(media="screen")
        
        print(f"Navigating to {file_url}...")
        
        # Suggestion 3: Robust waiting
        # 'networkidle' waits until there are no network connections for at least 500ms.
        # Changed to 'domcontentloaded' to avoid hanging on CDN issues
        page.goto(file_url, wait_until="domcontentloaded")
        
        # Additional robustness: Wait for fonts to load (critical for icons and layout stability)
        page.evaluate("document.fonts.ready")

        # Suggestion 4: Simulate scrolling to trigger lazy loading and animations
        print("Simulating scroll to trigger animations...")
        
        # Scroll logic: Scroll down slowly to the bottom of the page to ensure all elements are loaded
        page.evaluate("""async () => {
            await new Promise((resolve) => {
                let totalHeight = 0;
                const distance = 100;
                const timer = setInterval(() => {
                    const scrollHeight = document.body.scrollHeight;
                    window.scrollBy(0, distance);
                    totalHeight += distance;

                    if(totalHeight >= scrollHeight){
                        clearInterval(timer);
                        resolve();
                    }
                }, 50);
            });
        }""")
        
        # Wait for any final animations to settle
        page.wait_for_timeout(3000)
        
        # Suggestion 5: Better height calculation
        # document.body.scrollHeight alone can be misleading depending on CSS (e.g. overflow).
        # We calculate the max dimension to ensure we capture all content.
        dimensions = page.evaluate("""() => {
            const body = document.body;
            const html = document.documentElement;
            
            const fullHeight = Math.max(
                body.scrollHeight, 
                body.offsetHeight, 
                html.clientHeight, 
                html.scrollHeight, 
                html.offsetHeight
            );
            
            return { 
                width: html.scrollWidth, 
                height: fullHeight 
            };
        }""")
        
        content_height = dimensions['height'] + 10  # Add small buffer to avoid clipping
        print(f"Calculated dimensions: width={viewport_width}px, height={content_height}px")
        
        # Resize viewport to match content height before printing
        page.set_viewport_size({"width": viewport_width, "height": int(content_height)})
        
        print(f"Generating PDF to {output_file}...")
        page.pdf(
            path=output_file,
            # Set PDF page size to match content exactly for a "Single Page" result
            width=f"{viewport_width}px",
            height=f"{content_height}px",
            print_background=True,
            page_ranges="1",
            # Suggestion 5: Margins
            # Zero margins are usually best for single-page web captures to avoid white borders
            margin={"top": "0px", "bottom": "0px", "left": "0px", "right": "0px"}
        )
        
        browser.close()
        print(f"PDF generation complete: {output_file}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert an HTML file to a single-page PDF.")
    parser.add_argument("input", help="Path to the input HTML file")
    parser.add_argument("output", nargs="?", help="Path to the output PDF file (optional)")
    
    args = parser.parse_args()
    
    input_path = args.input
    output_path = args.output
    
    # Generate default output path if not provided
    if not output_path:
        base, _ = os.path.splitext(input_path)
        output_path = base + ".pdf"
        
    html_to_pdf(input_path, output_path)
