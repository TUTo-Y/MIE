#include "gui.h"

#include <string.h>
#include <locale.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 输入缓冲区
char inputBuffer[256];
int inputLength = 0;

// 将 Unicode 码点转换为 UTF-8 字符串
void utf8_encode(unsigned int codepoint, char *buffer, int *length)
{
    if (codepoint <= 0x7F)
    {
        buffer[0] = (char)codepoint;
        *length = 1;
    }
    else if (codepoint <= 0x7FF)
    {
        buffer[0] = (char)(0xC0 | (codepoint >> 6));
        buffer[1] = (char)(0x80 | (codepoint & 0x3F));
        *length = 2;
    }
    else if (codepoint <= 0xFFFF)
    {
        buffer[0] = (char)(0xE0 | (codepoint >> 12));
        buffer[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        buffer[2] = (char)(0x80 | (codepoint & 0x3F));
        *length = 3;
    }
    else if (codepoint <= 0x10FFFF)
    {
        buffer[0] = (char)(0xF0 | (codepoint >> 18));
        buffer[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        buffer[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        buffer[3] = (char)(0x80 | (codepoint & 0x3F));
        *length = 4;
    }
}

// 字符输入回调函数
void char_callback(GLFWwindow *window, unsigned int codepoint)
{
    char utf8[4];
    int length;
    utf8_encode(codepoint, utf8, &length);

    if (inputLength + length < sizeof(inputBuffer) - 1)
    {
        memcpy(inputBuffer + inputLength, utf8, length);
        inputLength += length;
        inputBuffer[inputLength] = '\0';
    }
    printf("输入的文本: %s\n", inputBuffer);
}

// 键盘输入回调函数
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_BACKSPACE)
        {
            if (inputLength > 0)
            {
                // 删除最后一个 UTF-8 字符
                do
                {
                    inputLength--;
                } while (inputLength > 0 && (inputBuffer[inputLength] & 0xC0) == 0x80);
                inputBuffer[inputLength] = '\0';
            }
        }
        else if (key == GLFW_KEY_ENTER)
        {
            // 处理回车键按下事件
            inputLength = 0;
            inputBuffer[0] = '\0';
        }
        printf("输入的文本: %s\n", inputBuffer);
    }
}

int main()
{
    /* 设置中文 */
    setlocale(LC_ALL, "zh_CN.UTF-8");

    /* 初始化glfw */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "MIE - 医学图像加密测试系统", NULL, NULL);
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

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* 渲染 */

    // 设置字符输入回调函数
    glfwSetCharCallback(window, char_callback);

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, key_callback);

    // 初始化输入缓冲区
    memset(inputBuffer, 0, sizeof(inputBuffer));

    GUIframe frame;
    guiFrameInit(&frame, 30);

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        // 在控制台中打印输入的文本

        // 控制帧率为30Hz
        guiFrameControl(&frame);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}