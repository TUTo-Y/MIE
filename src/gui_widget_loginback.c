#include "gui_widget_loginback.h"

#define SCALE 1.02f // 边框大小
#define LOGINBACK2SCALE 0.7f
#define LOGINBACK2WIDTH (((float)WINDOW_WIDTH) * LOGINBACK2SCALE)
#define LOGINBACK2HEIGHT (((float)WINDOW_HEIGHT) * LOGINBACK2SCALE)
#define LOGINBACK2DISW (((float)WINDOW_WIDTH) * (1.0f - LOGINBACK2SCALE) / 2.0f)
#define LOGINBACK2DISH (((float)WINDOW_HEIGHT) * (1.0f - LOGINBACK2SCALE) / 2.0f)

typedef struct gui_widget_loginback_struct
{
    // 基础数据
    GLuint loginbackTex;   // 背景纹理
    GLuint loginbackr2Tex; // 圆角背景纹理
    int imageW, imageH;    // 图像大小
    float disW, disH;      // 图像边距

    // 渲染数据
    GUIdrawr loginback;    // 背景
    GUIdrawrr loginbackr2; // 圆角背景

    // 物理数据
    double movA;      // 移动加速度
    int movX, movY;   // 当前位置
    int movDX, movDY; // 要移动的目标
    double animTime;  // 动画时间
} gui_widget_loginback_struct;

void gui_widget_loginback_StartCall(GUIwidget *widget)
{
    // 分配内存
    widget->data = malloc(sizeof(gui_widget_loginback_struct));
    memset(widget->data, 0, sizeof(gui_widget_loginback_struct));
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 加载图像
    int width, height, nrChannels;
    unsigned char *data = stbi_load(config.loginback_path, &width, &height, &nrChannels, 3);

    // 调整大小
    float scale = ((float)width / (float)WINDOW_WIDTH > (float)height / (float)WINDOW_HEIGHT) ? WINDOW_HEIGHT * SCALE / (float)height : WINDOW_WIDTH * SCALE / (float)width;
    st->imageW = width * scale;
    st->imageH = height * scale;
    st->disW = fabsf((WINDOW_WIDTH - st->imageW) / 2.0f);
    st->disH = fabsf((WINDOW_HEIGHT - st->imageH) / 2.0f);
    unsigned char *data2 = (unsigned char *)malloc(st->imageW * st->imageH * 3);
     
    if (!(stbir_resize_uint8_linear(data, width, height, 0,
                                  data2, st->imageW, st->imageH, 0, STBIR_RGB)))
    ERROR("调整大小错误\n");

    // 创建纹理
    st->loginbackTex = guiTextureCreate(data2, st->imageW, st->imageH, 3, GL_RGB);
    st->loginbackr2Tex = guiDrawGaussian(st->loginbackTex, (vec4){0, 0, st->imageW, st->imageH}, 3.0f);

    // 释放图像数据
    stbi_image_free(data);
    stbi_image_free(data2);
}
void gui_widget_loginback_DestroyCall(GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 释放纹理
    glDeleteTextures(1, &st->loginbackTex);
    glDeleteTextures(1, &st->loginbackr2Tex);

    // 释放内存
    free(widget->data);
}

void gui_widget_loginback_init(GUIwin *win, GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 生成渲染器
    st->loginback = guiDrawRTCreate(st->imageW, st->imageH);
    st->loginbackr2 = guiDrawRRTCreate(LOGINBACK2WIDTH, LOGINBACK2HEIGHT, 30.0f, GUI_DRAW_DEFAULT_VAGUE);

    // 绑定纹理
    guiDrawRTBindTexture(st->loginback, st->loginbackTex);
    guiDrawRRTBindTexture(st->loginbackr2, st->loginbackr2Tex);

    // 设置模型矩阵根据loginback
    float mox = 0.0f, moy = 0.0f;
    guiDrawRRTCutTexture(st->loginbackr2,
                         st->disW + LOGINBACK2DISW,
                         st->disH + LOGINBACK2DISH,

                         LOGINBACK2WIDTH, LOGINBACK2HEIGHT);
}

