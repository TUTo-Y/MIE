/**
 * @file guitexture.h
 * @brief 创建纹理
 */
#ifndef GUI_TEXTURE_H
#define GUI_TEXTURE_H

#include <stdio.h>
#include <stdbool.h>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

/**
 * @brief 创建纹理
 * @param data 图像数据
 * @param width 图像宽度
 * @param height 图像高度
 * @param channels 图像通道数
 * @param textureFormat 输出格式(GL_RGB, GL_RGBA等), 默认为0
 * @return GLuint 纹理ID
 * @note 该函数会创建一个纹理并返回纹理ID
 */
GLuint guiTextureCreate(const unsigned char *data, int width, int height, int channels, GLint textureFormat);

#endif // GUI_TEXTURE_H