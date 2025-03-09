# 包含external头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src/core)                 # core
include_directories(${PROJECT_SOURCE_DIR}/external/stb)             # stb
include_directories(${PROJECT_SOURCE_DIR}/external/glad/include)    # glad
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include)    # glfw
include_directories(${PROJECT_SOURCE_DIR}/external/cglm/include)    # cglm
include_directories(${PROJECT_SOURCE_DIR}/external/GmSSL/include)    # GmSSL

file(GLOB STB_SOURCES "${CMAKE_SOURCE_DIR}/src/stb/*.c")            # stb
file(GLOB GLAD_SOURCES "${CMAKE_SOURCE_DIR}/external/glad/src/*.c") # glad
file(GLOB CORE_FILES "${CMAKE_SOURCE_DIR}/src/core/*.c")            # core

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")                  # src


# 设置编译选项以启用调试信息
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 创建库目标
add_library(mcore STATIC
    ${STB_SOURCES}
    ${GLAD_SOURCES}
    ${CORE_FILES}
)

# 创建可执行文件目标
add_executable(MIE ${SRC_FILES})

# 链接库到可执行文件
target_link_libraries(MIE PRIVATE mcore glfw cglm opengl32 m pthread gmssl)

# # 复制资源文件
# file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resource/*")  # resource
# add_custom_command(
#     OUTPUT "${BUILD_OUTPUT_DIR}/resource"
#     BYPRODUCTS "${BUILD_OUTPUT_DIR}/resource"
#     COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_OUTPUT_DIR}/resource"
#     COMMAND ${CMAKE_COMMAND} -E make_directory "${BUILD_OUTPUT_DIR}/resource"
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     "${CMAKE_SOURCE_DIR}/resource"
#     "${BUILD_OUTPUT_DIR}/resource"
#     DEPENDS ${RESOURCE_FILES}
#     COMMENT "Copying resource files to ${BUILD_OUTPUT_DIR}/resource"
# )
# add_custom_target(copy_resources ALL DEPENDS "${BUILD_OUTPUT_DIR}/resource")
