#include "gui_str.h"

#define GUI_STR_R 0x10                 // 字符串预留空间
#define GUI_STR_INIT 0x20              // 字符串初始大小
#define GUI_STR_ADD (GUI_STR_INIT * 2) // 字符串每次增加的大小

/**
 * \brief 检查字符串是否需要更新修复矩阵
 */
#define guiStrCheckMode(str) \
    if ((str)->mode >> 1)    \
    {                        \
        guiStrAppMode(str);  \
    }

/**
 * \brief 检查字符串是否需要扩展
 * \param str 字符串
 * \param count 需要增加的字符数量
 */
void guiStrCheckLen(GUIstr *str, size_t count)
{
    size_t size = str->count + count + GUI_STR_R;
    if (size >= str->countMax)
    {
        // 扩展字符串
        str->countMax += ((size - str->countMax) > GUI_STR_ADD ? (size - str->countMax) : GUI_STR_ADD);
        str->strText = (wchar_t *)realloc(str->strText, sizeof(wchar_t) * str->countMax);
        str->strChar = (GUIchar **)realloc(str->strChar, sizeof(GUIchar *) * str->countMax);

        memset(str->strText + str->count, 0, sizeof(wchar_t) * (str->countMax - str->count));
        memset(str->strChar + str->count, 0, sizeof(GUIchar *) * (str->countMax - str->count));
    }
}
#if 1
void guiStrRender(GUIstr *str)
{
    if (str == NULL)
        return;

    guiStrUseProgram(str);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);

    // 应用模型矩阵
    guiStrAppModel(str);

    // 应用颜色矩阵
    guiStrAppColor(str);

    // 拷贝修复矩阵
    mat4 fix;
    glm_mat4_copy(guiStrGetFix(str), fix);

    for (size_t i = 0; i < str->count; i++)
    {
        guiStrAppFix(str, fix);
        guiCharRender(str->strChar[i]);
        glm_translate(fix, (vec3){str->strChar[i]->advance, 0.0f, 0.0f});
    }
}
#else
void guiStrAppMode(GUIstr *str)
{
    GUIfont *font = str->font;

    float x = 0.0f; // x坐标
    float y = 0.0f; // y坐标
    float w = 0.0f; // 宽度
    float h = 0.0f; // 高度

    for (size_t i = 0; i < str->count; i++)
    {
        w += str->strChar[i]->advance;
    }
    h = font->ascent - font->descent;
    y = font->ascent;

    glm_mat4_identity(guiStrGetFix(str));
    switch (str->mode)
    {
    case GUI_STR_MOD_LEFT: // 左对齐
        glm_translate(guiStrGetFix(str), (vec3){0.0f, (float)h / 2.0f - (float)y, 0.0f});
        break;
    case GUI_STR_MOD_CENTER: // 居中对齐
        glm_translate(guiStrGetFix(str), (vec3){-(float)w / 2.0f, (float)h / 2.0f - (float)y, 0.0f});
        break;
    case GUI_STR_MOD_RIGHT: // 右对齐
        glm_translate(guiStrGetFix(str), (vec3){-(float)w, (float)h / 2.0f - (float)y, 0.0f});
        break;
    case GUI_STR_MOD_LEFT_TOP: // 左上对其
        glm_translate(guiStrGetFix(str), (vec3){0.0f, -(float)y, 0.0f});
        break;
    }
}

#endif

void guiStrAppMode(GUIstr *str)
{
    GUIfont *font = str->font;

    float x = 0.0f; // x坐标
    float y = 0.0f; // y坐标
    float w = 0.0f; // 宽度
    float h = 0.0f; // 高度

    for (size_t i = 0; i < str->count; i++)
    {
        w += str->strChar[i]->advance;
    }

    glm_mat4_identity(guiStrGetFix(str));
    switch (str->mode)
    {
    case GUI_STR_MOD_LEFT: // 左对齐
        glm_translate(guiStrGetFix(str), (vec3){0.0f, font->descent, 0.0f});
        break;
    case GUI_STR_MOD_CENTER: // 居中对齐
        glm_translate(guiStrGetFix(str), (vec3){-(float)w / 2.0f, font->descent, 0.0f});
        break;
    case GUI_STR_MOD_CENTER2: // 完全居中对齐
        glm_translate(guiStrGetFix(str), (vec3){-(float)w / 2.0f, -(font->ascent - font->descent) / 2.0f, 0.0f});
        break;
    case GUI_STR_MOD_RIGHT: // 右对齐
        glm_translate(guiStrGetFix(str), (vec3){-(float)w, font->descent, 0.0f});
        break;
    case GUI_STR_MOD_LEFT_TOP: // 左上对其
        glm_translate(guiStrGetFix(str), (vec3){0.0f, font->descent - font->ascent, 0.0f});
        break;
    }
}

