#include "gui.h"
#include "resource.h"
#include "config.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"

#include "rand.h"
#include "RDH.h"

#include <math.h>
#include <string.h>
#include <locale.h>

#if 1

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

    // 禁用窗口大小调节
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 设置窗口提示，隐藏窗口边框
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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

void callback(GLFWwindow *window, unsigned int codepoint)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
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

    // 获取主显示器的尺寸
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xpos = (mode->width - WINDOW_WIDTH) / 2;
    int ypos = (mode->height - WINDOW_HEIGHT) / 2;

    // 设置窗口位置到屏幕中心
    glfwSetWindowPos(window, xpos, ypos);

    glfwMakeContextCurrent(window);

    /* 初始化glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetCharCallback(window, callback);

    float vertex[] = {
        -(float)WINDOW_WIDTH / 2.0f, 0, 0.0f,
        (float)WINDOW_WIDTH / 2.0f, 0, 0.0f,
        0.0f, (float)WINDOW_HEIGHT / 2.0f, 0.0f,
        0.0f, -(float)WINDOW_HEIGHT / 2.0f, 0.0f};

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 创建着色器
    char *vertexShaderSource;
    char *fragmentShaderSource;

    resGetFile("font.vert", (void **)&vertexShaderSource, NULL, true);
    resGetFile("font.frag", (void **)&fragmentShaderSource, NULL, true);
    GLuint programFont = guiShaderCreateProgram(vertexShaderSource, fragmentShaderSource, NULL);
    resGetFile("vertex.glsl", (void **)&vertexShaderSource, NULL, true);
    resGetFile("fragment.glsl", (void **)&fragmentShaderSource, NULL, true);
    GLuint programTemp = guiShaderCreateProgram(vertexShaderSource, fragmentShaderSource, NULL);

    // 设置着色器值
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 PV = GLM_MAT4_IDENTITY_INIT;
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    // 投影操作
    glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 3000.0f, projection);
    // 设置视图矩阵
    float z = (WINDOW_HEIGHT / 2.0f) / tanf(glm_rad(45.0f) / 2.0f);
    glm_lookat((vec3){0.0f, 0.0f, z}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, view);
    // 复合
    glm_mat4_mul(projection, view, PV);

    // 设置
    guiShaderUse(programTemp);
    guiShaderUniformMatrix(programTemp, "model", 4fv, (float *)model);
    guiShaderUniformMatrix(programTemp, "view", 4fv, (float *)view);
    guiShaderUniformMatrix(programTemp, "projection", 4fv, (float *)projection);
    guiShaderUniform(programTemp, "color", 4f, 0.0f, 0.0f, 0.0f, 1.0f);

    // 字体文件
    unsigned char *fontData;
    size_t size;
    resGetFile("kai.ttf", (void **)&fontData, &size, true);
    GUIttf *ttf = guiTTFCreate(fontData, 16, 80, 64, 0);

    guiStrInitPV(PV);

    GUIstr *s1 = guiStrCreate(ttf, 100, GUI_STR_MOD_CENTER, programFont, NULL, (vec4){1.0f, 0.0f, 0.0f, 1.0f});
    guiStrCpy(s1, L"我是一个字符串");

    // GUIstr *s2 = guiStrCreate(ttf, 80, GUI_STR_MOD_LEFT_TOP, programFont, NULL, (vec4){0.0f, 1.0f, 0.0f, 1.0f});
    // guiStrCpy(s2, L"郑德泓");


    // 初始化GUI界面
    GUIframe frame;
    guiFrameInit(&frame, 30);
    while (!glfwWindowShouldClose(window))
    {
        // 事件处理
        glfwPollEvents();

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染文本
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        // glm_translate(model, (vec3){100.0f, 0.0f, 0.0f});
        glm_rotate(model, (float)glfwGetTime() / 3, (vec4){0.5f, 1.0f, 0.0f});
        guiStrSetModel(s1, model);
        guiStrRender(s1);

        
        // glm_mat4_identity(model);
        // glm_translate(model, (vec3){-WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, 0.0f});
        // // glm_translate(model, (vec3){100.0f, 0.0f, 0.0f});
        // guiStrSetModel(s2, model);
        // guiStrRender(s2);

        // 渲染直角坐标系
        guiShaderUse(programTemp);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 4);

        // 控制帧率为30Hz
        guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }

    // guiTTFDelete(ttf);
    glfwDestroyWindow(window);
    Quit();
    return 0;
}

#endif