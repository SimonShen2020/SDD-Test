import urllib.request
import urllib.error
import ssl

# Ignore SSL errors
# This is required for some corporate networks or when certificates are missing locally
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

BASE_URL = "https://github.com/RapidAI/RapidOCR/releases/download"
VERSIONS = ["v1.1.0", "v1.2.0", "v1.2.1", "v1.3.0", "v1.4.0", "v2.0.0", "v2.1.0", "v3.0.0"]
FILES = ["ch_PP-OCRv3_rec_infer.onnx", "ch_PP-OCRv3_det_infer.onnx"]

def check_url(url):
    try:
        req = urllib.request.Request(url, method='HEAD')
        with urllib.request.urlopen(req, context=ctx, timeout=5) as response:
            if response.status == 200 or response.status == 302:
                print(f"[OK] {url}")
                return True
            else:
                print(f"[{response.status}] {url}")
    except urllib.error.HTTPError as e:
        # 404 is expected for many combinations, so maybe don't print unless verbose?
        # But user script printed everything. Let's print non-404 errors or successful ones.
        if e.code == 404:
            # print(f"[404] {url}")
            pass
        else:
            print(f"[{e.code}] {url}")
    except Exception as e:
        print(f"[Error] {url}: {e}")
    return False

def main():
    print("Checking GitHub Releases...")
    found_count = 0
    for ver in VERSIONS:
        for f in FILES:
            url = f"{BASE_URL}/{ver}/{f}"
            if check_url(url):
                found_count += 1
    
    print(f"\nFound {found_count} valid links on GitHub.")

    print("\nChecking ModelScope Legacy...")
    MS_base = "https://www.modelscope.cn/models/RapidAI/RapidOCR/resolve"
    # specific known potential paths
    ms_urls = [
        f"{MS_base}/master/resources/models/ch_PP-OCRv3_rec_infer.onnx",
        f"{MS_base}/v1.1.0/resources/models/ch_PP-OCRv3_rec_infer.onnx",
    ]
    for url in ms_urls:
        check_url(url)

if __name__ == "__main__":
    main()
