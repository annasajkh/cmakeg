import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Upload to github script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version

    os.system(f'gh release create {version} --title "Released {version}" --generate-notes')

    for r, d, file_paths in os.walk("installers/"):
        for file_path in file_paths:
            print(f"file: {file_path}")

            if ".exe" in file_path or ".deb" in file_path or ".rpm" in file_path or ".rpm" in file_path or ".pkg.tar.zst" in file_path or "tar.gz" in file_path:
                os.system(f'gh release upload {version} "' + file_path + '" --clobber')