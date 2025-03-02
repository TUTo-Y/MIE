/**
 * @file gui_fb.h
 * @brief GUI界面的帧缓冲相关函数
 */
#ifndef GUI_FB_H
#define GUI_FB_H

#include <glad.h>
#include <glfw/glfw3.h>

#include "log.h"
#include "gui.h"
#include "gui_texture.h"

typedef struct GUIfb_t
{
    GLuint fbo;
    GLuint rbo;
    int width;
    int height;
}GUIfb_t;
typedef GUIfb_t *GUIfb;

/**
 * \brief 创建帧缓冲
 * \param width 宽度
 * \param height 高度
 * \return GUIfb 帧缓冲对象
 */
GUIfb guiFbCreate(int width, int height);

/**
 * \brief 绑定纹理
 * \param fb 帧缓冲对象
 * \param texture 纹理ID
 */
void guiFbBindTexture(GUIfb fb, GLuint texture);

/**
 * \brief 开始绘制
 * \param fb 帧缓冲对象
 */
void guiFbStart(GUIfb fb);

/**
 * \brief 结束绘制
 * \param fb 帧缓冲对象
 */
void guiFbEnd(GUIfb fb);

/**
 * \brief 删除帧缓冲
 * \param fb 帧缓冲对象
 */
void guiFbDelete(GUIfb fb);


#endif // GUI_FB_H