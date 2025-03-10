/**
 * \file gui_draw.h
 * \brief GUI绘制
 */
#ifndef GUI_DRAW_H
#define GUI_DRAW_H

#include <math.h>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "gui_base.h"
#include "gui_shader.h"
#include "gui_program.h"

#define GUI_DRAW_DEFAULT_VAGUE (0.02f)

// 圆角矩形
typedef struct
{
    mat4 model;
    mat4 fix;

    // 圆角信息
    float halfw, halfh;
    float r;
    float vague;

    // 颜色信息
    union
    {
        struct
        {
            vec4 color;
        } color;
        struct
        {
            GLuint texture;
            mat4 cut; // 纹理裁剪
            int w, h; // 纹理宽度和高度
        } tex;
    };

} GUIdrawrr_t;
typedef GUIdrawrr_t *GUIdrawrr;

// 矩形
typedef struct
{
    mat4 model;
    mat4 fix;

    // 颜色信息
    union
    {
        struct
        {
            vec4 color;
        } color;
        struct
        {
            GLuint texture;
            mat4 cut; // 纹理裁剪
            int w, h; // 纹理宽度和高度
        } tex;
    };
} GUIdrawr_t;
typedef GUIdrawr_t *GUIdrawr;

// 圆
typedef struct
{
    mat4 model;
    mat4 fix;
    vec4 color;
} GUIdrawcc_t;
typedef GUIdrawcc_t *GUIdrawcc;

typedef struct
{
    mat4 model;
    mat4 fix;

    vec4 color;
    GLuint texture;
} GUIicon_t;
typedef GUIicon_t *GUIicon;

/**
 * \brief 初始化和退出纹理矩形渲染
 */
void guiDrawTInit();
void guiDrawTQuit();

/**
 * \brief 初始化和退出颜色矩形渲染
 */
void guiDrawCInit();
void guiDrawCQuit();

/**
 * \brief 创建圆角纹理矩形texture
 * \param width 宽度
 * \param height 高度
 * \param r 圆角半径
 * \param vague 模糊度，通常为0.02
 * \return 圆角矩形
 */
GUIdrawrr guiDrawRRTCreate(float width, float height, float r, float vague);

/**
 * \brief 绑定一个纹理texture
 * \param drawrrt 圆角矩形
 * \param texture 纹理
 */
void guiDrawRRTBindTexture(GUIdrawrr drawrrt, GLuint texture);

/**
 * \brief 剪切显示绑定的纹理
 * \param drawrrt 圆角矩形
 * \param x 纹理x坐标, 左上角为原点
 * \param y 纹理y坐标, 左上角为原点
 * \param w 纹理宽度
 * \param h 纹理高度
 * \note 该函数会将纹理坐标转换为左下角坐标系
 */
void guiDrawRRTCutTexture(GUIdrawrr drawrrt, float x, float y, float w, float h);

/**
 * \brief 设置圆角纹理矩形模糊度
 * \param drawrrt 圆角矩形
 * \param vague 模糊度
 */
static inline void guiDrawRRTSetVague(GUIdrawrr drawrrt, float vague)
{
    drawrrt->vague = vague;
}

/**
 * \brief 设置圆角纹理矩形半径
 * \param drawrrt 圆角矩形
 * \param r 半径
 */
static inline void guiDrawRRTSetR(GUIdrawrr drawrrt, float r)
{
    drawrrt->r = r;
}

/**
 * \brief 设置圆角纹理矩形宽度和高度
 * \param drawrrt 圆角矩形
 * \param width 宽度
 */
static inline void guiDrawRRTSetWH(GUIdrawrr drawrrt, float width, float height)
{
    drawrrt->halfw = width / 2.0f;
    drawrrt->halfh = height / 2.0f;
    glm_scale_make(drawrrt->fix, (vec3){drawrrt->halfw, drawrrt->halfh, 1.0f});
}

/**
 * \brief 设置圆角纹理矩形model
 * \param drawrrt 圆角矩形
 * \param model model矩阵
 */
