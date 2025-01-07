# 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和版本
project(MIE VERSION 1.0)

# 设置C语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

# 设置exe输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

# 包含external头文件路径
include_directories(${PROJECT_SOURCE_DIR}/external/stb) # stb
include_directories(${PROJECT_SOURCE_DIR}/external/glad/include) # glad
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include) # glfw

# 头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src/core/include)
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# 添加子目录
add_subdirectory(external/libarchive)
add_subdirectory(external/glfw)
add_subdirectory(src)
add_subdirectory(test)
