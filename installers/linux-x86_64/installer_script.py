import argparse
import os
import subprocess

def run_fpm(version, pkgtype, output):
    subprocess.run([
        "fpm",
        "-s", "dir",
        "-t", pkgtype,
        "-p", output,
        "--name", "cmakeg",
        "--license", "MIT",
        "--version", version,
        "--architecture", "x86_64",
        "--description", "Program to generate new cmake projects",
        "--url", "https://github.com/annasajkh/cmakeg",
        "--maintainer", "AnnasVirtual",
        "--after-install", "installers/linux-x86_64/install_permissions.sh",
        "--after-remove", "installers/linux-x86_64/uninstall.sh",
        "build/linux-x86_64/bin/cmakeg/release/=/opt/cmakeg/"
    ])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Linux installer script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version

    run_fpm(version, "deb", f"cmakeg-{version}-linux-x64-debian.deb")
    run_fpm(version, "rpm", f"cmakeg-{version}-linux-x64-fedora.rpm")
    run_fpm(version, "pacman", f"cmakeg-{version}-linux-x64-arch.pkg.tar.zst")