import os
import subprocess
import sys

def convert_md_to_pdf(input_file, output_file=None):
    """
    Wrapper to call the Node.js md-to-pdf converter.
    Requires Node.js and dependencies installed in ./md_to_pdf/
    """
    script_dir = os.path.dirname(os.path.abspath(__file__))
    tool_dir = os.path.join(script_dir, 'md_to_pdf')
    node_script = os.path.join(tool_dir, 'convert.js')
    
    if not os.path.exists(input_file):
        print(f"Error: Input file '{input_file}' not found.")
        return

    # Ensure dependencies are installed
    if not os.path.exists(os.path.join(tool_dir, 'node_modules')):
        print("Installing dependencies for md_to_pdf tool...")
        subprocess.check_call('npm install', shell=True, cwd=tool_dir)

    # Use the node script logic
    # Note: The node script currently hardcodes INPUT/OUTPUT. 
    # We should update the node script to accept args, but for now we'll set the file up.
    
    # Ideally we'd modify convert.js to read from CLI args.
    # For now, let's just run it and assume the user edits the file or we update the script.
    print(f"Running Node.js converter script located at {node_script}...")
    
    # Passing arguments to the node script would be better.
    # Let's assume we will update convert.js next to accept args.
    cmd = ['node', node_script, input_file]
    if output_file:
        cmd.append(output_file)
        
    try:
        subprocess.check_call(cmd, cwd=os.getcwd())
    except subprocess.CalledProcessError as e:
        print(f"Conversion failed: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python convert_md_to_pdf.py <input_md_file> [output_pdf_file]")
    else:
        input_f = sys.argv[1]
        output_f = sys.argv[2] if len(sys.argv) > 2 else None
        convert_md_to_pdf(input_f, output_f)
