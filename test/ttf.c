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
    stbtt_fontinfo *mFontInfo = &font;
    unsigned char *fontData;
    size_t size;
    resGetFile("kai.ttf", (void **)&fontData, &size, true);
    stbtt_InitFont(&font, fontData, stbtt_GetFontOffsetForIndex(fontData, 0));

    // 渲染文本
    int ascent = 0;
    int descent = 0;
    int lineGap = 0;
    // float scale = stbtt_ScaleForPixelHeight(mFontInfo, 10);
    stbtt_GetFontVMetrics(mFontInfo, &ascent, &descent, &lineGap);
    int mLineHeight = ascent - descent + lineGap;
    DEBUG("ascent: %d, descent: %d, lineGap: %d, lineHeight: %d\n", ascent, descent, lineGap, mLineHeight);

    int advance = 0;
    int lsb = 0;
    stbtt_GetCodepointHMetrics(mFontInfo, L's', &advance, &lsb);
    DEBUG("advance: %d, lsb: %d\n", advance, lsb);

    stbtt_vertex *stbVertex = NULL;
    int verCount = 0;
    // wchar_t text[] = L"郑";
    // 获取 Unicode 代码点
    verCount = stbtt_GetCodepointShape(mFontInfo, L's', &stbVertex);
    printf("verCount: %d\n", verCount);
    int x, y;
    float point[10240][2] = {0};
    int count = 0;
    for (int i = 0; i < verCount; i++)
    {
        stbtt_vertex v = stbVertex[i];
        switch (v.type)
        {
        case STBTT_vmove:
            x = v.x;
            y = v.y;
            point[count][0] = x;
            point[count][1] = y;
            count++;
            break;
        case STBTT_vline:
            x = v.x;
            y = v.y;
            point[count][0] = x;
            point[count][1] = y;
            count++;
            break;
        case STBTT_vcurve: // 二次贝塞尔曲线
        {
            short x0 = x;
            short y0 = y;
            short x1 = v.cx;
            short y1 = v.cy;
            short x2 = v.x;
            short y2 = v.y;

            for (int j = 0; j <= 10; j++)
            {
                float t = (float)j / 10.0f;
                float xt = (1 - t) * (1 - t) * x0 + 2 * t * (1 - t) * x1 + t * t * x2;
                float yt = (1 - t) * (1 - t) * y0 + 2 * t * (1 - t) * y1 + t * t * y2;
                point[count][0] = xt;
                point[count][1] = yt;
                count++;
            }
            x = x2;
            y = y2;
            break;
        }
        case STBTT_vcubic: // 三次贝塞尔曲线
        {
            short x0_cubic = x;
            short y0_cubic = y;
            short x1_cubic = v.cx;
            short y1_cubic = v.cy;
            short x2_cubic = v.cx1;
            short y2_cubic = v.cy1;
            short x3_cubic = v.x;
            short y3_cubic = v.y;

            for (int j = 0; j <= 10; j++)
            {
                float t = (float)j / 10.0f;
                float xt = (1 - t) * (1 - t) * (1 - t) * x0_cubic + 3 * (1 - t) * (1 - t) * t * x1_cubic + 3 * (1 - t) * t * t * x2_cubic + t * t * t * x3_cubic;
                float yt = (1 - t) * (1 - t) * (1 - t) * y0_cubic + 3 * (1 - t) * (1 - t) * t * y1_cubic + 3 * (1 - t) * t * t * y2_cubic + t * t * t * y3_cubic;
                if (count < 10240)
                { // 确保不会超出数组边界
                    point[count][0] = xt;
                    point[count][1] = yt;
                    count++;
                }
                else
                {
                    // 处理错误：点数组已满
                    break;
                }
            }
            x = x3_cubic;
            y = y3_cubic;
            break;
        }
        }
    }
    printf("count: %d\n", count);
    for (int i = 0; i < count; i++)
    {
        point[i][0] /= 1000.0f;
        // point[i][0] -= 0.5f;
        point[i][1] /= 1000.0f;
        // point[i][1] -= 0.5f;
        // printf("%f, %f\n", point[i][0], point[i][1]);
    }
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                              // 绑定缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count * 2, point, GL_STATIC_DRAW); // 将顶点数据复制到缓冲区中
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    char *vertexShaderSource;
    char *fragmentShaderSource;
    resGetFile("vertex.glsl", (void **)&vertexShaderSource, NULL, true);
    resGetFile("fragment.glsl", (void **)&fragmentShaderSource, NULL, true);

    GLuint program = guiShaderCreateProgram(vertexShaderSource, fragmentShaderSource, NULL);
    guiShaderUniform(program, "color", 4f, 0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);
    // glPointSize(5.0f);

    // 初始化GUI界面
    GUIframe frame;
    guiFrameInit(&frame, 30);
    while (!glfwWindowShouldClose(window))
    {
        // 事件处理
        glfwPollEvents();

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染
        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 model = GLM_MAT4_IDENTITY_INIT;

        // 投影操作
        // glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 2000.0f, projection);

        // 设置视图矩阵
        // float z = (WINDOW_HEIGHT / 2.0f) / tanf(glm_rad(45.0f) / 2.0f) * 2.0f;
        // glm_lookat((vec3){0.0f, 0.0f, z}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, view);

        // 旋转操作
        // glm_translate(model, (vec3){0.0f, 0.0f, -300.0f});
        // glm_rotate(model, (float)glfwGetTime() * 1.5f, (vec3){1.0f, 0.0f, 0.0f});
        
        // 设置视图矩阵
        guiShaderUniformMatrix(program, "model", 4fv, (float *)model);
        guiShaderUniformMatrix(program, "view", 4fv, (float *)view);
        guiShaderUniformMatrix(program, "projection", 4fv, (float *)projection);

        // 渲染文本
        
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLE_FAN, 0, count);
        glDrawArrays(GL_LINE_STRIP, 0, count);
        // glDrawArrays(GL_LINE_STRIP, 0, ((int)glfwGetTime() * 25 + 2) % count);
        // glDrawArrays(GL_TRIANGLES, 0, count/3);

        // 控制帧率为30Hz
        guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);

#if 0
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
    // while (!glfwWindowShouldClose(window))
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
#endif
    Quit();
    return 0;
}
