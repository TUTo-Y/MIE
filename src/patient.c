#include "patient.h"

static uint64_t key1 = 0;                    // 密钥1
static uint8_t key2[SM4_KEY_SIZE * 2] = {0}; // 密钥2

void patientChoiceBackFun(GUIid id, list *node)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;
    // 选中图片
    st->choice = node;
    GuiwidgetImgChoiceData *img = (GuiwidgetImgChoiceData *)node->data;

    guiWidgetImgSetTexture2(GUI_ID_PATIENT_IMG_IMG1, img->texture1, img->w, img->h); // 设置纹理
    guiWidgetImgSetTexture2(GUI_ID_PATIENT_IMG_IMG2, img->texture2, img->w, img->h); // 设置纹理
    SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)img->contant);                        // 设置文本框
}

// 患者按钮回调函数
void PatientButtonBackCall(GUIid id, size_t flag, void *data);

// 将图像加密并嵌入嵌入
void PatientEncEmbed(GuiwidgetImgChoiceData *img);

// 上传数据的线程
void *patientUploadThread(void *data);

// 等待医嘱下载的线程
void *patientWaitThread(void *data);

// 渲染任务
bool patientDrawTaskfun(size_t flag, void *data);

void patientInit()
{
    GUIstr *str;
    mat4 model;
    // 设置背景
    guiSetDefaultClearColor(0.95f, 0.9f, 0.85f, 1.0f);
    GUI_SET_DEFAULT_CLEARCOLOR();

    // 初始化key1和key2
    key1 = Random64();
    encSm4KeyIv(&key2[0], &key2[SM4_KEY_SIZE]);

    // 添加图片列表控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_IMG_CHOICE_LIST,
                          GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetImgChoiceFlag(GUI_ID_IMG_CHOICE_LIST, GUI_WIDGET_IMG_CHOICE_ADD | GUI_WIDGET_IMG_CHOICE_WASTE);
    guiWidgetImgChoiceSetChoiceFun(GUI_ID_IMG_CHOICE_LIST, patientChoiceBackFun);

    // 添加患者图像显示控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_IMG_IMG1,
                          GUI_WIDGET_CALLFLAG_DRAW,
                          800, 100, true);
    guiWidgetImgSetRect(GUI_ID_PATIENT_IMG_IMG1, (GUIrect){-200.0f, 416.0f, 300.0f, 300.0f});

    // 添加患者图像嵌入后显示控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_IMG_IMG2,
                          GUI_WIDGET_CALLFLAG_DRAW,
                          800, 100, true);
    guiWidgetImgSetRect(GUI_ID_PATIENT_IMG_IMG2, (GUIrect){-200.0f, 416.0f - 300.0f - 120.0f, 300.0f, 300.0f});

    // 添加患者选择图像控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_CHOICE_IMG, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_CHOICE_IMG, L"重新选择");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_CHOICE_IMG, (vec4){0xe4 / 255.0f, 0xf9 / 255.0f, 0xf5 / 255.0f, 1.0f}, (vec4){0x30 / 255.0f, 0xe3 / 255.0f, 0xca / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_CHOICE_IMG, -200.0f, 416.0f - 300.0f - 20.0f, 300.0f, 80.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_CHOICE_IMG, PatientButtonBackCall, 0, NULL);

    // 添加患者选择文本控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_CHOICE_TXET, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_CHOICE_TXET, L"选择文本");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_CHOICE_TXET, (vec4){0xe4 / 255.0f, 0xf9 / 255.0f, 0xf5 / 255.0f, 1.0f}, (vec4){0x30 / 255.0f, 0xe3 / 255.0f, 0xca / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_CHOICE_TXET, 250.0f, 416.0f - 300.0f - 20.0f, 300.0f, 80.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_CHOICE_TXET, PatientButtonBackCall, 0, NULL);

    // 添加患者嵌入文本控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, L"嵌入文本");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, (vec4){0xe4 / 255.0f, 0xf9 / 255.0f, 0xf5 / 255.0f, 1.0f}, (vec4){0x30 / 255.0f, 0xe3 / 255.0f, 0xca / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, -200.0f, 416.0f - 300.0f - 120.0f - 300.0f - 20.0f, 300.0f, 80.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, PatientButtonBackCall, 0, NULL);

    // 添加患者上传按钮控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, true);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_UPLOAD, L"上传数据");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_UPLOAD, (vec4){0xff / 255.0f, 0xaa / 255.0f, 0xa6 / 255.0f, 1.0f}, (vec4){0xff / 255.0f, 0x8c / 255.0f, 0x94 / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_UPLOAD, 250.0f, 416.0f - 300.0f - 120.0f - 60.0f, 300.0f, 200.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_UPLOAD, PatientButtonBackCall, 0, NULL);

    // 添加文本框控件
    hFont = CreateFont(
        18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE, // 添加凹陷边框
        L"EDIT",
        L"",
        WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        GL_POS_2_WINDOW_POS_x(250, WINDOW_WIDTH), GL_POS_2_WINDOW_POS_y(416, WINDOW_HEIGHT),
        300, 300,
        glfwGetWin32Window(guiIDGet(GUI_ID_WINDOW)->win.window),
        NULL,
        hInstance,
        NULL);
    hEdit2 = CreateWindowExW(
        WS_EX_CLIENTEDGE, // 添加凹陷边框
        L"EDIT",
        L"",
        WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
        GL_POS_2_WINDOW_POS_x(-350, WINDOW_WIDTH), GL_POS_2_WINDOW_POS_y(300, WINDOW_HEIGHT),
        700, 400,
        glfwGetWin32Window(guiIDGet(GUI_ID_WINDOW)->win.window),
        NULL,
        hInstance,
        NULL);
    SendMessageW(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessageW(hEdit2, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 显示控件
    ShowWindow(hEdit, SW_SHOW);
    ShowWindow(hEdit2, SW_HIDE);
    UpdateWindow(hEdit);
    UpdateWindow(hEdit2);

    // 等待控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    glm_translate_make(model, (vec3){0.0f, 0.0f, 0.0f});
    guiWidgetWaitSetModel(GUI_ID_WAIT, model);

    // GUI_ID_PATIENT_TEXT_WAIT1, // 患者等待医生接单文本
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_TEXT_WAIT1, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    str = guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_WAIT1);
    guiStrCpy(str, L"正在等待医生治疗...");
    glm_translate_make(model, (vec3){0.0f, 280.0f, 0.0f});
    guiStrSetModel(str, model);

    // GUI_ID_PATIENT_TEXT_ALLOW,    // 患者是否允许医生治疗文本
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_TEXT_ALLOW, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    str = guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_ALLOW);
    glm_translate_make(model, (vec3){0.0f, 200.0f, 0.0f});
    guiStrSetModel(str, model);

    // GUI_ID_PATIENT_BUTTON_ALLOW,  // 患者允许医生接单按钮ID
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_ALLOW, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, false);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_ALLOW, L"允许医生治疗");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_ALLOW, (vec4){0xe4 / 255.0f, 0xf9 / 255.0f, 0xf5 / 255.0f, 1.0f}, (vec4){0x30 / 255.0f, 0xe3 / 255.0f, 0xca / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_ALLOW, -100 - 240, -100, 240.0f, 120.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_ALLOW, PatientButtonBackCall, 0, NULL);

    // GUI_ID_PATIENT_BUTTON_REFUSE, // 患者拒绝医生接单按钮ID
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_REFUSE, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, false);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_REFUSE, L"不允许医生治疗");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_REFUSE, (vec4){0xff / 255.0f, 0xaa / 255.0f, 0xa6 / 255.0f, 1.0f}, (vec4){0xff / 255.0f, 0x8c / 255.0f, 0x94 / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_REFUSE, 100, -100, 240.0f, 120.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_REFUSE, PatientButtonBackCall, 0, NULL);

    // GUI_ID_PATIENT_TEXT_WAIT2, // 患者等待医生检查文本
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_TEXT_WAIT2, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    str = guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_WAIT2);
    guiStrCpy(str, L"正在等待医生上传医嘱...");
    glm_translate_make(model, (vec3){0.0f, 280.0f, 0.0f});
    guiStrSetModel(str, model);

    // GUI_ID_PATIENT_BUTTON_RESULT, // 患者查看检查结果按钮ID
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, false);
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_RESULT, L"确认");
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_RESULT, (vec4){0xe4 / 255.0f, 0xf9 / 255.0f, 0xf5 / 255.0f, 1.0f}, (vec4){0x30 / 255.0f, 0xe3 / 255.0f, 0xca / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_RESULT, -120, -150, 240.0f, 120.0f);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_RESULT, PatientButtonBackCall, 0, NULL);
}

