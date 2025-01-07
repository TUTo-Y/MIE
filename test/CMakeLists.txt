# 创建测试目标
add_executable(test
    ${GLAD_SOURCES}
    ${STB_SOURCES}
    getchar.c
)

# 测试目标
target_link_libraries(test PRIVATE mcore glfw)