#include "gui_widget_loginback.h"

#define SCALE 1.04f // 边框大小
#define LOGINBACK2SCALE 0.7f
#define LOGINBACK2WIDTH (((float)WINDOW_WIDTH) * LOGINBACK2SCALE)
#define LOGINBACK2HEIGHT (((float)WINDOW_HEIGHT) * LOGINBACK2SCALE)
#define LOGINBACK2DISW (((float)WINDOW_WIDTH) * (1.0f - LOGINBACK2SCALE) / 2.0f)
#define LOGINBACK2DISH (((float)WINDOW_HEIGHT) * (1.0f - LOGINBACK2SCALE) / 2.0f)

void gui_widget_loginback_registerCall(GUIid id)
{
    // 分配内存
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(gui_widget_loginback_struct));
    memset(GUI_ID2WIDGET(id)->data1, 0, sizeof(gui_widget_loginback_struct));
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)GUI_ID2WIDGET(id)->data1;

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
        ERR("调整大小错误\n");

    // 创建纹理和高斯模糊纹理
    st->loginbackTex = guiTextureCreate(data2, st->imageW, st->imageH, 3, GL_RGB);
    st->loginbackr2Tex = guiDrawGaussian(st->loginbackTex, (vec4){0, 0, st->imageW, st->imageH}, 20, 5.0f);

    // 释放图像数据
    stbi_image_free(data);
    stbi_image_free(data2);

    // 创建用户图标和密码图标
    unsigned char *user_data = stbi_load(config.user_icon_path, &width, &height, &nrChannels, 4);
    st->userIcon = guiTextureCreate(user_data, width, height, 4, GL_RGBA);
    st->user = guiDrawIconCreate(width, height, st->userIcon, (vec4){8.0f, 8.0f, 8.0f, 9.0f});
    stbi_image_free(user_data);

    unsigned char *pass_data = stbi_load(config.pass_icon_path, &width, &height, &nrChannels, 4);
    st->passIcon = guiTextureCreate(pass_data, width, height, 4, GL_RGBA);
    st->pass = guiDrawIconCreate(width, height, st->passIcon, (vec4){8.0f, 8.0f, 8.0f, 9.0f});
    stbi_image_free(pass_data);

    // 设置图标位置
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate_make(model, (vec3){-300, 160 - 80.0f, 0});
    guiDrawIconSetModel(st->user, model);
    glm_translate(model, (vec3){0, -160, 0});
    guiDrawIconSetModel(st->pass, model);
    st->drawIcon = true;

    // 加载logo
    {
        float vertex[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f};
        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3};
        
        glGenVertexArrays(1, &st->logoVAO);
        glGenBuffers(1, &st->logoVBO);
        glGenBuffers(1, &st->logoEBO);
        glBindVertexArray(st->logoVAO);
        glBindBuffer(GL_ARRAY_BUFFER, st->logoVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, st->logoEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        unsigned char *logo_data = stbi_load(config.logo_icon_path, &width, &height, &nrChannels, 4);
        st->logoTex = guiTextureCreate(logo_data, width, height, 4, GL_RGBA);
        
        glm_translate_make(st->logoModel, (vec3){-650.0f, 377.0f, 0.0f});
        glm_scale(st->logoModel, (vec3){100.0f, 100.0f, 1.0f});

        GLFWimage icon[1];
        icon[0].pixels = logo_data;
        icon[0].width = width;
        icon[0].height = height;
        glfwSetWindowIcon(GUI_GETWINDOW()->window, 1, icon);
        glfwSetWindowTitle(GUI_GETWINDOW()->window, "MIE - 医学图像加密测试系统");

        stbi_image_free(logo_data);
    }

    // 加载字体
    {
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate_make(model, (vec3){0.0f, 260.0f - 10.0f, 0});
        st->titleTTF = guiTTFCreate(resGetFile(config.title_ttf_path, NULL, NULL, false), 72, 0);
        st->title = guiStrCreate(st->titleTTF, 72, GUI_STR_MOD_CENTER, program.font, model, (vec4){0.1f, 0.1f, 0.1f, 1.0f});
        guiStrCpy(st->title, L"隐图云诊");
    }
}
void gui_widget_loginback_logoffCall(GUIid id)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)GUI_ID2WIDGET(id)->data1;

    // 释放纹理
    glDeleteTextures(1, &st->loginbackTex);
    glDeleteTextures(1, &st->loginbackr2Tex);
    glDeleteTextures(1, &st->userIcon);
    glDeleteTextures(1, &st->passIcon);

    // 释放图标
    guiDrawIconDestroy(st->user);
    guiDrawIconDestroy(st->pass);

    // 释放内存
    guiDrawRTDestroy(st->loginback);
    guiDrawRRTDestroy(st->loginbackr2);

    // 释放logo
    glDeleteVertexArrays(1, &st->logoVAO);
    glDeleteBuffers(1, &st->logoVBO);
    glDeleteBuffers(1, &st->logoEBO);
    glDeleteTextures(1, &st->logoTex);

    // 释放文字
    guiStrDelete(st->title);
    guiTTFDelete(st->titleTTF);

    // 释放内存
    free(st);
}