void PatientEncEmbed(GuiwidgetImgChoiceData *img)
{
    if (img->img1)
        free(img->img1);
    if (img->img2)
        free(img->img2);
    if (img->m)
        free(img->m);

    // 使用key1打乱图像
    uint8_t *t = malloc(img->w * img->h);
    memcpy(t, img->data, img->w * img->h);
    rdhShuffleImage(t, img->w * img->h, key1);

    // 使用key2加密数据
    SM4_CBC_CTX sm4_key;
    uint8_t *ciphertext = (uint8_t *)malloc((wcslen(img->contant) + 1) * sizeof(wchar_t) + 0x20);
    memset(ciphertext, 0, (wcslen(img->contant) + 1) * sizeof(wchar_t) + 0x20);
    size_t size = 0;
    sm4_cbc_encrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
    sm4_cbc_encrypt_update(&sm4_key, (uint8_t *)img->contant, (wcslen(img->contant) + 1) * sizeof(wchar_t), ciphertext, &img->size);
    sm4_cbc_encrypt_finish(&sm4_key, ((uint8_t *)ciphertext) + img->size, &size);
    img->size += size;

    // debug
    DEBUG("加密前的文本长度:%d\n", wcslen(img->contant) + sizeof(wchar_t));
    wprintf(L"加密前的的文本:[%ls]\n\n", (wchar_t *)img->contant);
    uint8_t digest[SM3_DIGEST_SIZE];
    encHash((uint8_t *)ciphertext, img->size, digest);
    DEBUG("加密后数据的hash : ");
    putbin2hex(digest, sizeof(digest), stdout);
    DEBUG("\n");
    DEBUG("加密后的数据长度 : %ld\n", img->size);

    // 随机分割
    rdhSplitImage(t, img->w * img->h, &img->img1, &img->img2);

    // 嵌入数据
    rdhStatus ret = rdhEmbedData(img->img1, img->img2, img->w, img->h, &img->m, &img->mSize, ciphertext, img->size);
    assert(ret == RDH_SUCESS);

    // 释放内存
    free(ciphertext);
    free(t);
}

