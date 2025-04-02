#include "gui_widget_img_choice.h"

GUIrect rect[5] = {
    {-500.0 - 64.0, (64.0 + 128.0 * 2 + 32.0 * 2), 128.0, 128.0},
    {-500.0 - 64.0, (64.0 + 128.0 * 1 + 32.0 * 1), 128.0, 128.0},
    {-500.0 - 64.0, (64.0 + 128.0 * 0 + 32.0 * 0), 128.0, 128.0},
    {-500.0 - 64.0, (64.0 + 128.0 * -1 + 32.0 * -1), 128.0, 128.0},
    {-500.0 - 64.0, (64.0 + 128.0 * -2 + 32.0 * -2), 128.0, 128.0}};
GUIrect rect2 = {-596, 416, 192, 732};
// GUIrect rect2 = {-500.0 - (32 + 128 + 32) / 2.0, (32 + 128 * 5 + 32 * 4 + 32) / 2.0, 32 + 128 + 32, 32 + 128 * 5 + 32 * 4 + 32};

void guiWidgetImgChoiceFreeFun(void *data)
{
    GuiwidgetImgChoiceData *img = (GuiwidgetImgChoiceData *)data;
    if (img->img != 0)
        guiDrawRRTDestroy(img->img);
    if (img->texture != 0)
        glDeleteTextures(1, &img->texture);
    if (img->texture1 != 0)
        glDeleteTextures(1, &img->texture1);
    if (img->texture2 != 0)
        glDeleteTextures(1, &img->texture2);

    if (img->data != NULL)
        stbi_image_free(img->data);
    if (img->contant != NULL)
        free(img->contant);

    free(data);
}

// 创建和销毁调用函数
void gui_widget_img_choice_registerCall(GUIid id)
{
    GUI_ID2WIDGET(id)->data1 = malloc(sizeof(GuiwidgetImgChoice));
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;
    memset(st, 0, sizeof(GuiwidgetImgChoice));

    int w, h, n;
    void *data;

    st->flag = 0;

    // 加载边框
    st->frame = guiDrawRRCCreate(32 + 128 + 32, 32 + 128 * 5 + 32 * 4 + 32, 10, 0.02, (vec4){0.95f, 0.97f, 0.95f, 1.0f});
    mat4 model;
    glm_translate_make(model, (vec3){-500.0f, 0.0f, 0.0f});
    guiDrawRRCSetModel(st->frame, model);

    // 加载垃圾桶图标
    data = stbi_load(config.waste_icon_path, &w, &h, &n, 4);
    st->wasteIconTexture = guiTextureCreate(data, w, h, 4, GL_RGBA);
    st->wasteIcon = guiDrawIconCreate(64, 64, st->wasteIconTexture, (vec4){1.0f, 0.1f, 0.1f, 1.0f});
    stbi_image_free(data);

    // 加载添加图标
    data = stbi_load(config.add_icon_path, &w, &h, &n, 4);
    st->addIconTexture = guiTextureCreate(data, w, h, 4, GL_RGBA);
    st->addIcon = guiDrawIconCreate(128, 128, st->addIconTexture, (vec4){0.0f, 0.5f, 0.5f, 1.0f});
    stbi_image_free(data);

    // 初始化图片列表
    listInitList(&st->imgList);

    st->choice = NULL;
    st->hover = -1;
}
void gui_widget_img_choice_logoffCall(GUIid id)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;

    // 销毁边框
    guiDrawRRCDestroy(st->frame);

    // 销毁图标
    guiDrawIconDestroy(st->wasteIcon);
    guiDrawIconDestroy(st->addIcon);
    glDeleteTextures(1, &st->wasteIconTexture);
    glDeleteTextures(1, &st->addIconTexture);

    // 销毁链表
    listDeleteList(&st->imgList, guiWidgetImgChoiceFreeFun);
}

