import argparse
import os
import glob

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Upload to github script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version

    os.system(f'gh release create {version} --title "Released {version}" --generate-notes')

    for filename in glob.glob("installers/*"):
        print(f"file: {filename}")
        
        if ".exe" in filename or ".deb" in filename or ".rpm" in filename or ".rpm" in filename or ".pkg.tar.zst" in filename or "tar.gz" in filename:
            os.system(f'gh release upload {version} "' + filename + '" --clobber')