void PatientButtonBackCall(GUIid id, size_t flag, void *data)
{
    list *node = NULL;
    pthread_t thread; // 线程标识符

    switch (id)
    {
    // 重新选择图片
    case GUI_ID_PATIENT_BUTTON_CHOICE_IMG:
        // 选择文件
        node = ((GuiwidgetImgChoice *)GUI_ID2WIDGET(GUI_ID_IMG_CHOICE_LIST)->data1)->choice;
        if (node)
        {
            char *path = choiceImageFile();
            if (path != NULL)
            {
                // 创建图像数据
                GuiwidgetImgChoiceData *img = malloc(sizeof(GuiwidgetImgChoiceData));
                memset(img, 0, sizeof(GuiwidgetImgChoiceData));

                img->data = stbi_load(path, &img->w, &img->h, NULL, 1);
                img->texture = guiTextureCreate2Gary(img->data, img->w, img->h);
                img->texture1 = guiTextureCreate2Gary(img->data, img->w, img->h);

                img->img = guiDrawRRTCreate(128, 128, 10, 0.02);
                guiDrawRRTBindTexture(img->img, img->texture);

                img->contant = 0;

                // 设置图像
                guiWidgetImgSetTexture2(GUI_ID_PATIENT_IMG_IMG1, img->texture1, img->w, img->h);
                guiWidgetImgSetEmpty(GUI_ID_PATIENT_IMG_IMG2); // 设置空纹理

                // 添加到链表中
                guiWidgetImgChoiceFreeFun(node->data);
                node->data = img;

                // 消息
                guiWidgetLogAddMsg(GUI_ID_LOG, L"重新选择", GUI_WIDGET_LOG_MSG);
            }
            else
                free(path);
        }
        else
        {
            guiWidgetLogAddMsg(GUI_ID_LOG, L"未选择图像", GUI_WIDGET_LOG_WARN);
        }
        break;

    // 选择文本按钮
    case GUI_ID_PATIENT_BUTTON_CHOICE_TXET:

        char *path = choiceTextFile();
        if (path != NULL)
        {
            char *str = NULL;
            wchar_t *wstr = NULL;
            size_t len = 0;

            // 读取文件内容
            FILE *fp = fopen(path, "rb");
            if (fp == NULL)
            {
                guiWidgetLogAddMsg(GUI_ID_LOG, L"打开文件失败", GUI_WIDGET_LOG_ERROR);
                free(path);
                return;
            }

            // 获取文件大小
            fseek(fp, 0, SEEK_END);
            len = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            // 读取
            str = (char *)malloc(len + 1);
            wstr = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
            fread(str, 1, len, fp);
            str[len] = '\0'; // 添加字符串结束符

            mbstowcs(wstr, str, len + 1); // 转换为宽字符

            SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)wstr);

            free(str);
            free(wstr);
            free(path);
            fclose(fp);
        }

        break;

    // 嵌入文本按钮
    case GUI_ID_PATIENT_BUTTON_IMPLANT_TXET:
        // 选择文件
        node = ((GuiwidgetImgChoice *)GUI_ID2WIDGET(GUI_ID_IMG_CHOICE_LIST)->data1)->choice;
        if (node)
        {
            GuiwidgetImgChoiceData *img = node->data;
            if (img->contant)
                free(img->contant);
            if (img->texture2)
                guiWidgetImgSetEmpty(GUI_ID_PATIENT_IMG_IMG2);

            // 将文本框读取到 img->contant
            int length = GetWindowTextLengthW(hEdit);
            img->contant = (wchar_t *)malloc((length + 1) * sizeof(wchar_t));
            GetWindowTextW(hEdit, img->contant, length + 1); // 获取文本框内容

            PatientEncEmbed(img); // 嵌入数据

            // 创建纹理
            uint8_t *t;
            rdhCombineImage(img->img1, img->img2, img->w * img->h, &t);
            img->texture2 = guiTextureCreate2Gary(t, img->w, img->h);
            guiWidgetImgSetTexture2(GUI_ID_PATIENT_IMG_IMG2, img->texture2, img->w, img->h); // 设置纹理
            free(t);

            // 设置嵌入后图像
            guiWidgetLogAddMsg(GUI_ID_LOG, L"嵌入成功", GUI_WIDGET_LOG_MSG);
        }
        else
        {
            guiWidgetLogAddMsg(GUI_ID_LOG, L"未选择图像", GUI_WIDGET_LOG_WARN);
        }
        break;

    // 上传数据按钮
    case GUI_ID_PATIENT_BUTTON_UPLOAD:

        if (!((GuiwidgetImgChoice *)GUI_ID2WIDGET(GUI_ID_IMG_CHOICE_LIST)->data1)->imgList.count)
        {
            guiWidgetLogAddMsg(GUI_ID_LOG, L"未选择图像", GUI_WIDGET_LOG_WARN);
            break;
        }
        // 禁用组件
        guiWidgetSetControl(GUI_ID_IMG_CHOICE_LIST, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_IMG_IMG1, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_IMG_IMG2, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE_IMG, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE_TXET, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, false);

        ShowWindow(hEdit, SW_HIDE);
        UpdateWindow(hEdit);

        // 启用组件
        guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_TEXT_WAIT1, GUI_WIDGET_CALLFLAG_ALL, true);

        // 创建线程来上传数据
        pthread_create(&thread, NULL, patientUploadThread, NULL);
        pthread_detach(thread);
        break;

        // 允许治疗
    case GUI_ID_PATIENT_BUTTON_ALLOW:
        webSendFlag(sockfd, WEB_MSG_PAITENT_YES); // 发送允许治疗的消息

        // 禁用控件
        guiWidgetSetControl(GUI_ID_PATIENT_TEXT_ALLOW, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_ALLOW, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_REFUSE, GUI_WIDGET_CALLFLAG_ALL, false);

        // 启用控件
        guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_TEXT_WAIT2, GUI_WIDGET_CALLFLAG_ALL, true);

        // 创建线程等待医生上传医嘱
        pthread_create(&thread, NULL, patientWaitThread, NULL);
        pthread_detach(thread);

        break;

        // 不允许治疗
    case GUI_ID_PATIENT_BUTTON_REFUSE:
        webSendFlag(sockfd, WEB_MSG_PAITENT_NO); // 发送允许治疗的消息
        break;

        // 确认
    case GUI_ID_PATIENT_BUTTON_RESULT:

        // 禁用控件
        ShowWindow(hEdit2, SW_HIDE);
        UpdateWindow(hEdit2);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_ALL, false);

        // 启用控件
        guiWidgetSetControl(GUI_ID_IMG_CHOICE_LIST, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_IMG_IMG1, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_IMG_IMG2, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE_IMG, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE_TXET, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_IMPLANT_TXET, GUI_WIDGET_CALLFLAG_ALL, true);
        guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);
        ShowWindow(hEdit, SW_SHOW);
        UpdateWindow(hEdit);
        break;
    }
}

