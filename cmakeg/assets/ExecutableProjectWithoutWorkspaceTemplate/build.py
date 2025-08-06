import os
import platform
import subprocess
import argparse

from enum import Enum

def build_project(build_type: str):
    system = platform.system()
    machine = platform.machine()
    
    if not os.path.exists("build"):
        os.mkdir("build")
    
    os.chdir("build")
    
    if not os.path.exists(build_type):
        os.mkdir(build_type)
    
    os.chdir(build_type)
    
    build_dir = f"{system.lower()}-{machine.lower()}"
    
    if not os.path.exists(build_dir):
        os.mkdir(build_dir)
    
    os.chdir(build_dir)
    
    cmake_generate_cmd = ["cmake", f"-DCMAKE_BUILD_TYPE={build_type.title()}", "../../.."]
    
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
        build_project(args.build_type)