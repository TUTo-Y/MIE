#include "gui_ttf.h"

#define GUI_TTF_FONT_R 8     // 文字预留空间
#define GUI_TTF_FONT_INIT 32 // 初始化文字数量
#define GUI_TTF_FONT_ADD 64  // 每次添加文字数量

/**
 * \brief 初始化TTF字体
 * \param fontData TTF字体数据(二进制数据)
 * \param ... 字号, 0表示结束
 * \return GUIttf TTF字体
 * \note GUIttf *ttf = guiTTFCreate(const unsigned char *fontData, 16, 32, 64, 0); // 初始化字号16, 32, 64
 */
GUIttf *guiTTFCreate(const unsigned char *fontData, ...)
{
    GUIttf *ttf = (GUIttf *)malloc(sizeof(GUIttf));

    /* 读取字体信息 */
    if (!stbtt_InitFont(&ttf->fontInfo, fontData, stbtt_GetFontOffsetForIndex(fontData, 0)))
    {
        ERROR("字体初始化失败\n");
        free(ttf);
        return NULL;
    }

    /* 计算要初始化字号的数量 */
    va_list args;
    va_start(args, fontData);
    ttf->fontCount = 0;
    while (va_arg(args, int))
        ttf->fontCount++;
    va_end(args);

    /* 分配空间 */
    ttf->fontList = (GUIfont *)malloc(sizeof(GUIfont) * ttf->fontCount);

    /* 初始化字号 */
    va_start(args, fontData);
    for (int i = 0; i < ttf->fontCount; i++)
    {
        GUIfont *font = &ttf->fontList[i];

        /* 初始化字体信息 */
        font->pixels = va_arg(args, int);                                                     // 设置字号
        font->scale = stbtt_ScaleForPixelHeight(&ttf->fontInfo, font->pixels);                // 计算缩放比例
        stbtt_GetFontVMetrics(&ttf->fontInfo, &font->ascent, &font->descent, &font->lineGap); // 获取字体高度
        font->ascent *= font->scale;
        font->descent *= font->scale;
        font->lineGap *= font->scale;

        // 初始化文本列表
        font->textMax = GUI_TTF_FONT_INIT;
        font->textCount = 0;
        font->textList = (wchar_t *)malloc(sizeof(wchar_t) * font->textMax);
        font->textRend = (GUIchar *)malloc(sizeof(GUIchar) * font->textMax);

        // 清空文本列表
        memset(font->textList, 0, sizeof(wchar_t) * font->textMax);
        memset(font->textRend, 0, sizeof(GUIchar) * font->textMax);

        // 设置文本列表
        font->textList[0] = L'\0';
    }
    va_end(args);

    return ttf;
}


/**
 * \brief 从Font中创建一个文字
 * \param ttf TTF字体
 * \param font 字号列表
 * \param text 文本内容
 * \return GUIchar 单个文字
 */
