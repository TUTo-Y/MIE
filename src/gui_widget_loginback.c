#include "gui_widget_loginback.h"

typedef struct gui_widget_loginback_struct
{
    // 基础数据
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;

    int imageW, imageH; // 图像大小
    int disW, disH;     // 边距

    // 渲染需要的数据
    mat4 model;

    double movA;      // 移动加速度
    int movX, movY;   // 当前位置
    int movDX, movDY; // 要移动的目标

    double animTime; // 动画时间
} gui_widget_loginback_struct;

void gui_widget_loginback_StartCall(GUIwidget *widget)
{
    // 分配内存
    widget->data = malloc(sizeof(gui_widget_loginback_struct));
    memset(widget->data, 0, sizeof(gui_widget_loginback_struct));
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 加载图像
    char path[PATH_MAX];
    strcpy_s(path, PATH_MAX, config.res_path);
    strcat_s(path, PATH_MAX, "loginback.jpg");

    int nrChannels;
    unsigned char *data = stbi_load(path, &st->imageW, &st->imageH, &nrChannels, 0);

    // 创建纹理
    st->texture = guiTextureCreate(data, st->imageW, st->imageH, nrChannels, GL_RGB);

    // 调整大小
    float scale = ((float)st->imageW / (float)WINDOW_WIDTH > (float)st->imageH / (float)WINDOW_HEIGHT) ? WINDOW_HEIGHT * 1.01f / st->imageH : WINDOW_WIDTH * 1.01f / st->imageW;

    st->imageW *= scale;
    st->imageH *= scale;

    st->disW = (WINDOW_WIDTH - st->imageW) / 2;
    st->disH = (WINDOW_HEIGHT - st->imageH) / 2;

    // 计算各个顶点的坐标
    int x = -st->imageW / 2;
    int y = st->imageH / 2;

    // 生成顶点数组
    float vertex[] = {
        // 顶点坐标       // 纹理坐标
        x, y, 0.0f, 0.0f, 1.0f,   // 左上
        -x, y, 0.0f, 1.0f, 1.0f,  // 右上
        -x, -y, 0.0f, 1.0f, 0.0f, // 右下
        x, -y, 0.0f, 0.0f, 0.0f   // 左下
    };
    int indices[] = {
        0, 1, 2,
        2, 3, 0};

    // 创建VAO
    glGenVertexArrays(1, &st->VAO);
    glGenBuffers(1, &st->VBO);
    glGenBuffers(1, &st->EBO);

    // 绑定VAO
    glBindVertexArray(st->VAO);

    // 绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, st->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, st->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 释放图像数据
    stbi_image_free(data);
}
void gui_widget_loginback_DestroyCall(GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 释放VAO, VBO, EBO, 纹理
    glDeleteVertexArrays(1, &st->VAO);
    glDeleteBuffers(1, &st->VBO);
    glDeleteBuffers(1, &st->EBO);
    glDeleteTextures(1, &st->texture);

    // 释放内存
    free(widget->data);
}

void gui_widget_loginback_init(GUIwin *win, GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 设置模型矩阵
    glm_mat4_identity(st->model);

    // 设置移动速度
    st->movA = 10;

    // 要移动的目标
    double x, y;
    glfwGetCursorPos(win->window, &x, &y);
    x = WINDOW_POS_2_GL_POS_x(x < 0 ? 0 : (x > WINDOW_WIDTH ? WINDOW_WIDTH : x));
    y = WINDOW_POS_2_GL_POS_y(y < 0 ? 0 : (y > WINDOW_HEIGHT ? WINDOW_HEIGHT : y));

    st->movDX = -(x / (WINDOW_WIDTH / 2)) * st->disW;
    st->movDY = -(y / (WINDOW_HEIGHT / 2)) * st->disH;

    st->movX = st->movDX;
    st->movY = st->movDY;

    glm_mat4_identity(st->model);
    glm_translate(st->model, (vec3){-st->movX, -st->movY, 0});

    // 设置动画时间
    st->animTime = glfwGetTime();
}

void gui_widget_loginback_destroy(GUIwin *win, GUIwidget *widget)
{
}

void gui_widget_loginback_msg(GUIwin *win, GUIwidget *widget, uint64_t type, void *data)
{
}

bool gui_widget_loginback_callDraw(GUIwin *win, GUIwidget *widget)
{
    gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

    // 使用着色器
    guiShaderUse(program_img);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, st->texture);

    // 设置模型矩阵
    if (st->movX != st->movDX || st->movY != st->movDY)
    {
        double time = glfwGetTime() - st->animTime;

        st->movX += (st->movDX - st->movX) * time * st->movA;
        st->movY += (st->movDY - st->movY) * time * st->movA;

        glm_mat4_identity(st->model);
        glm_translate(st->model, (vec3){-st->movX, -st->movY, 0});
    }
    guiShaderUniformMatrix(program_img, "model", 4fv, (float *)st->model);

    st->animTime = glfwGetTime();

    // 绘制
    glBindVertexArray(st->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return true;
}

bool gui_widget_loginback_callEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event)
{
    if (event->type == GUI_EVENT_TYPE_CURSOR_POS)
    {
        gui_widget_loginback_struct *st = (gui_widget_loginback_struct *)widget->data;

        double x, y;
        // glfwGetCursorPos(win->window, &x, &y);
        x = event->CursorPos.xpos;
        y = event->CursorPos.ypos;
        x = WINDOW_POS_2_GL_POS_x(x);
        y = WINDOW_POS_2_GL_POS_y(y);

        st->movDX = -(x / (WINDOW_WIDTH / 2)) * st->disW;
        st->movDY = -(y / (WINDOW_HEIGHT / 2)) * st->disH;

        // st->movX = st->movDX;
        // st->movY = st->movDY;

        // glm_mat4_identity(st->model);
        // glm_translate(st->model, (vec3){st->movX, st->movY, 0});
    }
    return true;
}