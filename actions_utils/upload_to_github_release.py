import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Upload to github script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version

    os.system(f'gh release create {version} --title "Released {version}" --generate-notes')

    for root, directories, files in os.walk("installers"):
        for file in files:
            file_path = os.path.join(root, file)

            if ".exe" in file or ".deb" in file or ".rpm" in file or ".rpm" in file or ".pkg.tar.zst" in file:
                print(f"uploading: {file} on {file_path}")
                os.system(f'gh release upload {version} "' + file_path + '" --clobber')