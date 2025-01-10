file(GLOB GLAD_SOURCES "${CMAKE_SOURCE_DIR}/external/glad/src/*.c") # glad

file(GLOB CORE_FILES "${CMAKE_SOURCE_DIR}/src/core/*.c")            # core
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")                  # src
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resource/*")  # resource

# 设置编译选项以启用调试信息
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 创建库目标
add_library(mcore STATIC
    ${GLAD_SOURCES}
    ${CORE_FILES}
    ${SRC_FILES}
)

# 创建可执行文件目标
add_executable(MIE main.c)

# 链接库到可执行文件
target_link_libraries(MIE PRIVATE mcore glfw cglm opengl32)

# 复制资源文件
add_custom_command(
    OUTPUT "${BUILD_OUTPUT_DIR}/resource"
    BYPRODUCTS "${BUILD_OUTPUT_DIR}/resource"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_OUTPUT_DIR}/resource"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BUILD_OUTPUT_DIR}/resource"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resource"
    "${BUILD_OUTPUT_DIR}/resource"
    DEPENDS ${RESOURCE_FILES}
    COMMENT "Copying resource files to ${BUILD_OUTPUT_DIR}/resource"
)

add_custom_target(copy_resources ALL DEPENDS "${BUILD_OUTPUT_DIR}/resource")
