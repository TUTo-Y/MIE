#include "gui_widget_wait.h"

// 创建和销毁调用函数
void gui_widget_wait_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GUIwidgetwait));
    GUIwidgetwait *st = GUI_ID2WIDGET(id)->data1;
    memset(GUI_ID2WIDGET(id)->data1, 0, sizeof(GUIwidgetwait));

    // 读取图片文件信息
    FILE *fp = fopen(config.wait_gif_path, "rb");
    size_t fp_size;
    unsigned char *fp_data = NULL;
    fseek(fp, 0, SEEK_END);
    fp_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fp_data = malloc(fp_size);
    fread(fp_data, 1, fp_size, fp);
    fclose(fp);

    // 读取图片信息
    int comp;
    st->imgData = stbi_load_gif_from_memory(fp_data, fp_size, &st->delays, &st->x, &st->y, &st->count, &comp, 4);
    free(fp_data);

    // 渲染
    st->texture = malloc(sizeof(GLuint) * st->count);
    for (int i = 0; i < st->count; i++)
        st->texture[i] = guiTextureCreate(st->imgData + i * st->x * st->y * 4, st->x, st->y, 4, GL_RGBA);

    // 创建VAO VBO, EBO
    float vertices[] = {
        -st->x / 2.0f, st->y / 2.0f, 0.0f, 1.0f,
        st->x / 2.0f, st->y / 2.0f, 1.0f, 1.0f,
        st->x / 2.0f, -st->y / 2.0f, 1.0f, 0.0f,
        -st->x / 2.0f, -st->y / 2.0f, 0.0f, 0.0f};
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};
    glGenVertexArrays(1, &st->VAO);
    glGenBuffers(1, &st->VBO);
    glGenBuffers(1, &st->EBO);
    glBindVertexArray(st->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, st->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, st->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 设置模型矩阵
    glm_mat4_identity(st->model);
}
void gui_widget_wait_logoffCall(GUIid id)
{
    GUIwidgetwait *st = GUI_ID2WIDGET(id)->data1;

    // 释放图片数据
    stbi_image_free(st->imgData);
    free(st->delays);
    for (int i = 0; i < st->count; i++)
        glDeleteTextures(1, &st->texture[i]);
    free(st->texture);

    // 释放VAO VBO EBO
    glDeleteVertexArrays(1, &st->VAO);
    glDeleteBuffers(1, &st->VBO);
    glDeleteBuffers(1, &st->EBO);
}

// 回调函数
void gui_widget_wait_loadCall(GUIid id)
{
    GUIwidgetwait *st = GUI_ID2WIDGET(id)->data1;

    st->time = glfwGetTime();
    st->index = 0;
}
void gui_widget_wait_uploadCall(GUIid id)
{
    GUIwidgetwait *st = GUI_ID2WIDGET(id)->data1;
}

bool gui_widget_wait_drawCall(GUIid id)
{
    GUIwidgetwait *st = GUI_ID2WIDGET(id)->data1;

    // 检查时间是否到了
    double time = glfwGetTime() - st->time;
    if (time >= st->delays[st->index] / 1000.0)
    {
        // 更新新的帧
        st->index = (st->index + 1) % st->count;
        st->time = glfwGetTime();
    }

    // 绘制
    glUseProgram(program.wait_gif);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.wait_gif, "model", 4fv, (float *)st->model);

    // 设置纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, st->texture[st->index]);
    
    // 绘制
    glBindVertexArray(st->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return true;
}