void gui_widget_loginback_destroy(GUIwin *win, GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 释放渲染器
    guiDrawRTDestroy(st->loginback);
    guiDrawRRTDestroy(st->loginbackr2);
}

void gui_widget_loginback_msg(GUIwin *win, GUIwidget *widget, uint64_t type, void *data)
{
}

bool gui_widget_loginback_callDraw(GUIwin *win, GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    guiDrawRTRender(st->loginback);
    guiDrawRRTRender(st->loginbackr2);

    // // 使用着色器
    // guiShaderUse(program.loginBack);
    // // guiShaderUse(program.img);

    // // 绑定纹理
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, st->texture);

    // // 设置模型矩阵
    // if (st->movX != st->movDX || st->movY != st->movDY)
    // {
    //     double time = glfwGetTime() - st->animTime;
    //     double factor = time * st->movA;

    //     st->movX += (st->movDX - st->movX) * factor;
    //     st->movY += (st->movDY - st->movY) * factor;

    //     glm_translate_make(st->model, (vec3){st->movX, st->movY, 0});
    // }
    // guiShaderUniformMatrix(program.loginBack, "model", 4fv, (float *)st->model);

    // st->animTime = glfwGetTime();

    // // 绘制
    // glBindVertexArray(st->VAO);
    // glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

    return true;
}

bool gui_widget_loginback_callEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event)
{

    if (event->type == GUI_EVENT_TYPE_CURSOR_POS)
    {
        gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

        // st->movDX = -(WINDOW_POS_2_GL_POS_x(event->CursorPos.xpos) / (WINDOW_WIDTH / 2)) * st->disW;
        // st->movDY = -(WINDOW_POS_2_GL_POS_y(event->CursorPos.ypos) / (WINDOW_HEIGHT / 2)) * st->disH;
    }
    return true;
}

// #include "gui_widget_loginback.h"

// #define SCALE 1.02f // 边框大小

// typedef struct gui_widget_loginback_struct
// {
//     // 基础数据
//     GLuint VAO;
//     GLuint VBO;
//     GLuint EBO;
//     GLuint texture;

//     int imageW, imageH; // 图像大小
//     int disW, disH;     // 边距

//     // 渲染需要的数据
//     mat4 model;

//     double movA;      // 移动加速度
//     int movX, movY;   // 当前位置
//     int movDX, movDY; // 要移动的目标

//     double animTime; // 动画时间
// } gui_widget_loginback_struct;

// void gui_widget_loginback_StartCall(GUIwidget *widget)
// {
//     // 分配内存
//     widget->data = malloc(sizeof(gui_widget_loginback_struct));
//     memset(widget->data, 0, sizeof(gui_widget_loginback_struct));
//     gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

//     // 加载图像
//     int nrChannels;
//     unsigned char *data = stbi_load(config.loginback_path, &st->imageW, &st->imageH, &nrChannels, 0);

//     // 创建纹理
//     st->texture = guiTextureCreate(data, st->imageW, st->imageH, nrChannels, GL_RGB);

//     // 调整大小
//     float scale = ((float)st->imageW / (float)WINDOW_WIDTH > (float)st->imageH / (float)WINDOW_HEIGHT) ? WINDOW_HEIGHT * SCALE / st->imageH : WINDOW_WIDTH * SCALE / st->imageW;

//     st->imageW *= scale;
//     st->imageH *= scale;

//     st->disW = (WINDOW_WIDTH - st->imageW) / 2;
//     st->disH = (WINDOW_HEIGHT - st->imageH) / 2;

//     // 生成顶点数组
//     int x = -st->imageW / 2;
//     int y = st->imageH / 2;
//     float vertex[] = {
//         // 顶点坐标       // 纹理坐标
//         x, y, 0.0f, 0.0f, 1.0f,   // 左上
//         -x, y, 0.0f, 1.0f, 1.0f,  // 右上
//         -x, -y, 0.0f, 1.0f, 0.0f, // 右下
//         x, -y, 0.0f, 0.0f, 0.0f   // 左下
//     };
//     int indices[] = {
//         0, 1, 2, 3};

