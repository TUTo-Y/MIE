# 医学图像加密

## 编译

更新子模块

```sh
git init
git submodule add git@github.com:glfw/glfw.git external/glfw
git submodule add git@github.com:nothings/stb.git external/stb
git submodule update --init --recursive
```

使用`MinGW`编译 (推荐)

```sh
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```

使用`nmake`编译

```sh
mkdir build
cd build
cmake -G "NMake Makefiles" ..
nmake
```

使用`Visual Studio`编译

```sh
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```