static inline void guiDrawRRTSetModel(GUIdrawrr drawrrt, mat4 model)
{
    glm_mat4_copy(model, drawrrt->model);
}

/**
 * \brief 渲染圆角纹理矩形
 * \param drawrrt 圆角矩形
 */
void guiDrawRRTRender(GUIdrawrr drawrrt);

/**
 * \brief 销毁圆角纹理矩形
 * \param drawrrt 圆角矩形
 */
static inline void guiDrawRRTDestroy(GUIdrawrr drawrrt)
{
    free(drawrrt);
}

/**
 * \brief 创建圆角颜色矩形
 * \param width 宽度
 * \param height 高度
 * \param r 圆角半径
 * \param vague 模糊度，通常为0.02
 * \param color 颜色
 */
GUIdrawrr guiDrawRRCCreate(float width, float height, float r, float vague, vec4 color);

/**
 * \brief 设置圆角颜色矩形颜色
 * \param drawrrc 圆角矩形
 * \param color 颜色
 */
static inline void guiDrawRRCSetColor(GUIdrawrr drawrrc, vec4 color)
{
    glm_vec4_copy(color, drawrrc->color.color);
}

/**
 * \brief 设置圆角颜色矩形模糊度
 * \param drawrrc 圆角矩形
 * \param vague 模糊度
 */
static inline void guiDrawRRCSetVague(GUIdrawrr drawrrc, float vague)
{
    drawrrc->vague = vague;
}
/**
 * \brief 设置圆角颜色矩形半径
 * \param drawrrc 圆角矩形
 * \param r 半径
 */
static inline void guiDrawRRCSetR(GUIdrawrr drawrrc, float r)
{
    drawrrc->r = r;
}

/**
 * \brief 设置圆角颜色矩形宽度和高度
 * \param drawrrc 圆角矩形
 * \param width 宽度
 */
static inline void guiDrawRRCSetWH(GUIdrawrr drawrrc, float width, float height)
{
    drawrrc->halfw = width / 2.0f;
    drawrrc->halfh = height / 2.0f;
    glm_scale_make(drawrrc->fix, (vec3){drawrrc->halfw, drawrrc->halfh, 1.0f});
}

/**
 * \brief 设置圆角颜色矩形model
 * \param drawrrc 圆角矩形
 * \param model model矩阵
 */
static inline void guiDrawRRCSetModel(GUIdrawrr drawrrc, mat4 model)
{
    glm_mat4_copy(model, drawrrc->model);
}

/**
 * \brief 渲染圆角颜色矩形
 * \param drawrrc 圆角矩形
 */
void guiDrawRRCRender(GUIdrawrr drawrrc);

/**
 * \brief 销毁圆角颜色矩形
 * \param drawrrc 圆角矩形
 */
static inline void guiDrawRRCDestroy(GUIdrawrr drawrrc)
{
    free(drawrrc);
}

/**
 * \brief 创建纹理矩形
 * \param width 宽度
 * \param height 高度
 */
GUIdrawr guiDrawRTCreate(float width, float height);

/**
 * \brief 绑定一个纹理texture
 * \param drawrt 矩形
 * \param texture 纹理
 */
void guiDrawRTBindTexture(GUIdrawr drawrt, GLuint texture);

/**
 * \brief 剪切显示绑定的纹理
 * \param drawrt 矩形
 * \param x 纹理x坐标, 左上角为原点
 * \param y 纹理y坐标, 左上角为原点
 * \param w 纹理宽度
 * \param h 纹理高度
 * \note 该函数会将纹理坐标转换为左下角坐标系
 */
void guiDrawRTCutTexture(GUIdrawr drawrt, float x, float y, float w, float h);

/**
 * \brief 设置纹理矩形宽度和高度
 * \param drawrt 矩形
 * \param width 宽度
 */
static inline void guiDrawRTSetWH(GUIdrawr drawrt, float width, float height)
{
    glm_scale_make(drawrt->fix, (vec3){width / 2.0f, height / 2.0f, 1.0f});
}

/**
 * \brief 设置纹理矩形model
 * \param drawrt 矩形
 * \param model model矩阵
 */