// 上传数据的线程
void *patientUploadThread(void *data)
{
    ENCkem kem;
    // 加密和嵌入所有图像
    list *l = &((GuiwidgetImgChoice *)GUI_ID2WIDGET(GUI_ID_IMG_CHOICE_LIST)->data1)->imgList;

    // 向服务器发送上传数据的请求
    webSendFlag(sockfd, WEB_MSG_PAITENT_UPLOAD); // 发送消息

    // debug
    DEBUG("向服务器发送的key1 : ");
    putbin2hex((uint8_t *)&key1, sizeof(key1), stdout);
    DEBUG("\n");
    DEBUG("向服务器发送的key2 : ");
    putbin2hex((uint8_t *)&key2, sizeof(key2), stdout);
    DEBUG("\n");

    // 发送密钥
    uint8_t key_pack[sizeof(uint64_t) + SM4_KEY_SIZE * 2];
    memcpy(key_pack, &key1, sizeof(uint64_t));
    memcpy(key_pack + sizeof(uint64_t), key2, SM4_KEY_SIZE * 2);
    kem = encKEMEnc(key_pack, sizeof(key_pack), &keyServer);
    webSendData(sockfd, (char *)kem, encKEMSizeKEM(kem));

    // debug
    DEBUG("向服务器发送的图像数量 : %d\n", l->count);

    // 发送图像数量
    webSendFlag(sockfd, l->count); // 发送图像数量

    // 向服务器发送图像
    list *node = l->bk;
    while (node != l)
    {
        PatientEncEmbed(node->data); // 嵌入数据
        GuiwidgetImgChoiceData *img = node->data;

        // debug
        uint8_t digest[SM3_DIGEST_SIZE];
        DEBUG("图像宽高 : %d %d\n", img->w, img->h);
        encHash(img->img1, img->w * img->h, digest);
        DEBUG("图像1哈希值 : ");
        putbin2hex(digest, sizeof(digest), stdout);
        DEBUG("\n");
        encHash(img->img2, img->w * img->h, digest);
        DEBUG("图像2哈希值 : ");
        putbin2hex(digest, sizeof(digest), stdout);
        DEBUG("\n");
        encHash(img->m, img->mSize, digest);
        DEBUG("m哈希值 : ");
        putbin2hex(digest, sizeof(digest), stdout);
        DEBUG("\n\n");

        // 发送图像宽高
        webSendFlag(sockfd, (FLAG)img->w); // 发送w
        webSendFlag(sockfd, (FLAG)img->h); // 发送h

        // 发送图像数据
        webSendData(sockfd, img->img1, img->w * img->h); // 发送图像数据
        webSendData(sockfd, img->img2, img->w * img->h); // 发送图像数据

        // 发送图像嵌入的数据大小
        webSendFlag(sockfd, img->size); // 发送图像嵌入的数据大小

        // 发送m
        webSendData(sockfd, img->m, img->mSize); // 发送m

        // 发送mSize
        webSendFlag(sockfd, img->mSize); // 发送mSize

        node = node->bk;
    }

    // 等待服务端消息
    while (webRecvFlag(sockfd) != WEB_MSG_PAITENT_MSG)
        ;

    // 接受医生姓名并渲染设置文本
    char *doctor_name = webRecvData(sockfd, NULL, NULL);
    guiTaskAddTask(patientDrawTaskfun, 0, doctor_name);

    // 禁用控件
    guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, false);
    guiWidgetSetControl(GUI_ID_PATIENT_TEXT_WAIT1, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    guiWidgetSetControl(GUI_ID_PATIENT_TEXT_ALLOW, GUI_WIDGET_CALLFLAG_ALL, true);
    guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_ALLOW, GUI_WIDGET_CALLFLAG_ALL, true);
    guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_REFUSE, GUI_WIDGET_CALLFLAG_ALL, true);
}

