#include "gui_texture.h"

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
GLuint guiTextureCreate(const unsigned char *data, int width, int height, int channels, GLint textureFormat)
{
    GLenum format;

    // 确定颜色通道数
    switch (channels)
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        ERROR("无效的颜色通道数:%d\n", channels);
        return 0;
    }

    // 输出颜色通道
    textureFormat = textureFormat == 0 ? format : textureFormat;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 设置字节对齐方式
    if (format == GL_RED)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

GLuint guiTextureCreateEmpty(int width, int height, GLint textureFormat)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    return texture;
}