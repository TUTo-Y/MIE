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

// #define divide_by_2_floor(num) (((num) - ((num) < 0)) >> 1)
// #define divide_by_4_floor(num) (((num) - ((num) < 0 ? 3 : 0)) >> 2)

// #define divide_by_2_floor(num) (((num) - ((num) < 0 ? 0 : 0)) >> 1)
// #define divide_by_4_floor(num) (((num) - ((num) < 0 ? 0 : 0)) >> 2)

#define divide_by_2_floor(num) ((num) >> 1)
#define divide_by_4_floor(num) ((num) >> 2)

int main()
{
    for(int i = 0; i < 0x100; i++)
    {
        printf("i = %d &3 = %d\n", i, i & 3);
    }
    return 0;
}

#if 0
int main()
{
    /* 设置中文 */
    setlocale(LC_ALL, "zh_CN.UTF-8");

    DEBUG("测试除2\n");
    for (int i = ((-31) * 2) + ((-31) * 2); i <= ((31) * 2) + ((31) * 2); i++)
    {
        int out = divide_by_2_floor(i);
        if (i >= 0)
        {
            if (out == i / 2)
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tyes\n", i, divide_by_2_floor(i));
            }
            else
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tno\n", i, divide_by_2_floor(i));
            }
        }
        else
        {
            if (out == (i - 1) / 2)
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tyes\n", i, divide_by_2_floor(i));
            }
            else
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tno\n", i, divide_by_2_floor(i));
            }
        }
    }
    DEBUG("\n\n测试除4\n");
    for (int i = ((-31) * 4) + ((-31) * 4); i <= ((31) * 4) + ((31) * 4); i++)
    {
        int out = divide_by_4_floor(i);
        if (i >= 0)
        {
            if (out == i / 4)
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tyes\n", i, divide_by_4_floor(i));
            }
            else
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tno\n", i, divide_by_4_floor(i));
            }
        }
        else
        {
            if (out == (i - 3) / 4)
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tyes\n", i, divide_by_4_floor(i));
            }
            else
            {
                DEBUG("计算值 = %4d\tdHSB = %4d\tno\n", i, divide_by_4_floor(i));
            }
        }
    }
    return 0;
}
#endif

#if 0
int main(int argc, char *argv[], char *envp[])
{
    char *str = "HelloWOrld!\n123456789987978978986666666666";

    /* 设置中文 */
    setlocale(LC_ALL, "zh_CN.UTF-8");
    glfwInit();
    glfwGetTime();
    srand((unsigned int)time(NULL));

    int w, h, channels;
    stbi_uc *data;

    // data = stbi_load("girl.jpeg", &w, &h, &channels, 1);
    data = stbi_load("image.bmp", &w, &h, &channels, 1);

#if 1 // 切割
    uint8_t *data1, *data2;
    {
        double start = glfwGetTime();
        rdhSplitImage(data, w * h, &data1, &data2);
        printf("分割时间: %3.2f ms\n", (glfwGetTime() - start) * 1000);
    }
    stbi_write_bmp("data1.bmp", w, h, 1, data1);
    stbi_write_bmp("data2.bmp", w, h, 1, data2);
#endif

#if 1 // 嵌入
    uint8_t *data1Copy, *data2Copy;
    data1Copy = (uint8_t *)malloc(w * h);
    data2Copy = (uint8_t *)malloc(w * h);
    memcpy(data1Copy, data1, w * h);
    memcpy(data2Copy, data2, w * h);

    uint8_t *m;
    int mSize;
    char *out;
    rdhEmbedData(data1, data2, w, h, &m, &mSize, (const uint8_t *)str, strlen(str) + 1);
    rdhExtractData(data1, data2, w, h, m, mSize, (uint8_t **)&out);
    printf("out: %s\n", out);

    // 检查解码结果
    if(memcmp(data1, data1Copy, w * h) != 0 || memcmp(data2, data2Copy, w * h) != 0)
    {
        printf("图像与原数据不吻合\n");
    }
    printf("检查完毕\n");
#endif

#if 0 // 合并
    // uint8_t *data3;
    // {
    //     double start = glfwGetTime();
    //     rdhCombineImage(data1, data2, w * h, &data3);
    //     printf("合并时间: %3.2f ms\n", (glfwGetTime() - start) * 1000);
    // }
    // stbi_write_bmp("data3.bmp", w, h, 1, data3);
#endif
    stbi_image_free(data);
    return 0;
}
#endif

