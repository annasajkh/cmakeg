import argparse
import os

version = "0.0.2"

os.system(f'gh release create {version} --title "Released {version}" --generate-notes')

search_dirs = ["linux-installers", "windows-installer"]

for search_dir in search_dirs:
    for root, dirs, files in os.walk(search_dir):
        for file in files:
            file_path = os.path.join(root, file)

            if file.endswith((".exe", ".deb", ".rpm", ".pkg.tar.zst")):
                print(f"uploading: {file} on {file_path}")

                os.system(f'gh release upload {version} "{file_path}" --clobber')