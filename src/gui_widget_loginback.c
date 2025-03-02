#include "gui_widget_loginback.h"

#define SCALE 1.04f // 边框大小
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
    st->loginbackr2Tex = guiDrawGaussian(st->loginbackTex, (vec4){0, 0, st->imageW, st->imageH}, 20, 5.0f);

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

    // 初始化物理数据
    double x, y;
    glfwGetCursorPos(win->window, &x, &y);
    x = WINDOW_POS_2_GL_POS_x(x < 0 ? 0 : (x > WINDOW_WIDTH ? WINDOW_WIDTH : x));
    y = WINDOW_POS_2_GL_POS_y(y < 0 ? 0 : (y > WINDOW_HEIGHT ? WINDOW_HEIGHT : y));

    st->movDX = -(x / (WINDOW_WIDTH / 2)) * st->disW;
    st->movDY = -(y / (WINDOW_HEIGHT / 2)) * st->disH;

    st->movX = st->movDX;
    st->movY = st->movDY;

    st->animTime = glfwGetTime();

    // 设置loginback
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){-st->movX, -st->movY, 0});
    guiDrawRTSetModel(st->loginback, model);

    // 设置loginback2
    guiDrawRRTCutTexture(st->loginbackr2,
                         st->movX + st->disW + LOGINBACK2DISW,
                         -st->movY + st->disH + LOGINBACK2DISH,
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

    // 设置模型矩阵
    if (st->movX != st->movDX || st->movY != st->movDY)
    {
        double time = glfwGetTime() - st->animTime;
        double factor = time * 5;

        double diffX = (st->movDX - st->movX) / 2.0f + 1;
        double diffY = (st->movDY - st->movY) / 2.0f + 1;
        st->movX += (int)diffX;
        st->movY += (int)diffY;

        // 设置loginback
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model, (vec3){-st->movX, -st->movY, 0});
        guiDrawRTSetModel(st->loginback, model);

        // 设置loginback2
        guiDrawRRTCutTexture(st->loginbackr2,
                             st->movX + st->disW + LOGINBACK2DISW,
                             -st->movY + st->disH + LOGINBACK2DISH,
                             LOGINBACK2WIDTH, LOGINBACK2HEIGHT);
    }

    // 渲染背景
    guiDrawRTRender(st->loginback);

    // 渲染圆角背景
    guiDrawRRTRender(st->loginbackr2);

    st->animTime = glfwGetTime();

    return true;
}

bool gui_widget_loginback_callEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event)
{

    if (event->type == GUI_EVENT_TYPE_CURSOR_POS)
    {
        gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

        st->movDX = -(WINDOW_POS_2_GL_POS_x(event->CursorPos.xpos) / (WINDOW_WIDTH / 2)) * st->disW;
        st->movDY = -(WINDOW_POS_2_GL_POS_y(event->CursorPos.ypos) / (WINDOW_HEIGHT / 2)) * st->disH;
    }
    return true;
}
