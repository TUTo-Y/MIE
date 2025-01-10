# 创建测试目标
add_executable(test
    getchar.c
)

# 测试目标
target_link_libraries(test PRIVATE mcore glfw cglm opengl32)