static inline void guiDrawRTSetModel(GUIdrawr drawrt, mat4 model)
{
    glm_mat4_copy(model, drawrt->model);
}

/**
 * \brief 渲染纹理矩形
 * \param drawrt 矩形
 */
void guiDrawRTRender(GUIdrawr drawrt);

/**
 * \brief 销毁纹理矩形
 * \param drawrt 矩形
 */
static inline void guiDrawRTDestroy(GUIdrawr drawrt)
{
    free(drawrt);
}

/**
 * \brief 创建颜色矩形
 * \param width 宽度
 * \param height 高度
 * \param color 颜色
 */
GUIdrawr guiDrawRCCreate(float width, float height, vec4 color);

/**
 * \brief 设置颜色矩形颜色
 * \param drawrc 矩形
 * \param color 颜色
 */
static inline void guiDrawRCSetColor(GUIdrawr drawrc, vec4 color)
{
    glm_vec4_copy(color, drawrc->color.color);
}

/**
 * \brief 设置颜色矩形宽度和高度
 * \param drawrc 矩形
 * \param width 宽度
 */
static inline void guiDrawRCSetWH(GUIdrawr drawrc, float width, float height)
{
    glm_scale_make(drawrc->fix, (vec3){width / 2.0f, height / 2.0f, 1.0f});
}

/**
 * \brief 设置颜色矩形model
 * \param drawrc 矩形
 * \param model model矩阵
 */
static inline void guiDrawRCSetModel(GUIdrawr drawrc, mat4 model)
{
    glm_mat4_copy(model, drawrc->model);
}

/**
 * \brief 渲染颜色矩形
 * \param drawrc 矩形
 */
void guiDrawRCRender(GUIdrawr drawrc);

/**
 * \brief 销毁颜色矩形
 * \param drawrc 矩形
 */
static inline void guiDrawRCDestroy(GUIdrawr drawrc)
{
    free(drawrc);
}

/**
 * \brief 创建圆
 * \param
 */
GUIdrawcc guiDrawCCCreate(float r, vec4 color);

/**
 * \brief 销毁圆
 */
static inline void guiDrawCCDestroy(GUIdrawcc cc)
{
    free(cc);
}

/**
 * \brief 设置圆颜色
 */
static inline void guiDrawCCSetColor(GUIdrawcc cc, vec4 color)
{
    glm_vec4_copy(color, cc->color);
}

/**
 * \brief 设置圆半径
 */
static inline void guiDrawCCSetR(GUIdrawcc cc, float r)
{
    glm_scale_make(cc->fix, (vec3){r, r, 1.0f});
}

/**
 * \brief 设置圆model
 */
static inline void guiDrawCCSetModel(GUIdrawcc cc, mat4 model)
{
    glm_mat4_copy(model, cc->model);
}

/**
 * \brief 设置圆位置
 */
static inline void guiDrawCCSetPos(GUIdrawcc cc, float x, float y)
{
    glm_translate_make(cc->model, (vec3){x, y, 0});
}

/**
 * \brief 渲染圆
 * \param cc 圆
 */
void guiDrawCCRender(GUIdrawcc cc);
void guiDrawCC2Render(GUIdrawcc cc2);

/**
 * \brief 创建图标
 * \param width 宽度
 * \param height 高度
 * \param texture 图标纹理
 * \param color 颜色
 */
GUIicon guiDrawIconCreate(float width, float height, GLuint texture, vec4 color);

/**
 * \brief 设置图标纹理
 * \param icon 图标
 * \param texture 纹理
 */
static inline void guiDrawIconBindTexture(GUIicon icon, GLuint texture)
{
    icon->texture = texture;
}

/**
 * \brief 设置图标颜色
 * \param icon 图标
 * \param color 颜色
 */
static inline void guiDrawIconSetColor(GUIicon icon, vec4 color)
{
    glm_vec4_copy(color, icon->color);
}

/**
 * \brief 设置图标宽度和高度
 * \param icon 图标
 * \param width 宽度
 * \param height 高度
 */
static inline void guiDrawIconSetWH(GUIicon icon, float width, float height)
{
    glm_scale_make(icon->fix, (vec3){width / 2.0f, height / 2.0f, 1.0f});
}

