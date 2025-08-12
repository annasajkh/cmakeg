# My c++ cmake template

### Compiling Debug
```shell
mkdir build-win-x64 or mkdir build-linux-x64 or mkdir build-osx-x64
cd build-win-x64 or mkdir build-linux-x64 or mkdir build-osx-x64
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --parallel --config Debug
```

### Compiling Release
```shell
mkdir build-win-x64 or mkdir build-linux-x64 or mkdir build-osx-x64
cd build-win-x64 or mkdir build-linux-x64 or mkdir build-osx-x64
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel --config Release
```