bool patientDrawTaskfun(size_t flag, void *data)
{
    char *doctor_name = data;
    wchar_t *wstr = (wchar_t *)malloc((strlen(doctor_name) + 1) * sizeof(wchar_t));
    mbstowcs(wstr, doctor_name, strlen(doctor_name) + 1); // 转换为宽字符
    GUIstr *str = guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_ALLOW);
    guiStrCpy(str, L"是否允许 ");
    guiStrCat(str, wstr);
    guiStrCat(str, L" 查看病例？");
    free(wstr);
    free(doctor_name);
}

void *patientWaitThread(void *data)
{
    // 等待服务器消息
    while (webRecvFlag(sockfd) != WEB_MSG_PAITENT_MSG)
        ;

    // 接受医嘱并设置文本框
    ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    wchar_t *doctor_advice = (wchar_t *)encKEMDec(kem, &keyClient);
    SendMessageW(hEdit2, WM_SETTEXT, 0, (LPARAM)doctor_advice); // 设置文本框
    wprintf(L"医嘱 : %ls 长度 : %d\n", doctor_advice, wcslen(doctor_advice));

    encKEMFree(kem);
    free(doctor_advice);

    // 禁用控件
    guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, false);
    guiWidgetSetControl(GUI_ID_PATIENT_TEXT_WAIT2, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    ShowWindow(hEdit2, SW_SHOW);
    UpdateWindow(hEdit2);
    guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_ALL, true);
}

