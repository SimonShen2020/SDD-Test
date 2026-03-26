import argparse
import json
import os

def generate_layout(rows, cols, vendor, output_file):
    cameras = []
    for r in range(rows):
        for c in range(cols):
            # 新架构使用相机名称作为唯一标识，不再使用串号
            name = f"cam{r}{c}"
            cameras.append({
                "name": name,
                "row": r,
                "col": c
            })
            
    config = {
        "layout": {
            "rows": rows,
            "cols": cols
        },
        "vendor": vendor,
        "cameras": cameras
    }
    
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(config, f, indent=4)
        
    print(f"✅ Successfully generated camera layout configuration to {output_file}")
    print(f"  - Layout: {rows}x{cols}")
    print(f"  - Vendor: {vendor}")
    print(f"  - Total cameras: {rows*cols}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a JSON configuration file for M x N camera layout.")
    parser.add_argument("-r", "--rows", type=int, required=True, help="Number of rows in the layout")
    parser.add_argument("-c", "--cols", type=int, required=True, help="Number of columns in the layout")
    parser.add_argument("-v", "--vendor", type=str, default="Null", help="Camera plugin vendor name (e.g. Null, Huarui)")
    parser.add_argument("-o", "--output", type=str, default="config.json", help="Output JSON file path")
    
    args = parser.parse_args()
    
    generate_layout(args.rows, args.cols, args.vendor, args.output)
