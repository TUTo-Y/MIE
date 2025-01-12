#include "gui_str.h"

#define GUI_SYT_R 8     // 字符串预留空间
#define GUI_SYT_INIT 32 // 字符串初始大小
#define GUI_STR_ADD 64  // 字符串每次增加的大小

/**
 * \brief 检查字符串是否够写count个字符
 * \param str 字符串
 * \param count 字符数量
 */
void guiStrCheck(GUIstr *str, size_t count)
{
    size_t size = str->count + count + GUI_SYT_R;
    if (str->count >= str->countMax - size)
    {
        str->countMax += size > GUI_STR_ADD ? size : GUI_STR_ADD;
    }
}

/**
 * \brief 设置字符串矩阵
 * \param str 字符串
 * \param model 模型矩阵, 为NULL则不更新
 * \param view 视图矩阵, 为NULL则不更新
 * \param projection 投影矩阵, 为NULL则不更新
 * \param appMat 是否更新着色器中的model, view和projection矩阵数据
 */
void guiStrSetMat(GUIstr *str, mat4 model, mat4 view, mat4 projection, bool appMat)
{
    if (str == NULL)
        return;
    if (model)
        glm_mat4_copy(model, str->model);
    if (view)
        glm_mat4_copy(view, str->view);
    if (projection)
        glm_mat4_copy(projection, str->projection);
    if (appMat == true)
    {
        guiShaderUse(str->program);
        guiShaderUniformMatrix(str->program, "model", 4fv, (float *)str->model);
        guiShaderUniformMatrix(str->program, "view", 4fv, (float *)str->view);
        guiShaderUniformMatrix(str->program, "projection", 4fv, (float *)str->projection);
    }
}

/**
 * \brief 设置字符串文本颜色
 * \param str 字符串
 * \param color 颜色
 * \param appColor 是否更新着色器中的颜色数据
 */
void guiStrSetColor(GUIstr *str, vec4 color, bool appColor)
{
    if (str == NULL)
        return;
    if (color)
        glm_vec4_copy(color, str->color);
    if (appColor == true)
    {
        guiShaderUse(str->program);
        guiShaderUniform(str->program, "color", 4f, str->color[0], str->color[1], str->color[2], str->color[3]);
    }
}

/**
 * \brief 创建一个字符串
 * \param ttf 字体
 * \param pixels 字号
 * \param program 渲染器
 * \param model 模型矩阵
 * \param view 视图矩阵
 * \param projection 投影矩阵
 * \return
 * \return GUIstr* 字符串
 */
GUIstr *guiStrCreate(GUIttf *ttf, int pixels, GLuint program, mat4 model, mat4 view, mat4 projection)
{
    GUIstr *str = (GUIstr *)malloc(sizeof(GUIstr));

    str->ttf = ttf;
    str->pixels = pixels;
    str->program = program;

    str->count = 0;
    str->countMax = GUI_SYT_INIT;
    str->strText = (wchar_t *)malloc(sizeof(wchar_t) * str->countMax);
    str->strChar = (GUIchar **)malloc(sizeof(GUIchar *) * str->countMax);

    memset(str->strText, 0, sizeof(wchar_t) * str->countMax);
    memset(str->strChar, 0, sizeof(GUIchar *) * str->countMax);

    str->strText[0] = L'\0';

    // 设置矩阵
    guiStrSetMat(str, model, view, projection, true);
    // 设置颜色
    guiStrSetColor(str, (vec4){0.0f, 0.0f, 0.0f, 1.0f}, true);

    return str;
}

/**
 * \brief 设置字符串文本
 * \param str 字符串
 * \param text 文本
 */
void guiStrCpy(GUIstr *str, const wchar_t *text)
{
    size_t textSize = wcslen(text);
    guiStrCheck(str, textSize);

    for (size_t i = 0; i < textSize; i++)
    {
        str->strText[i] = text[i];
        str->strChar[i] = guiTTFCreateChar(str->ttf, text[i], str->pixels);
    }

    str->strText[textSize] = L'\0';
    str->count = textSize;
}

/**
 * \brief 追加字符串文本
 * \param str 字符串
 * \param text 文本
 */
void guiStrCat(GUIstr *str, const wchar_t *text)
{
    size_t textSize = wcslen(text);
    guiStrCheck(str, textSize);

    for (size_t i = 0; i < textSize; i++)
    {
        str->strText[str->count + i] = text[i];
        str->strChar[str->count + i] = guiTTFCreateChar(str->ttf, text[i], str->pixels);
    }

    str->strText[str->count + textSize] = L'\0';
    str->count += textSize;
}

/**
 * \brief 追加字符串字符
 * \param str 字符串
 * \param text 字符
 */
void guiStrCatC(GUIstr *str, const wchar_t text)
{
    guiStrCheck(str, 1);

    str->strText[str->count] = text;
    str->strChar[str->count] = guiTTFCreateChar(str->ttf, text, str->pixels);

    str->strText[str->count + 1] = L'\0';
    str->count++;
}

/**
 * \brief 删除最后一个字符
 * \param str 字符串
 */
void guiStrBack(GUIstr *str)
{
    if (str->count == 0)
        return;

    str->count--;
    str->strText[str->count] = L'\0';
}

/**
 * \brief 删除N个字符
 * \param str 字符串
 * \param n 删除字符数量
 */
void guiStrBackN(GUIstr *str, int n)
{
    if (str->count == 0)
        return;

    if (n > str->count)
        n = str->count;

    str->count -= n;
    str->strText[str->count] = L'\0';
}

/**
 * \brief 渲染字符串
 * \param str 字符串
 * \param model 模型矩阵
 * \param appMat 是否更新矩阵数据
 * \param appColor 是否更新颜色数据
 * \note 对于view和projection矩阵，需要在调用此函数之前设置
 */
void guiStrRender(GUIstr *str, mat4 model, bool appMat, bool appColor)
{
    if (str == NULL)
        return;

    guiShaderUse(str->program);

    mat4 m;
    glm_mat4_copy(str->model, m);

    if (model)
        glm_mat4_mul(model, m, m);

    if (appMat == true)
    {
        // guiShaderUniformMatrix(str->program, "model", 4fv, (float *)str->model);
        guiShaderUniformMatrix(str->program, "view", 4fv, (float *)str->view);
        guiShaderUniformMatrix(str->program, "projection", 4fv, (float *)str->projection);
    }
    if (appColor == true)
    {
        guiShaderUniform(str->program, "color", 4f, str->color[0], str->color[1], str->color[2], str->color[3]);
    }

    for (size_t i = 0; i < str->count; i++)
    {
        guiShaderUniformMatrix(str->program, "model", 4fv, (float *)m);
        guiTTFRenderChar(str->strChar[i]);
        glm_translate(m, (vec3){str->strChar[i]->advance, 0.0f, 0.0f});
    }
}

/**
 * \brief 删除字符串
 */
void guiStrDelete(GUIstr *str)
{
    free(str->strText);
    free(str->strChar);
    free(str);
}