# 医学图像加密


## 环境

|模块|功能|
|:----:|:----:|
|stb|用于读取和保存图片，渲染字体|
|GmSSL|国密算法库|
|GLFW|窗口管理|
|cglm|矩阵计算|
|OpenGL|渲染界面, 基于OpenGL的核心渲染模式|
|glad|OpenGL的加载器生成器|

## 构建

### 更新子模块

```sh
git init
git submodule update --init --recursive
```

### GLAD

在[glad在线网站](https://glad.dav1d.de/)下载，选择OpenGL3.3的core模式

### 编译

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
