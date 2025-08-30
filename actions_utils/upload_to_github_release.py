import argparse
import os
import glob

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Linux installer script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version

    os.system(r'gh release create ${{ github.ref_name }} --title "Released ${{ github.ref_name }}" --generate-notes')

    for filename in glob.glob("installers/*"):
        print(f"file: {filename}")
        if ".exe" in filename or ".deb" in filename or ".rpm" in filename or ".rpm" in filename or "tar.gz" in filename:
            os.system(r'gh release upload ${{ github.ref_name }} "' + filename + '" --clobber')