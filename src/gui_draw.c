#include "gui_draw.h"

struct
{
    GLuint VAO, VBO, EBO;
} GUIt;

struct
{
    GLuint VAO, VBO, EBO;
} GUIc;

void guiDrawTInit()
{
    // 顶点数据
    float vertex[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f};
    unsigned int indices[] = {
        0, 1, 2, 3};

    // 创建VAO, VBO, EBO
    glGenVertexArrays(1, &GUIt.VAO);
    glBindVertexArray(GUIt.VAO);
    glGenBuffers(1, &GUIt.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GUIt.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glGenBuffers(1, &GUIt.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GUIt.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void guiDrawTQuit()
{
    glDeleteVertexArrays(1, &GUIt.VAO);
    glDeleteBuffers(1, &GUIt.VBO);
    glDeleteBuffers(1, &GUIt.EBO);
}

void guiDrawCInit()
{
    // 顶点数据
    float vertex[] = {
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f};
    unsigned int indices[] = {
        0, 1, 2, 3};

    // 创建VAO, VBO, EBO
    glGenVertexArrays(1, &GUIc.VAO);
    glBindVertexArray(GUIc.VAO);
    glGenBuffers(1, &GUIc.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GUIc.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glGenBuffers(1, &GUIc.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GUIc.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}
void guiDrawCQuit()
{
    glDeleteVertexArrays(1, &GUIc.VAO);
    glDeleteBuffers(1, &GUIc.VBO);
    glDeleteBuffers(1, &GUIc.EBO);
}

GUIdrawrr guiDrawRRTCreate(float width, float height, float r, float vague)
{
    // 创建圆角矩形
    GUIdrawrr drawrrt = (GUIdrawrr_t *)malloc(sizeof(GUIdrawrr_t));
    drawrrt->halfw = width / 2.0f;
    drawrrt->halfh = height / 2.0f;
    drawrrt->r = r;
    drawrrt->vague = vague;

    // 初始化fix
    glm_scale_make(drawrrt->fix, (vec3){drawrrt->halfw, drawrrt->halfh, 1.0f});

    // 初始化model
    glm_mat4_identity(drawrrt->model);

    // 初始化cut
    glm_mat4_identity(drawrrt->tex.cut);

    return drawrrt;
}

void guiDrawRRTBindTexture(GUIdrawrr drawrrt, GLuint texture)
{
    drawrrt->tex.texture = texture;

    // 获取纹理宽度和高度
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &drawrrt->tex.w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &drawrrt->tex.h);

    // 初始化cut
    glm_mat4_identity(drawrrt->tex.cut);
}

void guiDrawRRTCutTexture(GUIdrawrr drawrrt, float x, float y, float w, float h)
{
    // 获取纹理宽度和高度
    float width = drawrrt->tex.w, height = drawrrt->tex.h;

    // 设置cut
    y = height - y - h; // 坐标转换为右下角坐标系
    glm_translate_make(drawrrt->tex.cut, (vec3){x / width, y / height, 0.0f});
    glm_scale(drawrrt->tex.cut, (vec3){w / width, h / height, 1.0f});
}

void guiDrawRRTRender(GUIdrawrr drawrrt)
{
    guiShaderUse(program.rrt);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.rrt, "fix", 4fv, (float *)drawrrt->fix);
    guiShaderUniformMatrix(program.rrt, "model", 4fv, (float *)drawrrt->model);
    guiShaderUniformMatrix(program.rrt, "cut", 4fv, (float *)drawrrt->tex.cut);
    guiShaderUniform(program.rrt, "r", 1f, drawrrt->r);
    guiShaderUniform(program.rrt, "vague", 1f, drawrrt->vague);
    guiShaderUniform(program.rrt, "halfw", 1f, drawrrt->halfw);
    guiShaderUniform(program.rrt, "halfh", 1f, drawrrt->halfh);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, drawrrt->tex.texture);

    // 绘制
    glBindVertexArray(GUIt.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

GUIdrawrr guiDrawRRCCreate(float width, float height, float r, float vague, vec4 color)
{
    // 创建圆角矩形
    GUIdrawrr drawrrc = (GUIdrawrr_t *)malloc(sizeof(GUIdrawrr_t));
    drawrrc->halfw = width / 2.0f;
    drawrrc->halfh = height / 2.0f;
    drawrrc->r = r;
    drawrrc->vague = vague;

    // 初始化color
    glm_vec4_copy(color, drawrrc->color.color);

    // 初始化fix
    glm_scale_make(drawrrc->fix, (vec3){drawrrc->halfw, drawrrc->halfh, 1.0f});

    // 初始化model
    glm_mat4_identity(drawrrc->model);

    return drawrrc;
}

void guiDrawRRCRender(GUIdrawrr drawrrc)
{
    guiShaderUse(program.rrc);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.rrc, "fix", 4fv, (float *)drawrrc->fix);
    guiShaderUniformMatrix(program.rrc, "model", 4fv, (float *)drawrrc->model);
    guiShaderUniform(program.rrc, "r", 1f, drawrrc->r);
    guiShaderUniform(program.rrc, "vague", 1f, drawrrc->vague);
    guiShaderUniform(program.rrc, "halfw", 1f, drawrrc->halfw);
    guiShaderUniform(program.rrc, "halfh", 1f, drawrrc->halfh);
    guiShaderUniform(program.rrc, "color", 4f, drawrrc->color.color[0], drawrrc->color.color[1], drawrrc->color.color[2], drawrrc->color.color[3]);

    // 绘制
    glBindVertexArray(GUIc.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

GUIdrawr guiDrawRTCreate(float width, float height)
{
    GUIdrawr drawrt = (GUIdrawr_t *)malloc(sizeof(GUIdrawr_t));

    // 初始化fix
    glm_scale_make(drawrt->fix, (vec3){width / 2.0f, height / 2.0f, 1.0f});

    // 初始化model
    glm_mat4_identity(drawrt->model);

    // 初始化cut
    glm_mat4_identity(drawrt->tex.cut);

    return drawrt;
}

void guiDrawRTBindTexture(GUIdrawr drawrt, GLuint texture)
{
    drawrt->tex.texture = texture;

    // 获取纹理宽度和高度
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &drawrt->tex.w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &drawrt->tex.h);

    // 初始化cut
    glm_mat4_identity(drawrt->tex.cut);
}

void guiDrawRTCutTexture(GUIdrawr drawrt, float x, float y, float w, float h)
{
    // 获取纹理宽度和高度
    float width = drawrt->tex.w, height = drawrt->tex.h;

    // 设置cut
    y = height - y - h; // 坐标转换为右下角坐标系
    glm_translate_make(drawrt->tex.cut, (vec3){x / width, y / height, 0.0f});
    glm_scale(drawrt->tex.cut, (vec3){w / width, h / height, 1.0f});
}

void guiDrawRTRender(GUIdrawr drawrt)
{
    guiShaderUse(program.rt);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.rt, "fix", 4fv, (float *)drawrt->fix);
    guiShaderUniformMatrix(program.rt, "model", 4fv, (float *)drawrt->model);
    guiShaderUniformMatrix(program.rt, "cut", 4fv, (float *)drawrt->tex.cut);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, drawrt->tex.texture);

    // 绘制
    glBindVertexArray(GUIt.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

GUIdrawr guiDrawRCCreate(float width, float height, vec4 color)
{
    GUIdrawr drawrc = (GUIdrawr_t *)malloc(sizeof(GUIdrawr_t));

    // 初始化color
    glm_vec4_copy(color, drawrc->color.color);

    // 初始化fix
    glm_scale_make(drawrc->fix, (vec3){width / 2.0f, height / 2.0f, 1.0f});

    // 初始化model
    glm_mat4_identity(drawrc->model);

    return drawrc;
}

void guiDrawRCRender(GUIdrawr drawrc)
{
    guiShaderUse(program.rc);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.rc, "fix", 4fv, (float *)drawrc->fix);
    guiShaderUniformMatrix(program.rc, "model", 4fv, (float *)drawrc->model);
    guiShaderUniform(program.rc, "color", 4f, drawrc->color.color[0], drawrc->color.color[1], drawrc->color.color[2], drawrc->color.color[3]);

    // 绘制
    glBindVertexArray(GUIc.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

GUIdrawcc guiDrawCCCreate(float r, vec4 color)
{
    GUIdrawcc cc = (GUIdrawcc_t*) malloc(sizeof(GUIdrawcc_t));

    // 初始化color
    glm_vec4_copy(color, cc->color);

    // 初始化fix
    glm_scale_make(cc->fix, (vec3){r, r, 1.0f});

    // 初始化model
    glm_mat4_identity(cc->model);

    return cc;
}

void guiDrawCCRender(GUIdrawcc cc)
{
    guiShaderUse(program.cc);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.cc, "fix", 4fv, (float *)cc->fix);
    guiShaderUniformMatrix(program.cc, "model", 4fv, (float *)cc->model);
    guiShaderUniform(program.cc, "color", 4f, cc->color[0], cc->color[1], cc->color[2], cc->color[3]);

    // 绘制
    glBindVertexArray(GUIc.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

void guiDrawCC2Render(GUIdrawcc cc2)
{
    guiShaderUse(program.cc2);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.cc2, "fix", 4fv, (float *)cc2->fix);
    guiShaderUniformMatrix(program.cc2, "model", 4fv, (float *)cc2->model);
    guiShaderUniform(program.cc2, "color", 4f, cc2->color[0], cc2->color[1], cc2->color[2], cc2->color[3]);

    // 绘制
    glBindVertexArray(GUIc.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

}

GUIicon guiDrawIconCreate(float width, float height, GLuint texture, vec4 color)
{
    GUIicon icon = (GUIicon_t *)malloc(sizeof(GUIicon_t));

    // 初始化texture
    icon->texture = texture;

    // 初始化color
    glm_vec4_copy(color, icon->color);

    // 初始化fix
    glm_scale_make(icon->fix, (vec3){width / 2.0f, height / 2.0f, 1.0f});

    // 初始化model
    glm_mat4_identity(icon->model);

    return icon;
}

void guiDrawIconRender(GUIicon icon)
{
    guiShaderUse(program.icon);

    // 设置模型矩阵
    guiShaderUniformMatrix(program.icon, "fix", 4fv, (float *)icon->fix);
    guiShaderUniformMatrix(program.icon, "model", 4fv, (float *)icon->model);
    guiShaderUniform(program.icon, "color", 4f, icon->color[0], icon->color[1], icon->color[2], icon->color[3]);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, icon->texture);

    // 绘制
    glBindVertexArray(GUIt.VAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
}

GLuint guiDrawGaussian(GLuint texture, vec4 pos, int n, float step)
{
    // 获取基础数据
    float x = pos[0], y = pos[1], w = pos[2], h = pos[3];
    GLint width, height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    // 创建纹理
    GLuint textureGaussian[2];
    textureGaussian[0] = guiTextureCreateEmpty(w, h, GL_RGBA);
    textureGaussian[1] = guiTextureCreateEmpty(w, h, GL_RGBA);

    GLuint textureGaussian_t = texture;

    // 设置着色器, 第一次需要进行剪切
    guiShaderUse(program.gaussblur);
    y = height - y - h; // 坐标转换为右下角坐标系
    mat4 cut;
    glm_translate_make(cut, (vec3){x / width, y / height, 0.0f});
    glm_scale(cut, (vec3){w / width, h / height, 1.0f});
    guiShaderUse(program.gaussblur);
    guiShaderUniformMatrix(program.gaussblur, "cut", 4fv, (float *)cut);
    guiShaderUniform(program.gaussblur, "step", 1f, step);

    // 创建帧缓冲
    GUIfb fb = guiFbCreate(w, h);
    guiFbStart(fb);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            guiFbBindTexture(fb, textureGaussian[j]);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            guiShaderUniform(program.gaussblur, "horizontal", 1i, j);
            // 渲染
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureGaussian_t);
            textureGaussian_t = textureGaussian[j];
            glBindVertexArray(GUIt.VAO);
            glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

            // 第二次不再剪切
            mat4 cut = GLM_MAT4_IDENTITY_INIT;
            guiShaderUniformMatrix(program.gaussblur, "cut", 4fv, (float *)cut);
        }
    }

    GUI_SET_DEFAULT_CLEARCOLOR();
    // 结束渲染
    guiFbEnd(fb);
    guiFbDelete(fb);

    // 销毁纹理
    glDeleteTextures(1, &textureGaussian[0]);

    return textureGaussian[1];
}