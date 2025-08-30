import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Linux installer script")
    parser.add_argument("--version", type=str, help="the version of the project")        
    
    args = parser.parse_args()
    version = args.version
    
    os.system(f'fpm -s dir -t deb \
   -p cmakeg-{version}-linux-x64-debian.deb \
   --name cmakeg \
   --license MIT \
   --version {version} \
   --architecture x86_64 \
   --description "Program to generate new cmake projects" \
   --url "https://github.com/annasajkh/cmakeg" \
   --maintainer "AnnasVirtual" \
   --after-install install_permissions.sh \
   --after-remove <(echo "rm -rf /opt/cmakeg && rm -f /usr/bin/cmakeg") \
   ../../build/linux-x86_64/bin/cmakeg/release=/opt/cmakeg')
    
    os.system(f'fpm -s dir -t rpm \
   -p cmakeg-{version}-linux-x64-fedora.rpm \
   --name cmakeg \
   --license MIT \
   --version {version} \
   --architecture x86_64 \
   --description "Program to generate new cmake projects" \
   --url "https://github.com/annasajkh/cmakeg" \
   --maintainer "AnnasVirtual" \
   --after-install install_permissions.sh \
   --after-remove <(echo "rm -rf /opt/cmakeg && rm -f /usr/bin/cmakeg") \
   ../../build/linux-x86_64/bin/cmakeg/release=/opt/cmakeg')
    
    os.system(f'fpm -s dir -t pacman \
   -p cmakeg-{version}-linux-x64-arch.pkg.tar.zst \
   --name cmakeg \
   --license MIT \
   --version {version} \
   --architecture x86_64 \
   --description "Program to generate new cmake projects" \
   --url "https://github.com/annasajkh/cmakeg" \
   --maintainer "AnnasVirtual" \
   --after-install install_permissions.sh \
   --after-remove <(echo "rm -rf /opt/cmakeg && rm -f /usr/bin/cmakeg") \
   ../../build/linux-x86_64/bin/cmakeg/release=/opt/cmakeg')