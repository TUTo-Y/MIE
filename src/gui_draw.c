#include "gui_draw.h"

GUIdrawrr guiDrawRRTCreate(float width, float height, float r, float vague)
{
    // 创建圆角矩形
    GUIdrawrr drawrr = (GUIdrawrr_t *)malloc(sizeof(GUIdrawrr_t));
    drawrr->width = width;
    drawrr->height = height;
    drawrr->r = r;
    drawrr->vague = vague;

    // 顶点数据
    float vertex[] = {
        -width / 2.0f, height / 2.0f, 0.0f, 1.0f,
        width / 2.0f, height / 2.0f, 1.0f, 1.0f,
        width / 2.0f, -height / 2.0f, 1.0f, 0.0f,
        -width / 2.0f, -height / 2.0f, 0.0f, 0.0f};
    unsigned int indices[] = {
        0, 1, 2, 3};

    // 创建VAO
    glGenVertexArrays(1, &drawrr->VAO);
    glBindVertexArray(drawrr->VAO);
    glGenBuffers(1, &drawrr->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, drawrr->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glGenBuffers(1, &drawrr->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawrr->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 初始化model
    glm_mat4_identity(drawrr->model);

    return drawrr;
}

void guiDrawRRTApplyTexture(GUIdrawrr drawrr, GLuint texture, float x, float y, float w, float h, float width, float height)
{
    drawrr->texture.texture = texture;

    y = height - y - h; // 坐标转换为右下角坐标系
    glm_translate_make(drawrr->texture.cut, (vec3){x / width, y / height, 0.0f});
    glm_scale(drawrr->texture.cut, (vec3){w / width, h / height, 1.0f});
}

void guiDrawRRTRender(GUIdrawrr drawrr)
{
    guiShaderUse(program.rrt);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.rrt, "cut", 4fv, (float *)drawrr->texture.cut);
    guiShaderUniformMatrix(program.rrt, "model", 4fv, (float *)drawrr->model);
    guiShaderUniform(program.rrt, "r", 1f, drawrr->r);
    guiShaderUniform(program.rrt, "vague", 1f, drawrr->vague);
    guiShaderUniform(program.rrt, "halfw", 1f, drawrr->width / 2.0f);
    guiShaderUniform(program.rrt, "halfh", 1f, drawrr->height / 2.0f);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, drawrr->texture.texture);

    // 绘制
    glBindVertexArray(drawrr->VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

void guiDrawRRTDestroy(GUIdrawrr drawrr)
{
    glDeleteVertexArrays(1, &drawrr->VAO);
    glDeleteBuffers(1, &drawrr->VBO);
    glDeleteBuffers(1, &drawrr->EBO);
    free(drawrr);
}

GUIdrawrr guiDrawRRCCreate(float width, float height, float r, float vague, vec4 color)
{
    // 创建圆角矩形
    GUIdrawrr drawrr = (GUIdrawrr_t *)malloc(sizeof(GUIdrawrr_t));
    drawrr->width = width;
    drawrr->height = height;
    drawrr->r = r;
    drawrr->vague = vague;
    glm_vec4_copy(color, drawrr->color.color);

    // 顶点数据
    float vertex[] = {
        -width / 2.0f, height / 2.0f,
        width / 2.0f, height / 2.0f,
        width / 2.0f, -height / 2.0f,
        -width / 2.0f, -height / 2.0f};
    unsigned int indices[] = {
        0, 1, 2, 3};

    // 创建VAO
    glGenVertexArrays(1, &drawrr->VAO);
    glBindVertexArray(drawrr->VAO);
    glGenBuffers(1, &drawrr->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, drawrr->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glGenBuffers(1, &drawrr->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawrr->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 初始化model
    glm_mat4_identity(drawrr->model);

    return drawrr;
}

void guiDrawRRCRender(GUIdrawrr drawrrc)
{
    guiShaderUse(program.rrc);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.rrc, "model", 4fv, (float *)drawrrc->model);
    guiShaderUniform(program.rrc, "r", 1f, drawrrc->r);
    guiShaderUniform(program.rrc, "vague", 1f, drawrrc->vague);
    guiShaderUniform(program.rrc, "halfw", 1f, drawrrc->width / 2.0f);
    guiShaderUniform(program.rrc, "halfh", 1f, drawrrc->height / 2.0f);
    guiShaderUniform(program.rrc, "color", 4f, drawrrc->color.color[0], drawrrc->color.color[1], drawrrc->color.color[2], drawrrc->color.color[3]);

    // 绘制
    glBindVertexArray(drawrrc->VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

void guiDrawRRCDestroy(GUIdrawrr drawrrc)
{
    glDeleteVertexArrays(1, &drawrrc->VAO);
    glDeleteBuffers(1, &drawrrc->VBO);
    glDeleteBuffers(1, &drawrrc->EBO);
    free(drawrrc);
}