#include "gui_fb.h"

GUIfb guiFbCreate(int width, int height)
{
    GUIfb fb = (GUIfb)malloc(sizeof(GUIfb_t));
    fb->width = width;
    fb->height = height;

    // 创建帧缓冲
    glGenFramebuffers(1, &fb->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);

    // // 生成纹理
    // fb->texture = guiTextureCreateEmpty(width, height, GL_RGBA);

    // 创建渲染缓冲对象
    glGenRenderbuffers(1, &fb->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // 将渲染缓冲对象附加到帧缓冲的深度和模板附件上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);

    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("帧缓冲创建失败:帧缓冲不完整!");
        glDeleteFramebuffers(1, &fb->fbo);
        glDeleteRenderbuffers(1, &fb->rbo);
        free(fb);
        return NULL;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fb;
}


void guiFbBindTexture(GUIfb fb, GLuint texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void guiFbStart(GUIfb fb)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    glViewport(0, 0, fb->width, fb->height);
}

void guiFbEnd(GUIfb fb)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GUI_SET_DEFAULT_VIEWPORT();
}

void guiFbDelete(GUIfb fb)
{
    glDeleteFramebuffers(1, &fb->fbo);
    glDeleteRenderbuffers(1, &fb->rbo);
    free(fb);
}