// 渲染函数
bool gui_widget_img_choice_drawCall(GUIid id)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;

    // 渲染边框
    guiDrawRRCRender(st->frame);

    mat4 model;
    glm_translate_make(model, (vec3){-500.0f, (128 * 2 + 32 * 2), 0.0f});

    // 渲染所有图片
    list *node = st->imgList.bk;
    int count = 0;
    while (node != &st->imgList)
    {
        GuiwidgetImgChoiceData *img = (GuiwidgetImgChoiceData *)node->data;
        guiDrawRRTSetModel(img->img, model);
        guiDrawRRTRender(img->img);

        // 渲染垃圾桶图标
        if (count == st->hover && GUI_FLAG_CHECK(st->flag, GUI_WIDGET_IMG_CHOICE_WASTE))
        {
            mat4 model2;
            glm_mat4_copy(model, model2);
            glm_translate(model2, (vec3){32, 32, 0.0f});
            guiDrawIconSetModel(st->wasteIcon, model2);
            guiDrawIconRender(st->wasteIcon);
        }

        glm_translate(model, (vec3){0, -(128 + 32), 0.0f});
        node = node->bk;
        count++;
    }

    // 渲染加号图标
    if (GUI_FLAG_CHECK(st->flag, GUI_WIDGET_IMG_CHOICE_ADD) && (st->imgList.count < 5))
    {
        guiDrawIconSetModel(st->addIcon, model);
        guiDrawIconRender(st->addIcon);
    }

    return true;
}
bool gui_widget_img_choice_eventCall(GUIid id, const GUIevent *event)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;

    bool in[5] = {false, false, false, false, false};
    for (int i = 0; i < 5; i++)
    {
        in[i] = guiCheckPosInRect(&rect[i], event->mousePos.x, event->mousePos.y);
    }

    switch (event->type)
    {

    case GUI_EVENT_TYPE_CURSOR_POS:
        // 减少范围
        if (guiCheckPosInRect(&rect2, event->mousePos.x, event->mousePos.y))
        {
            st->hover = -1;
            for (int i = 0; i < 5; i++)
            {
                if (in[i] == true)
                {
                    st->hover = i;
                    break;
                }
            }
        }

        break;
    case GUI_EVENT_TYPE_MOUSE_BUTTON:
        // 按下
        if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT && event->MouseButton.action == GLFW_PRESS &&
            guiCheckPosInRect(&rect2, event->mousePos.x, event->mousePos.y))
        {
            GUI_FLAG_SET(st->flag, GUI_WIDGET_IMG_PRESS);
        }

        // 松开
        if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT && event->MouseButton.action == GLFW_RELEASE &&
            guiCheckPosInRect(&rect2, event->mousePos.x, event->mousePos.y) && GUI_FLAG_CHECK(st->flag, GUI_WIDGET_IMG_PRESS))
        {
            GUI_FLAG_UNSET(st->flag, GUI_WIDGET_IMG_PRESS);

            // 检查是否点击了某个图片
            list *node = st->imgList.bk;
            int count = 0;
            while (node != &st->imgList)
            {
                if (in[count++] == true)
                {
                    // 检查是否删除图片
                    GUIrect r = rect[count - 1];
                    r.x += 64.0;
                    r.y += 0;
                    r.width = 64.0;
                    r.height = 64.0;
                    if (guiCheckPosInRect(&r, event->mousePos.x, event->mousePos.y) && GUI_FLAG_CHECK(st->flag, GUI_WIDGET_IMG_CHOICE_WASTE))
                    {
                        // 如果这个图片被选中
                        if (st->choice == node)
                        {
                            guiWidgetImgSetEmpty(GUI_ID_PATIENT_IMG_IMG1);   // 设置空纹理
                            guiWidgetImgSetEmpty(GUI_ID_PATIENT_IMG_IMG2);   // 设置空纹理
                            SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)L""); // 清空文本框

                            st->choice = NULL;
                        }
                        listDeleteNode(&st->imgList, node, guiWidgetImgChoiceFreeFun);
                        guiWidgetLogAddMsg(GUI_ID_LOG, L"删除图片", GUI_WIDGET_LOG_MSG);
                    }
                    else
                    {
                        CALL(st->choiceFun, id, node); // 调用回调函数
                    }

                    break;
                }
                node = node->bk;
            }

            // 如果没有点击图片，检查是否点击了添加按钮(需要有添加权限)
            if (in[count] == true && GUI_FLAG_CHECK(st->flag, GUI_WIDGET_IMG_CHOICE_ADD))
            {
                // 选择文件
                char *path = choiceImageFile();
                if (path != NULL)
                {
                    // 创建图像数据
                    GuiwidgetImgChoiceData *img = malloc(sizeof(GuiwidgetImgChoiceData));
                    memset(img, 0, sizeof(GuiwidgetImgChoiceData));

                    img->data = stbi_load(path, &img->w, &img->h, NULL, 1);
                    img->texture = guiTextureCreate2Gary(img->data, img->w, img->h);
                    img->texture1 = guiTextureCreate2Gary(img->data, img->w, img->h);
                    img->texture2 = guiTextureCreateEmpty2();
                    assert(img->texture != 0);
                    assert(img->texture1 != 0);
                    assert(img->texture2 != 0);
                    img->contant = (wchar_t *)malloc(sizeof(wchar_t));
                    *img->contant = L'\0';

                    img->img = guiDrawRRTCreate(128, 128, 10, 0.02);
                    guiDrawRRTBindTexture(img->img, img->texture);

                    guiWidgetImgSetTexture2(GUI_ID_PATIENT_IMG_IMG1, img->texture1, img->w, img->h); // 设置纹理
                    guiWidgetImgSetTexture2(GUI_ID_PATIENT_IMG_IMG2, img->texture2, img->w, img->h); // 设置纹理
                    SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)img->contant);                        // 清空文本框

                    // 添加到链表中
                    st->choice = listDataToNode(listCreateNode(), img, 0, false);
                    listAddNodeInEnd(&st->imgList, st->choice);

                    // 消息
                    guiWidgetLogAddMsg(GUI_ID_LOG, L"添加图片", GUI_WIDGET_LOG_MSG);
                }
                else
                    free(path);
            }
        }

        break;
    }

    return true;
}

// 回调函数
void gui_widget_img_choice_loadCall(GUIid id)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;
}
void gui_widget_img_choice_uploadCall(GUIid id)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;
}

// 清空图像列表
void guiWidgetImgChoiceDelete(GUIid id)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;
    // 销毁链表
    listDeleteList(&st->imgList, guiWidgetImgChoiceFreeFun);
    st->hover = -1;
    st->choice = NULL;
}