/**
 * \brief 设置图标model
 * \param icon 图标
 * \param model model矩阵
 */
static inline void guiDrawIconSetModel(GUIicon icon, mat4 model)
{
    glm_mat4_copy(model, icon->model);
}

/**
 * \brief 渲染图标
 * \param icon 图标
 */
void guiDrawIconRender(GUIicon icon);

/**
 * \brief 销毁图标
 * \param icon 图标
 */
static inline void guiDrawIconDestroy(GUIicon icon)
{
    free(icon);
}

/**
 * \brief 渲染一个高斯模糊图像
 * \param texture 纹理
 * \param pos 位置x, y, w, h
 * \param n n次模糊
 * \param step 步长
 * \return GLuint 高斯纹理
 */
GLuint guiDrawGaussian(GLuint texture, vec4 pos, int n, float step);

/**
 * ********************************************************************************************************************************
 */
/**
 * 写在一起太难看了，下面这部分和上面分开
 */
/**
 * ********************************************************************************************************************************
 */

typedef struct
{
    GLuint VAO, VBO, EBO;
    
    mat4 model;
    mat4 fix;

    // 圆角信息
    float halfw, halfh;
    float r;
    float vague;

    // 颜色信息
    float alpha;
} GUIdrawrrc2_t;
typedef GUIdrawrrc2_t *GUIdrawrrc2;
/**
 * \brief 创建圆角矩形
 * \param color 颜色, 左上角，右上角，右下角，左下角
 * \param r 圆角半径
 * \param vague 模糊度
 * \return 圆角矩形
 */
GUIdrawrrc2 guiDrawRRC2Create(vec4 color[4], float r, float vague);

/**
 * \brief 设置圆角矩形宽度和高度
 * \param drawrrc2 圆角矩形
 * \param width 宽度
 * \param height 高度
 */
static inline void guiDrawRRC2SetWH(GUIdrawrrc2 drawrrc2, float width, float height)
{
    drawrrc2->halfw = width / 2.0f;
    drawrrc2->halfh = height / 2.0f;
    glm_scale_make(drawrrc2->fix, (vec3){drawrrc2->halfw, drawrrc2->halfh, 1.0f});
}

/**
 * \brief 设置圆角矩形model
 * \param drawrrc2 圆角矩形
 * \param model model矩阵
 */
static inline void guiDrawRRC2SetModel(GUIdrawrrc2 drawrrc2, mat4 model)
{
    glm_mat4_copy(model, drawrrc2->model);
}

/**
 * \brief 设置圆角矩形半径
 * \param drawrrc2 圆角矩形
 * \param r 半径
 */
static inline void guiDrawRRC2SetR(GUIdrawrrc2 drawrrc2, float r)
{
    drawrrc2->r = r;
}

/**
 * \brief 设置圆角矩形模糊度
 * \param drawrrc2 圆角矩形
 * \param vague 模糊度
 */
static inline void guiDrawRRC2SetVague(GUIdrawrrc2 drawrrc2, float vague)
{
    drawrrc2->vague = vague;
}

/**
 * \brief 设置圆角矩形透明度
 * \param drawrrc2 圆角矩形
 * \param alpha 透明度
 */
static inline void guiDrawRRC2SetAlpha(GUIdrawrrc2 drawrrc2, float alpha)
{
    drawrrc2->alpha = alpha;
}

/**
 * \brief 渲染圆角矩形
 * \param drawrrc2 圆角矩形
 */
void guiDrawRRC2Render(GUIdrawrrc2 drawrrc2);

/**
 * \brief 销毁圆角矩形
 * \param drawrrc2 圆角矩形
 */
static inline void guiDrawRRC2Destroy(GUIdrawrrc2 drawrrc2)
{
    glDeleteVertexArrays(1, &drawrrc2->VAO);
    glDeleteBuffers(1, &drawrrc2->VBO);
    glDeleteBuffers(1, &drawrrc2->EBO);

    free(drawrrc2);
}

#endif // GUI_DRAW_H