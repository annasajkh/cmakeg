# Cmake Project Generator

## Building

### Prerequisite 
- Cmake version 3.20 or above
- Python 3

### Compiling
- Clone the repo `git clone https://github.com/annasajkh/cmakeg`
- For debug build do `python build.py` for release build do `python build.py --build-type release`

### Example Usage
NOTE: You have to add cmakeg to the path environment variable manually if you compile it from source<br><br>
Create a new workspace
```bash
cmakeg workspace --name TestWorkspace
```
Go to the workspace
```bash
cd TestWorkspace
```
Add a new executable project to the workspace with the name TestExecutableProject
```bash
cmakeg executable-project --name TestExecutableProject
```
Add a new static library project to the workspace with the name TestStaticLibrary
```bash
cmakeg static-library-project --name TestStaticLibrary
```
Reference TestStaticLibrary to the TestExecutableProject so you can use the library
```bash
cmakeg reference --library-source-name TestStaticLibrary --project-destination-name TestExecutableProject
```
Change the content of `main.cpp` in `TestWorkspace/TestExecutableProject/src/main.cpp` to be
```cpp
#include <iostream>
#include "TestStaticLibrary/Computer.hpp"

int main()
{
    std::cout << "The meaning of life is " << computer::getTheMeaningOfLife() << "\n";
    std::cin.get();
}
```
Compile everything (run this in the TestWorkspace directory)
```bash
python build.py --version 0.0.1
```
This is debug by default for release build use
```bash
python build.py --build-type release --version 0.0.1
```
The executable should be compiled to `TestWorkspace\build\your-os-and-arch\bin\TestExecutableProject`