// bool patientChoiceImage(size_t wh, void *data)
// {

//     // 删除患者加号界面
//     guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BIGCHOICE);

//     // // 设置控件
//     // guiWidgetImgSetTexture(GUI_ID_PATIENT_IMG, img, width, height);

//     // 启用控件
//     guiWidgetSetControl(GUI_ID_PATIENT_IMG, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);

//     return true;
// }

// void patientButtonChoiceBackcall(GUIid id, size_t flag, void *data)
// {
//     // 选择图片
//     char *img_str = choiceImageFile();
//     if (!img_str)
//     {
//         guiWidgetLogAddMsg(GUI_ID_LOG, L"未选择图片", GUI_WIDGET_LOG_WARN);
//         return;
//     }

//     // 读取图片
//     int width, height, channels;
//     unsigned char *img = stbi_load(img_str, &width, &height, &channels, 1);
//     if (!img)
//     {
//         guiWidgetLogAddMsg(GUI_ID_LOG, L"加载图片失败", GUI_WIDGET_LOG_ERROR);
//         free(img_str);
//         return;
//     }

//     // 选择图片
//     guiWidgetImgSetTexture(GUI_ID_PATIENT_IMG, img, width, height);

//     // 释放内存
//     free(img_str);
// }

// void patientButtonUploadBackcall(GUIid id, size_t flag, void *data)
// {
//     guiWidgetLogAddMsg(GUI_ID_LOG, L"上传图像", GUI_WIDGET_LOG_WARN);