//     // 创建VAO
//     glGenVertexArrays(1, &st->VAO);
//     glGenBuffers(1, &st->VBO);
//     glGenBuffers(1, &st->EBO);

//     // 绑定VAO
//     glBindVertexArray(st->VAO);

//     // 绑定VBO
//     glBindBuffer(GL_ARRAY_BUFFER, st->VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

//     // 绑定EBO
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, st->EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     // 设置顶点属性指针
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);

//     // 释放图像数据
//     stbi_image_free(data);
// }
// void gui_widget_loginback_DestroyCall(GUIwidget *widget)
// {
//     gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

//     // 释放VAO, VBO, EBO, 纹理
//     glDeleteVertexArrays(1, &st->VAO);
//     glDeleteBuffers(1, &st->VBO);
//     glDeleteBuffers(1, &st->EBO);
//     glDeleteTextures(1, &st->texture);

//     // 释放内存
//     free(widget->data);
// }

// void gui_widget_loginback_init(GUIwin *win, GUIwidget *widget)
// {
//     gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

//     // 设置模型矩阵
//     glm_mat4_identity(st->model);

//     // 设置移动速度
//     st->movA = 10;

//     // 要移动的目标
//     double x, y;
//     glfwGetCursorPos(win->window, &x, &y);
//     x = WINDOW_POS_2_GL_POS_x(x < 0 ? 0 : (x > WINDOW_WIDTH ? WINDOW_WIDTH : x));
//     y = WINDOW_POS_2_GL_POS_y(y < 0 ? 0 : (y > WINDOW_HEIGHT ? WINDOW_HEIGHT : y));

//     st->movDX = -(x / (WINDOW_WIDTH / 2)) * st->disW;
//     st->movDY = -(y / (WINDOW_HEIGHT / 2)) * st->disH;

//     st->movX = st->movDX;
//     st->movY = st->movDY;

//     glm_mat4_identity(st->model);
//     glm_translate(st->model, (vec3){-st->movX, -st->movY, 0});

//     // 设置动画时间
//     st->animTime = glfwGetTime();
// }

// void gui_widget_loginback_destroy(GUIwin *win, GUIwidget *widget)
// {
// }

// void gui_widget_loginback_msg(GUIwin *win, GUIwidget *widget, uint64_t type, void *data)
// {
// }

// bool gui_widget_loginback_callDraw(GUIwin *win, GUIwidget *widget)
// {
//     gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

//     // 使用着色器
//     guiShaderUse(program.img);

//     // 绑定纹理
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, st->texture);

//     // 设置模型矩阵
//     if (st->movX != st->movDX || st->movY != st->movDY)
//     {
//         double time = glfwGetTime() - st->animTime;
//         double factor = time * st->movA;

//         st->movX += (st->movDX - st->movX) * factor;
//         st->movY += (st->movDY - st->movY) * factor;

//         glm_translate_make(st->model, (vec3){st->movX, st->movY, 0});
//     }
//     guiShaderUniformMatrix(program.img, "model", 4fv, (float *)st->model);

//     st->animTime = glfwGetTime();

//     // 绘制
//     glBindVertexArray(st->VAO);
//     glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

//     return true;
// }

// bool gui_widget_loginback_callEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event)
// {
//     if (event->type == GUI_EVENT_TYPE_CURSOR_POS)
//     {
//         gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

//         st->movDX = -(WINDOW_POS_2_GL_POS_x(event->CursorPos.xpos) / (WINDOW_WIDTH / 2)) * st->disW;
//         st->movDY = -(WINDOW_POS_2_GL_POS_y(event->CursorPos.ypos) / (WINDOW_HEIGHT / 2)) * st->disH;
//     }
//     return true;
// }