import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Upload to github script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version

    os.system(f'gh release create {version} --title "Released {version}" --generate-notes')

    search_dirs = ["installers", "linux-installers", "windows-installer"]

    for search_dir in search_dirs:
        if not os.path.exists(search_dir):
            continue

        for root, dirs, files in os.walk(search_dir):
            for file in files:
                file_path = os.path.join(root, file)

                if file.endswith((".exe", ".deb", ".rpm", ".pkg.tar.zst")):
                    print(f"uploading: {file} on {file_path}")
                    
                    os.system(f'gh release upload {version} "{file_path}" --clobber')