GUIchar *guiTTFCreateCharFromFont(GUIttf *ttf, GUIfont *font, const wchar_t text)
{
    if (!font)
        return NULL;

    /* 添加文字 */
    font->textList[font->textCount] = text;
    GUIchar *ttfChar = &font->textRend[font->textCount];
    font->textCount++;

    /* 检查文本列表是否已满 */
    if (font->textCount >= font->textMax - GUI_TTF_FONT_R)
    {
        // 扩展列表
        font->textMax += GUI_TTF_FONT_ADD;
        font->textList = (wchar_t *)realloc(font->textList, sizeof(wchar_t) * font->textMax);
        font->textRend = (GUIchar *)realloc(font->textRend, sizeof(GUIchar) * font->textMax);

        // 清空新扩展的列表
        memset(font->textList + font->textCount, 0, sizeof(wchar_t) * GUI_TTF_FONT_ADD);
        memset(font->textRend + font->textCount, 0, sizeof(GUIchar) * GUI_TTF_FONT_ADD);
    }
    font->textList[font->textCount] = L'\0';

    /* 设置文字信息 */
    ttfChar->text = text;
    ttfChar->ascent = font->ascent;
    ttfChar->descent = font->descent;
    ttfChar->lineGap = font->lineGap;

    /* 获取字体宽度 */
    stbtt_GetCodepointHMetrics(&ttf->fontInfo, text, &ttfChar->advance, &ttfChar->x);
    ttfChar->advance *= font->scale;
    ttfChar->x *= font->scale;

    /* 创建纹理 */
    unsigned char *data = stbtt_GetCodepointBitmap(&ttf->fontInfo, font->scale, font->scale, text, &ttfChar->w, &ttfChar->h, &ttfChar->x, &ttfChar->y);
    ttfChar->y = -ttfChar->y;
    ttfChar->texture = guiTextureCreate(data, ttfChar->w, ttfChar->h, 1, GL_RED);
    stbtt_FreeBitmap(data, 0);

    /* 设置VAO, VBO, EBO */
    float vertices[] = {
        // 位置             // 纹理坐标
        (float)(ttfChar->x), (float)(ttfChar->y), 0.0f, 0.0f,
        (float)(ttfChar->x + ttfChar->w), (float)(ttfChar->y), 1.0f, 0.0f,
        (float)(ttfChar->x + ttfChar->w), (float)(ttfChar->y - ttfChar->h), 1.0f, 1.0f,
        (float)(ttfChar->x), (float)(ttfChar->y - ttfChar->h), 0.0f, 1.0f};
    const GLuint index[] = {
        0, 1, 2,
        0, 2, 3};
    glGenVertexArrays(1, &ttfChar->VAO);
    glGenBuffers(1, &ttfChar->VBO);
    glGenBuffers(1, &ttfChar->EBO);

    glBindBuffer(GL_ARRAY_BUFFER, ttfChar->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ttfChar->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glBindVertexArray(ttfChar->VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return ttfChar;
}

/**
 * \brief 创建一个文字
 * \param ttf TTF字体
 * \param text 文本内容
 * \param pixels 字号
 * \return GUIchar 单个文字
 */
GUIchar *guiTTFCreateChar(GUIttf *ttf, const wchar_t text, int pixels)
{
    GUIfont *font = NULL; // 对应字号的字体
    bool isFind = false;  // 是否找到字号

    /* 查找字 */
    for (int i = 0; i < ttf->fontCount; i++)
    {
        font = &ttf->fontList[i];
        if (font->pixels == pixels)
        {
            /* 查找文字是否存在 */
            wchar_t *c = wcschr(font->textList, text);
            if (c)
            {
                return &font->textRend[c - font->textList];
            }

            isFind = true;
            break;
        }
    }

    /* 没有找到字号 */
    if (!isFind)
    {
        ERROR("字号 %d 不存在\n", pixels);
        return NULL;
    }

    /* 创建文字 */
    return guiTTFCreateCharFromFont(ttf, font, text);
}

/**
 * \brief 渲染一个文字
 * \param c 文字
 */
void guiTTFRenderChar(GUIchar *c)
{
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c->texture);      // 绑定纹理
    glBindVertexArray(c->VAO);                     // 绑定VAO
    glBindBuffer(GL_ARRAY_BUFFER, c->VBO);         // 绑定VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->EBO); // 绑定EBO

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制
}

/**
 * \brief 清除TTF缓存
 */
void guiTTFDelete(GUIttf *ttf)
{
    for (int i = 0; i < ttf->fontCount; i++)
    {
        GUIfont *font = &ttf->fontList[i];
        for (int j = 0; j < font->textCount; j++)
        {
            GUIchar *c = &font->textRend[j];
            glDeleteTextures(1, &c->texture);
            glDeleteVertexArrays(1, &c->VAO);
            glDeleteBuffers(1, &c->VBO);
            glDeleteBuffers(1, &c->EBO);
        }
        free(font->textList);
        free(font->textRend);
    }
    free(ttf->fontList);
    free(ttf);
}