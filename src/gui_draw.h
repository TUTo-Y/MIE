#ifndef GUI_DRAW_H
#define GUI_DRAW_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "gui_shader.h"
#include "gui_program.h"

// 圆角矩形
typedef struct GUIdrawrr_t
{
    GLuint VAO, VBO, EBO;

    // 渲染位置
    float width, height;
    float r;
    float vague;

    // 颜色信息
    union gui_draw
    {
        struct
        {
            vec4 color;
        } color;
        struct
        {
            GLuint texture;
            mat4 cut; // 纹理裁剪
        } texture;
    };

    mat4 model;

} GUIdrawrr_t;
typedef GUIdrawrr_t *GUIdrawrr;

/**
 * \brief 创建圆角矩形texture
 * \param width 宽度
 * \param height 高度
 * \param r 圆角半径
 * \param vague 模糊度，通常为0.02
 * \return 圆角矩形
 */
GUIdrawrr guiDrawRRTCreate(float width, float height, float r, float vague);

/**
 * \brief 应用一个纹理texture
 * \param drawrr 圆角矩形
 * \param texture 纹理
 * \param x 纹理x坐标, 左上角为原点
 * \param y 纹理y坐标, 左上角为原点
 * \param w 纹理宽度, 左上角为原点
 * \param h 纹理高度, 左上角为原点
 * \param width 纹理原始宽度
 * \param height 纹理原始高度
 */
void guiDrawRRTApplyTexture(GUIdrawrr drawrrt, GLuint texture, float x, float y, float w, float h, float width, float height);

/**
 * \brief 渲染圆角矩形
 * \param drawrrt 圆角矩形
 */
void guiDrawRRTRender(GUIdrawrr drawrrt);

/**
 * \brief 销毁圆角矩形
 * \param drawrrt 圆角矩形
 */
void guiDrawRRTDestroy(GUIdrawrr drawrrt);

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
 * \brief 渲染圆角矩形
 * \param drawrrc 圆角矩形
 */
void guiDrawRRCRender(GUIdrawrr drawrrc);

/**
 * \brief 销毁圆角矩形
 * \param drawrrc 圆角矩形
 */
void guiDrawRRCDestroy(GUIdrawrr drawrrc);

/**
 * \brief 生成高斯模糊的纹理
 * \param 
 */

#endif // GUI_DRAW_H