#if 0
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

    float vertex[] = {
        -(float)WINDOW_WIDTH / 2.0f, 0, 0.0f,
        (float)WINDOW_WIDTH / 2.0f, 0, 0.0f,
        0.0f, (float)WINDOW_HEIGHT / 2.0f, 0.0f,
        0.0f, -(float)WINDOW_HEIGHT / 2.0f, 0.0f
    };

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
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    // 投影操作
    glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 3000.0f, projection);
    // 设置视图矩阵
    float z = (WINDOW_HEIGHT / 2.0f) / tanf(glm_rad(45.0f) / 2.0f);
    glm_lookat((vec3){0.0f, 0.0f, z}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, view);
    // 旋转操作
    // glm_translate(model, (vec3){0.0f, 0.0f, -300.0f});
    // glm_rotate(model, (float)glfwGetTime()*1.5f, (vec3){1.0f, 0.0f, 0.0f});
    // glm_scale(model, (vec3){0.5f, 0.5f, 0.5f});

    // 设置
    guiShaderUse(programTemp);
    guiShaderUniformMatrix(programTemp, "model", 4fv, (float *)model);
    guiShaderUniformMatrix(programTemp, "view", 4fv, (float *)view);
    guiShaderUniformMatrix(programTemp, "projection", 4fv, (float *)projection);
    guiShaderUniform(programTemp, "color", 4f, 0.0f, 0.0f, 0.0f, 1.0f);
    // guiShaderUniform(programTemp, "Texture", 1i, 0);

    unsigned char *fontData;
    size_t size;
    resGetFile("kai.ttf", (void **)&fontData, &size, true);
    GUIttf *ttf = guiTTFCreate(fontData, 16, 80, 64, 0);


    GUIstr *s1 = guiStrCreate(ttf, 80, programFont, model, view, projection, (vec4){1.0f, 0.0f, 0.0f, 1.0f});
    guiStrCpy(s1, L"我");
    guiStrSetMod(s1, GUI_STR_MOD_RIGHT);
    
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
        
        glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
        guiStrRender(s1, model, false, false, true);
        // glm_translate(model, (vec3){0.0f, -100.0f, 0.0f});
        // guiStrRender(s2, model, false, false, true);
        // glm_translate(model, (vec3){0.0f, -100.0f, 0.0f});
        // guiStrRender(s3, model, false, false, true);

        // 渲染直角坐标系
        guiShaderUse(programTemp);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 4);

        // 控制帧率为30Hz
        guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }

    guiTTFDelete(ttf);
    glfwDestroyWindow(window);
    Quit();
