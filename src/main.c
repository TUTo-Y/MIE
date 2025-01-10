#include "gui.h"
#include "resource.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include "config.h"

#include <cglm/cglm.h>

#include <math.h>
#include <string.h>
#include <locale.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Init()
{
    /* 设置中文 */
    setlocale(LC_ALL, "zh_CN.UTF-8");

    /* 初始化资源 */
    resInit();

    /* 初始化glfw */
    glfwInit();
    glfwGetError(NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 设置图像翻转
    stbi_set_flip_vertically_on_load(true);
    stbi_flip_vertically_on_write(true);
}

void Quit()
{
    /* 释放资源 */
    resQuit();

    /* 释放glfw */
    glfwTerminate();
}

int main()
{
    Init();

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MIE - 医学图像加密测试系统", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* 初始化glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // 设置回调函数
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 字体
    stbtt_fontinfo font;
    unsigned char *fontData;
    size_t size;
    resGetFile("kai.ttf", (void **)&fontData, &size, true);
    if (!stbtt_InitFont(&font, fontData, stbtt_GetFontOffsetForIndex(fontData, 0)))
    {
        ERROR("失败\n");
    }
    else
    {
        DEBUG("成功");
    }

    // 渲染文本
    unsigned char *bitmap;
    int width, height;
    // bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, 20), 20, 20, 0, 0);
    stbi_write_png("text.png", 20, 20, 1, bitmap, 20);
    stbtt_FreeBitmap(bitmap, NULL);

    // 渲染
    // float vertices[] = {
    //     // 位置             // 纹理坐标
    //     -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    //     1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    //     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    //     -1.0f, -1.0f, 0.0f, 0.0f, 0.0f};
    float vertices[] = {
        // 位置             // 纹理坐标
        -((float)WINDOW_WIDTH / 2.0f), ((float)WINDOW_HEIGHT / 2.0f), 0.0f, 0.0f, 1.0f,
        ((float)WINDOW_WIDTH / 2.0f), ((float)WINDOW_HEIGHT / 2.0f), 0.0f, 1.0f, 1.0f,
        ((float)WINDOW_WIDTH / 2.0f), -((float)WINDOW_HEIGHT / 2.0f), 0.0f, 1.0f, 0.0f,
        -((float)WINDOW_WIDTH / 2.0f), -((float)WINDOW_HEIGHT / 2.0f), 0.0f, 0.0f, 0.0f};
    GLuint index[] = {
        0, 1, 2,
        0, 2, 3};
    GLuint VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        // 绑定缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 将顶点数据复制到缓冲区中
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    char *vertexShaderSource;
    char *fragmentShaderSource;
    resGetFile("vertex.glsl", (void **)&vertexShaderSource, NULL, true);
    resGetFile("fragment.glsl", (void **)&fragmentShaderSource, NULL, true);

    GLuint program = guiShaderCreateProgram(vertexShaderSource, fragmentShaderSource, NULL);
    guiShaderUniform(program, "color", 4f, 1.0f, 1.0f, 1.0f, 1.0f);
    guiShaderUniform(program, "ourTexture", 1i, 0);

    // 纹理
    // int width, height, nrChannels;
    // unsigned char *image;
    // unsigned char *data;
    // size_t size;
    // resGetFile("image.jpeg", (void **)&data, &size, false);
    // image = stbi_load_from_memory(data, size, &width, &height, &nrChannels, 0);
    // GLuint texture = guiTextureCreate(image, width, height, nrChannels, GL_RGBA);
    // stbi_image_free(image);

    // 初始化GUI界面
    GUIframe frame;
    guiFrameInit(&frame, 30);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // 事件处理
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染
        mat4 ortho = GLM_MAT4_IDENTITY_INIT;
        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 model = GLM_MAT4_IDENTITY_INIT;

        // 投影操作
        glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 2000.0f, projection);

        // 设置视图矩阵
        float z = (WINDOW_HEIGHT / 2.0f) / tanf(glm_rad(45.0f) / 2.0f);
        glm_lookat((vec3){0.0f, 0.0f, z}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, view);

        // 旋转操作
        // glm_translate(model, (vec3){0.0f, 0.0f, -300.0f});
        glm_rotate(model, (float)glfwGetTime() * 1.5f, (vec3){1.0f, 0.0f, 0.0f});

        // 使用着色器程序
        guiShaderUse(program);

        // 设置模型矩阵的uniform
        guiShaderUniformMatrix(program, "model", 4fv, (float *)model);
        guiShaderUniformMatrix(program, "view", 4fv, (float *)view);
        guiShaderUniformMatrix(program, "projection", 4fv, (float *)projection);
        guiShaderUniformMatrix(program, "ortho", 4fv, (float *)ortho);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 控制帧率为30Hz
        guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }
    return 0;

    Quit();
}
