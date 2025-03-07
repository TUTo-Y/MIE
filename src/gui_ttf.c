#include "gui_ttf.h"

#define GUI_TTF_FONT_R 3                         // 文字预留空间
#define GUI_TTF_FONT_INIT 0x20                   // 初始化文字数量
#define GUI_TTF_FONT_ADD (GUI_TTF_FONT_INIT * 2) // 每次添加文字数量

GUIttf *font_default = NULL;

GUIchar *guiCharCreate(GUIttf *ttf, GUIfont *font, wchar_t text)
{
    if (!font && !ttf)
        return NULL;

    /* 添加文字 */
    GUIchar *ttfChar;
    font->textList[font->textCount] = text;
    ttfChar = &font->textRend[font->textCount++];
    font->textList[font->textCount] = L'\0';

    /* 检查文本列表是否已满 */
    if (font->textCount >= font->textMax - GUI_TTF_FONT_R)
    {
        // 扩展列表
        font->textMax += GUI_TTF_FONT_ADD;
        font->textList = (wchar_t *)realloc(font->textList, sizeof(wchar_t) * font->textMax);
        font->textRend = (GUIchar *)realloc(font->textRend, sizeof(GUIchar) * font->textMax);

        // 清空新扩展的列表
        memset(&font->textList[font->textCount], 0, sizeof(wchar_t) * (font->textMax - font->textCount));
        memset(&font->textRend[font->textCount], 0, sizeof(GUIchar) * (font->textMax - font->textCount));
    }

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
    GLuint index[] = {
        0, 1, 2,
        0, 2, 3};
    glGenVertexArrays(1, &ttfChar->VAO);
    glGenBuffers(1, &ttfChar->VBO);
    glGenBuffers(1, &ttfChar->EBO);
    glBindVertexArray(ttfChar->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ttfChar->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ttfChar->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return ttfChar;
}

GUIchar *guiCharGetFromFont(GUIttf *ttf, GUIfont *font, wchar_t text)
{
    if (!font && !ttf)
        return NULL;

    /* 查找文字是否存在 */
    wchar_t *c = wcschr(font->textList, text);
    if (c)
        return &font->textRend[c - font->textList];

    /* 创建文字 */
    return guiCharCreate(ttf, font, text);
}

GUIchar *guiCharGetFromTTF(GUIttf *ttf, wchar_t text, int pixels)
{
    if (!ttf)
        return NULL;

    /* 查找字号 */
    GUIfont *font; // 对应字号的字体
    font = guiFontGet(ttf, pixels);
    if (!font)
        return NULL;

    /* 根据字号查找字 */
    wchar_t *c = wcschr(font->textList, text);
    if (c)
        return &font->textRend[c - font->textList];

    /* 没有找到字就创建字 */
    return guiCharCreate(ttf, font, text);
}

void guiCharRender(GUIchar *c)
{
    glBindTexture(GL_TEXTURE_2D, c->texture);            // 绑定纹理
    glBindVertexArray(c->VAO);                           // 绑定VAO
    glBindBuffer(GL_ARRAY_BUFFER, c->VBO);               // 绑定VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->EBO);       // 绑定EBO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制
}

void guiFontInit(GUIttf *ttf, GUIfont *font, int pixels)
{
    /* 初始化字体信息 */
    font->pixels = pixels;                                                                // 设置字号
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
}

GUIfont *guiFontGet(GUIttf *ttf, int pixels)
{
    // 查找字号
    for (int i = 0; i < ttf->fontCount; i++)
    {
        GUIfont *font = &ttf->fontList[i];
        if (font->pixels == pixels)
        {
            return font;
        }
    }
#if 0
    // 没有找到字号则添加
    ttf->fontCount++;
    ttf->fontList = (GUIfont *)realloc(ttf->fontList, sizeof(GUIfont) * ttf->fontCount);
    guiFontInit(ttf, &ttf->fontList[ttf->fontCount - 1], pixels);

    return &ttf->fontList[ttf->fontCount - 1];
#else
    ERROR("不允许新添加字号\n");
    return NULL;
#endif
}

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
        guiFontInit(ttf, &ttf->fontList[i], va_arg(args, int));
    va_end(args);

    return ttf;
}

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