GUIstr *guiStrCreate(GUIttf *ttf, int pixels, int mode, GLuint program, mat4 model, vec4 color)
{
    GUIstr *str = (GUIstr *)malloc(sizeof(GUIstr));

    str->ttf = ttf;
    str->font = guiFontGet(ttf, pixels);
    str->program = program;
    str->count = 0;
    str->countMax = GUI_STR_INIT;
    str->strText = (wchar_t *)malloc(sizeof(wchar_t) * str->countMax);
    str->strChar = (GUIchar **)malloc(sizeof(GUIchar *) * str->countMax);
    memset(str->strText, 0, sizeof(wchar_t) * str->countMax);
    memset(str->strChar, 0, sizeof(GUIchar *) * str->countMax);
    str->strText[0] = L'\0';

    guiStrUseProgram(str);

    // 设置uniform位置
    str->fixLoc = guiShaderUniformGetLocation(str->program, "fix");
    str->modelLoc = guiShaderUniformGetLocation(str->program, "model");
    str->colorLoc = guiShaderUniformGetLocation(str->program, "color");

    // 设置各个矩阵
    if (model) // 设置模型矩阵
        guiStrSetModel(str, model);
    else
        glm_mat4_identity(str->model);
    if (color) // 设置颜色
        guiStrSetColor(str, color);
    else
        glm_vec4_copy((vec4){0.0f, 0.0f, 0.0f, 1.0f}, str->color);

    // 设置修复矩阵
    guiStrSetMode(str, mode);
    guiStrAppMode(str);

    // 设置纹理
    guiShaderUniform(str->program, "Texture", 1i, 0);

    return str;
}

void guiStrDelete(GUIstr *str)
{
    free(str->strText);
    free(str->strChar);
    free(str);
}

void guiStrCpy(GUIstr *str, const wchar_t *text)
{
    size_t textSize = wcslen(text);
    guiStrCheckLen(str, textSize);

    for (size_t i = 0; i < textSize; i++)
    {
        str->strText[i] = text[i];
        str->strChar[i] = guiCharGetFromFont(str->ttf, str->font, text[i]);
    }

    str->strText[textSize] = L'\0';
    str->count = textSize;

    guiStrCheckMode(str);
}

void guiStrCat(GUIstr *str, const wchar_t *text)
{
    size_t textSize = wcslen(text);
    guiStrCheckLen(str, textSize);

    for (size_t i = 0; i < textSize; i++)
    {
        str->strText[str->count + i] = text[i];
        str->strChar[str->count + i] = guiCharGetFromFont(str->ttf, str->font, text[i]);
    }

    str->strText[str->count + textSize] = L'\0';
    str->count += textSize;

    guiStrCheckMode(str);
}

void guiStrCatC(GUIstr *str, const wchar_t text)
{
    guiStrCheckLen(str, 1);

    str->strText[str->count] = text;
    str->strChar[str->count] = guiCharGetFromFont(str->ttf, str->font, text);

    str->strText[str->count + 1] = L'\0';
    str->count++;

    guiStrCheckMode(str);
}

void guiStrBack(GUIstr *str)
{
    if (str->count == 0)
        return;

    str->count--;
    str->strText[str->count] = L'\0';

    guiStrCheckMode(str);
}

void guiStrBackN(GUIstr *str, int n)
{
    if (str->count == 0)
        return;

    if (n > str->count)
        n = str->count;

    str->count -= n;
    str->strText[str->count] = L'\0';

    guiStrCheckMode(str);
}