void gui_widget_loginback_loadCall(GUIid id)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)GUI_ID2WIDGET(id)->data1;

    // 生成渲染器
    st->loginback = guiDrawRTCreate(st->imageW, st->imageH);
    st->loginbackr2 = guiDrawRRTCreate(LOGINBACK2WIDTH, LOGINBACK2HEIGHT, 30.0f, GUI_DRAW_DEFAULT_VAGUE);

    // 绑定纹理
    guiDrawRTBindTexture(st->loginback, st->loginbackTex);
    guiDrawRRTBindTexture(st->loginbackr2, st->loginbackr2Tex);

    // 初始化物理数据
    double x, y;
    glfwGetCursorPos(GUI_GETWINDOW()->window, &x, &y);
    x = (x < 0 ? 0 : (x > WINDOW_WIDTH ? WINDOW_WIDTH : x));
    y = (y < 0 ? 0 : (y > WINDOW_HEIGHT ? WINDOW_HEIGHT : y));
    x = WINDOW_POS_2_GL_POS_x(x, WINDOW_WIDTH);
    y = WINDOW_POS_2_GL_POS_y(y, WINDOW_HEIGHT);

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

void gui_widget_loginback_uploadCall(GUIid id)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)GUI_ID2WIDGET(id)->data1;

    // 释放渲染器
    guiDrawRTDestroy(st->loginback);
    guiDrawRRTDestroy(st->loginbackr2);
}

bool gui_widget_loginback_drawCall(GUIid id)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)GUI_ID2WIDGET(id)->data1;

    // 设置模型矩阵
    if (st->movDX != st->movX || st->movDY != st->movY)
    {
        int tmp;
        st->movX += (tmp = st->movDX - st->movX) ? tmp / 2 + 1 : 0;
        st->movY += (tmp = st->movDY - st->movY) ? tmp / 2 + 1 : 0;

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

    // 渲染图标
    if (st->drawIcon)
    {
        guiDrawIconRender(st->user);
        guiDrawIconRender(st->pass);
    }

    st->animTime = glfwGetTime();

    // 渲染logo
    guiShaderUse(program.wait_gif);
    guiShaderUniformMatrix(program.wait_gif, "model", 4fv, (float *)st->logoModel);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, st->logoTex);

    glBindVertexArray(st->logoVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 渲染标题
    guiStrRender(st->title);

    return true;
}

bool gui_widget_loginback_eventCall(GUIid id, const GUIevent *event)
{

    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)GUI_ID2WIDGET(id)->data1;
    if (event->type == GUI_EVENT_TYPE_CURSOR_POS)
    {
        st->movDX = -(event->CursorPos.xpos / (WINDOW_WIDTH / 2.0f)) * st->disW;
        st->movDY = -(event->CursorPos.ypos / (WINDOW_HEIGHT / 2.0f)) * st->disH;
    }
    else if (event->type == GUI_WIDGET_LOGINBACK_DRAW_ICON)
    {
        st->drawIcon = (bool)event->Custom.data1;
    }
    return true;
}
