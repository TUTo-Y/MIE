file(GLOB GLAD_SOURCES "${CMAKE_SOURCE_DIR}/external/glad/src/*.c") # glad

file(GLOB CORE_FILES "${CMAKE_SOURCE_DIR}/src/core/*.c")
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")

# 创建库目标
add_library(mcore STATIC
    ${GLAD_SOURCES}
    ${CORE_FILES}
    ${SRC_FILES}
)

# 创建可执行文件目标
add_executable(MIE main.c)

# 链接库到可执行文件
target_link_libraries(MIE PRIVATE mcore glfw opengl32 archive)