//     // 禁用文本框的消息回调函数
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_EVENT, false);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_EVENT, false);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_EVENT, false);

//     // 禁用按钮
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, false);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, false);

//     // 启用等待控件
//     guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);

//     // 保存数据
//     GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(GUI_ID_PATIENT_IMG)->data1;
//     upload.img = (char *)malloc(st->w * st->h);
//     memcpy(upload.img, st->data, st->w * st->h);
//     upload.w = st->w;
//     upload.h = st->h;
//     packInit(&upload.pack);
//     packSetName(&upload.pack, ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_PATIENT_INPUT_NAME)->data1)->textplain);
//     packSetAge(&upload.pack, ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_PATIENT_INPUT_AGE)->data1)->textplain);
//     packSetState(&upload.pack, ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_PATIENT_INPUT_STATE)->data1)->textplain);

//     // 创建线程用来上传数据
//     pthread_t thread; // 线程标识符
//     pthread_create(&thread, NULL, patientUploadThread, NULL);
//     pthread_detach(thread);
// }

// // 任务函数
// void *patientUploadThread(void *data)
// {
//     /* 生成图像加密密钥key1 */
//     uint64_t key1 = rand();

//     /* 生成图像加密密钥key2 */
//     uint8_t key2[SM4_KEY_SIZE * 2];
//     encSm4KeyIv(&key2[0], &key2[SM4_KEY_SIZE]);

//     // test
//     DEBUG("key1 : ");
//     putbin2hex((uint8_t *)&key1, sizeof(key1), stdout);
//     DEBUG("");

//     DEBUG("key2 : ");
//     putbin2hex((uint8_t *)key2, sizeof(key2), stdout);
//     DEBUG("");

//     /* 加密数据信息数据 */
//     PACKbuf packbuf;
//     size_t packlen;
//     size_t packlen2;
//     SM4_CBC_CTX sm4_key;
//     sm4_cbc_encrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
//     sm4_cbc_encrypt_update(&sm4_key, (uint8_t *)&upload.pack, sizeof(PACK), (uint8_t *)&packbuf, &packlen);
//     sm4_cbc_encrypt_finish(&sm4_key, ((char *)&packbuf) + packlen, &packlen2);
//     packlen += packlen2;

//     /* 使用key1打乱图像 */
//     rdhShuffleImage((uint8_t *)upload.img, upload.w * upload.h, key1);

//     /* 分割图像 */
//     uint8_t *img1, *img2;
//     rdhSplitImage((uint8_t *)upload.img, upload.w * upload.h, &img1, &img2);

//     /* 嵌入数据 */
//     uint8_t *m = NULL;
//     int mSize = 0;
//     if (rdhEmbedData(img1, img2, upload.w, upload.h, &m, &mSize, (uint8_t *)&packbuf, packlen) == RDH_ERROR)
//     {
//         guiWidgetLogAddMsg(GUI_ID_LOG, L"嵌入数据失败", GUI_WIDGET_LOG_ERROR);
//         return NULL;
//     }

//     /* 向服务器发送上传数据的请求 */
//     webSendFlag(sockfd, WEB_MSG_PAITENT_UPLOAD); // 发送消息

//     /* 打包密钥并使用kem加密后发送 */
//     uint8_t key_pack[sizeof(uint64_t) + SM4_KEY_SIZE * 2];
//     memcpy(key_pack, &key1, sizeof(uint64_t));
//     memcpy(key_pack + sizeof(uint64_t), key2, SM4_KEY_SIZE * 2);
//     ENCkem kem = encKEMEnc(key_pack, sizeof(key_pack), &keyServer);
//     webSendData(sockfd, (char *)kem, encKEMSizeKEM(kem));

