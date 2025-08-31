import os
import platform
import subprocess
import argparse
from typing import List

def replace_version(file_path: str, line_to_replace_clues: List[tuple[str, str]]):
    file = open(file_path)
    file_content = file.read()
    file.close()

    file_content_modified = []

    for line in file_content.split("\n"):
        modify_line = False

        for line_to_replace_clue in line_to_replace_clues:
            if line_to_replace_clue[0] in line:
                modify_line = True
                file_content_modified.append(line_to_replace_clue[1])

        if not modify_line:
            file_content_modified.append(line)

    file = open(file_path, "w")
    file.write("\n".join(file_content_modified))
    file.close()

def build_project(build_type: str):
    os.chdir("cmakeg/dependencies/")

    if not os.path.exists("CLI11"):
        os.system("git clone https://github.com/CLIUtils/CLI11")
        os.chdir("CLI11")
        os.system("git checkout v2.5.0")
        os.chdir("..")
    
    if not os.path.exists("boost"):
        os.system("git clone https://github.com/boostorg/boost")
        os.chdir("boost")
        os.system("git checkout boost-1.88.0")
        os.chdir("..")
    
    if len(os.listdir("boost/libs/algorithm")) == 0:
        os.chdir("boost")
        
        os.system("git submodule update --depth 1 -q --init tools/boostdep")
        
        os.system("git submodule update --depth 1 -q --init libs/dll")
        os.system("python tools/boostdep/depinst/depinst.py -X test -g \"--depth 1\" dll")
        
        os.system("git submodule update --depth 1 -q --init libs/algorithm")
        os.system("python tools/boostdep/depinst/depinst.py -X test -g \"--depth 1\" algorithm")
        
        os.chdir("..")
    
    os.chdir("../../")
    
    system = platform.system()
    machine = platform.machine()
    
    if not os.path.exists("build"):
        os.mkdir("build")
    
    os.chdir("build")
    
    build_dir = f"{system.lower()}-{machine.lower()}"
    
    if not os.path.exists(build_dir):
        os.mkdir(build_dir)
    
    os.chdir(build_dir)
    
    cmake_generate_cmd = ["cmake", f"-DCMAKE_BUILD_TYPE={build_type.title()}", "../../"]
    
    if system == "Windows":
        cmake_build_cmd = ["cmake", "--build", ".", "--config", f"{build_type.title()}", "--", "/m"]
    else:
        cmake_build_cmd = ["cmake", "--build", ".", "--config", f"{build_type.title()}", "--parallel"]
    
    try:
        subprocess.run(cmake_generate_cmd, check=True)
        subprocess.run(cmake_build_cmd, check=True)
        
        print("Build completed successfully!")
    except subprocess.CalledProcessError as exception:
        print(f"Build failed with error: {exception}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Cmake builder script")
    parser.add_argument("--build-type", type=str, help="The build type either debug or release", default="debug")

    args = parser.parse_args()
    
    if not args.build_type in ["debug", "release"]:
        print("Error: --build-type has to be either debug or release")
    else:
        version_file = open("version.txt")
        version = version_file.read().strip()
        version_file.close()
        
        print(f"Building with version: {version}")
        
        replace_version("installers/windows-amd64/installer_script.iss", [("#define MyAppVersion", f"#define MyAppVersion \"{version}\""), ("OutputBaseFilename", f"OutputBaseFilename=cmakeg-{version}-win-x64-setup")])
        replace_version("actions_utils/upload_to_github_release.py", [("version = ", f"version = \"{version}\"")])
        replace_version("installers/linux-x86_64/installer_script.py", [("version = ", f"version = \"{version}\"")])
        replace_version("cmakeg/src/main.cpp", [("std::string version = ", f"    std::string version = \"{version}\";")])
        replace_version("CMakeLists.txt", [("project(${PROJECT_NAME} VERSION", "project(${PROJECT_NAME} VERSION " + version + " LANGUAGES CXX)")])

        build_project(args.build_type)