#if 0
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
    {

        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        // 投影操作
        glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 3000.0f, projection);
        // 设置视图矩阵
        float z = (WINDOW_HEIGHT / 2.0f) / tanf(glm_rad(45.0f) / 2.0f) * 2.1;
        glm_lookat((vec3){0.0f, 0.0f, z}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, view);
        // 旋转操作
        // glm_translate(model, (vec3){0.0f, 0.0f, -300.0f});
        // glm_rotate(model, (float)glfwGetTime()*1.5f, (vec3){1.0f, 0.0f, 0.0f});

        // 设置
        guiShaderUse(programFont);
        guiShaderUniformMatrix(programFont, "model", 4fv, (float *)model);
        guiShaderUniformMatrix(programFont, "view", 4fv, (float *)view);
        guiShaderUniformMatrix(programFont, "projection", 4fv, (float *)projection);
        guiShaderUniform(programFont, "color", 4f, 0.0f, 0.0f, 0.0f, 1.0f);
        guiShaderUniform(programFont, "Texture", 1i, 0);

        guiShaderUse(programTemp);
        guiShaderUniformMatrix(programTemp, "model", 4fv, (float *)model);
        guiShaderUniformMatrix(programTemp, "view", 4fv, (float *)view);
        guiShaderUniformMatrix(programTemp, "projection", 4fv, (float *)projection);
        guiShaderUniform(programTemp, "color", 4f, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    // 字体
    unsigned char *fontData;
    size_t size;
    resGetFile("kai.ttf", (void **)&fontData, &size, true);

    // 初始化字体
    wchar_t text = L'我';
    stbtt_fontinfo font; // 字体信息
    stbtt_InitFont(&font, fontData, stbtt_GetFontOffsetForIndex(fontData, 0));

    // 获取字体高度
    int ascent = 0;  // 上升
    int descent = 0; // 下降
    int lineGap = 0; // 行间距
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    int high = ascent - descent + lineGap;

    // 获取字体宽度
    int advance, lsb;
    stbtt_GetCodepointHMetrics(&font, text, &advance, &lsb);

    /* 计算字体缩放 */
    float pixels = 300.0;                                   /* 字体大小（字号） */
    float scale = stbtt_ScaleForPixelHeight(&font, pixels); /* scale = pixels / (ascent - descent) */

    int w, h, x, y;
    // unsigned char *data = stbtt_GetCodepointBitmapSubpixel(&font, scale, scale, text, 0, 0, &w, &h, &x, &y);
    unsigned char *data = stbtt_GetCodepointBitmap(&font, scale, scale, text, &w, &h, &x, &y);
    ascent *= scale;
    descent *= scale;
    lineGap *= scale;
    advance *= scale;
    lsb *= scale;
    // printf("字体 顶部: %d, 底部: %d, 行距: %d\n", ascent, descent, lineGap);
    // printf("字体 宽度: %d, 左边距: %d\n", advance, lsb);
    // printf("位图 宽度: %d, 高度: %d, x: %d, y: %d\n", w, h, x, y);
    y = -y;

    // 保存位图
    stbi_write_bmp("f.bmp", w, h, 1, data);

    // 顶点
    float vertices[] = {
        // 位置             // 纹理坐标
        (float)(x), (float)(y), 0.0f, 0.0f,
        (float)(x + w), (float)(y), 1.0f, 0.0f,
        (float)(x + w), (float)(y - h), 1.0f, 1.0f,
        (float)(x), (float)(y - h), 0.0f, 1.0f};
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    // 创建纹理
    GLuint texture;
    texture = guiTextureCreate(data, w, h, 1, GL_RED);

    // line
    float line[] = {
        // 直角坐标系
        -(float)WINDOW_WIDTH, 0.0f,
        (float)WINDOW_WIDTH, 0.0f,
        0.0f, -(float)WINDOW_HEIGHT,
        0.0f, (float)WINDOW_HEIGHT,
        
        0, (float)ascent,
        (float)WINDOW_WIDTH, (float)ascent,
        
        0, (float)descent,
        (float)WINDOW_WIDTH, (float)descent,
        
        0, (float)(descent - lineGap),
        (float)WINDOW_WIDTH, (float)(descent - lineGap),

        0, (float)(y),
        (float)WINDOW_WIDTH, (float)(y),

        0, (float)(y - h),
        (float)WINDOW_WIDTH, (float)(y - h),

        (float)advance, (float)WINDOW_HEIGHT,
        (float)advance, -(float)WINDOW_HEIGHT

        // (float)lsb, (float)WINDOW_HEIGHT,
        // (float)lsb, -(float)WINDOW_HEIGHT,
        
        // (float)(x + w), (float)WINDOW_HEIGHT,
        // (float)(x + w), -(float)WINDOW_HEIGHT

        };
    GLuint VBOline, VAOline;
    glGenBuffers(1, &VBOline);
    glGenVertexArrays(1, &VAOline);
    glBindVertexArray(VAOline);
    glBindBuffer(GL_ARRAY_BUFFER, VBOline);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 初始化GUI界面
    GUIframe frame;
    guiFrameInit(&frame, 30);
    while (0)
    // while (!glfwWindowShouldClose(window))
    {
        // 事件处理
        glfwPollEvents();

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染
        guiShaderUse(programFont);

        // 渲染文本
        guiShaderUse(programFont);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 绘制线
        guiShaderUse(programTemp);
        glBindVertexArray(VAOline);
        glDrawArrays(GL_LINES, 0, sizeof(line) / sizeof(float) / 2);

        // 控制帧率为30Hz
        guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }
#endif
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
    return 0;
}

#endif