//     // 上传图像
//     webSendFlag(sockfd, (((size_t)upload.w) << 32) | upload.h); // 发送wh
//     webSendData(sockfd, img1, upload.w * upload.h);             // 上传图像
//     webSendData(sockfd, img2, upload.w * upload.h);             // 上传图像
//     free(img1);
//     free(img2);

//     // 上传m
//     webSendFlag(sockfd, mSize);
//     webSendData(sockfd, m, mSize);
//     free(m);

//     // 释放冗余数据
//     encKEMFree(kem);
//     free(upload.img);

//     /* 开始监听模式, 直到选择 */
//     while (webRecvFlag(sockfd) != WEB_MSG_PAITENT_MSG)
//     {
//     }

//     /* 添加任务让用户确认 */
//     kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
//     uint8_t *name = encKEMDec(kem, &keyClient);
//     guiTaskAddTask(patientCheckDoctorTask, 0, name);

//     return NULL;
// }

// bool patientCheckDoctorTask(size_t flag, void *data)
// {
//     // 禁用等待状态
//     guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, false);

//     // 启用控件
//     wchar_t name[0x40];
//     wcscpy(name, L"是否允许");
//     mbstowcs(name + wcslen(name), data, strlen(data));
//     wcscat(name, L"检查?");
//     guiStrCpy(guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_CHECK), name);
//     guiWidgetSetControl(GUI_ID_PATIENT_TEXT_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON1_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON2_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
//     free(data);

//     return true;
// }

// // 允许
// void patientCheckDoctorBackcall(GUIid id, size_t flag, void *data)
// {
//     // 禁用控件
//     guiWidgetSetControl(GUI_ID_PATIENT_TEXT_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON1_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON2_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);

//     if (flag) // 允许
//     {
//         // 告诉服务器允许上传
//         webSendFlag(sockfd, WEB_MSG_PAITENT_YES);

//         // 启用控件
//         guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, true);

//         // 创建等待线程
//         pthread_t thread;
//         pthread_create(&thread, NULL, patientWaitMsgThread, NULL);
//         pthread_detach(thread);
//     }
//     else // 不允许
//     {
//         // 告诉服务器不允许上传
//         webSendFlag(sockfd, WEB_MSG_PAITENT_NO);

//         // 文本框的消息
//         guiWidgetSetControl(GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_EVENT, true);
//         guiWidgetSetControl(GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_EVENT, true);
//         guiWidgetSetControl(GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_EVENT, true);

//         // 按钮
//         guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);
//         guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, true);
//     }
// }
// void *patientWaitMsgThread(void *t)
// {
//     // 接受数据
//     ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
//     uint8_t *data = encKEMDec(kem, &keyClient);
//     size_t data_size = encKEMSizeText(kem);
//     memcpy(&upload.pack.advice, data, data_size);

//     // 释放数据
//     encKEMFree(kem);
//     free(data);

//     // 添加控件启动
//     guiTaskAddTask(patientAdvice, 0, NULL);

//     return NULL;
// }

// bool patientAdvice(size_t flag, void *data)
// {
//     // 禁用控件
//     guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, false);

//     // 设置控件
//     wchar_t advice[0x50];
//     wcscpy(advice, L"医生建议: ");
//     wcscat(advice, upload.pack.advice);
//     guiStrCpy(guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_ADVICE), advice);

//     // 启用控件
//     guiWidgetSetControl(GUI_ID_PATIENT_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_ADVICE, GUI_WIDGET_CALLFLAG_ALL, true);

//     return true;
// }

// void patientOver(GUIid id, size_t flag, void *data)
// {
//     // 禁用控件
//     guiWidgetSetControl(GUI_ID_PATIENT_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, false);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_ADVICE, GUI_WIDGET_CALLFLAG_ALL, false);

//     // 启用控件
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_EVENT, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_EVENT, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_EVENT, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);
//     guiWidgetSetControl(GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, true);
// }