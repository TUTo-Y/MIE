#include "doctor.h"

static uint64_t key1 = 0;                    // 密钥1
static uint8_t key2[SM4_KEY_SIZE * 2] = {0}; // 密钥2
static bool upload = false;                  // 是否允许上传

void doctorChoiceBackFun(GUIid id, list *node)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(id)->data1;
    // 选中图片
    st->choice = node;
    GuiwidgetImgChoiceData *img = (GuiwidgetImgChoiceData *)node->data;

    guiWidgetImgSetTexture2(GUI_ID_DOCTOR_IMG_IMG, img->texture, img->w, img->h); // 设置纹理
    SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)img->contant);                     // 设置文本框
}

// 医生等待线程
void *doctorWaitThread(void *data);
void *doctorWait2Thread(void *data);

// 渲染任务
bool doctorDrawTaskfun(size_t flag, void *data);

// 医生按钮
void DoctorButtonBackCall(GUIid id, size_t flag, void *data);

void doctorInit()
{
    GUIstr *str;
    mat4 model;

    guiSetDefaultClearColor(0.85f, 0.9f, 0.95f, 1.0f);
    GUI_SET_DEFAULT_CLEARCOLOR();

    // 添加图片列表控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_IMG_CHOICE_LIST,
                          GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, false);
    guiWidgetImgChoiceFlag(GUI_ID_IMG_CHOICE_LIST, 0);
    guiWidgetImgChoiceSetChoiceFun(GUI_ID_IMG_CHOICE_LIST, doctorChoiceBackFun);

    // 两个文本框
    hFont = CreateFont(
        18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE, // 添加凹陷边框
        L"EDIT",
        L"",
        WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
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
        WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        GL_POS_2_WINDOW_POS_x(-200, WINDOW_WIDTH), GL_POS_2_WINDOW_POS_y(416 - 300 - 120, WINDOW_HEIGHT),
        750, 200,
        glfwGetWin32Window(guiIDGet(GUI_ID_WINDOW)->win.window),
        NULL,
        hInstance,
        NULL);
    SendMessageW(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessageW(hEdit2, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 不显示控件
    ShowWindow(hEdit, SW_HIDE);
    ShowWindow(hEdit2, SW_HIDE);
    UpdateWindow(hEdit);
    UpdateWindow(hEdit2);

    // 等待控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, true);
    glm_translate_make(model, (vec3){0.0f, 0.0f, 0.0f});
    guiWidgetWaitSetModel(GUI_ID_WAIT, model);

    // 等待文本
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_WAIT1, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, true);
    str = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_WAIT1);
    guiStrCpy(str, L"正在等待患者...");
    glm_translate_make(model, (vec3){0.0f, 280.0f, 0.0f});
    guiStrSetModel(str, model);

    // 图像控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_IMG_IMG,
                          GUI_WIDGET_CALLFLAG_DRAW,
                          800, 100, false);
    guiWidgetImgSetRect(GUI_ID_DOCTOR_IMG_IMG, (GUIrect){-200.0f, 416.0f, 300.0f, 300.0f});

    // 医嘱提示文本
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    str = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_ADVICE);
    guiStrSetMode(str, GUI_STR_MOD_LEFT_TOP);
    guiStrCpy(str, L"请输入医嘱:");
    glm_scale_make(model, (vec3){0.5f, 0.5f, 0.0f});
    glm_translate(model, (vec3){-200.0f - 50.0f, 416 - 300 - 60, 0.0f});
    guiStrSetModel(str, model);

    // 解密按钮控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, false);
    guiWidgetButtonSetText(GUI_ID_DOCTOR_BUTTON_RESULT, L"解密病历");
    guiWidgetButtonSetColor(GUI_ID_DOCTOR_BUTTON_RESULT, (vec4){0x02 / 255.0f, 0xAA / 255.0f, 0xB0 / 255.0f, 1.0f}, (vec4){0x00 / 255.0f, 0xCD / 255.0f, 0xAC / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_DOCTOR_BUTTON_RESULT, -200.0f, 416.0f - 300.0f - 120.0f - 200.0f - 20.0f, 300.0f, 80.0f);
    guiWidgetButtonSetBackCall(GUI_ID_DOCTOR_BUTTON_RESULT, DoctorButtonBackCall, 0, NULL);

    // 上传按钮控件
    guiWidgetAddToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                          800, 100, false);
    guiWidgetButtonSetText(GUI_ID_DOCTOR_BUTTON_UPLOAD, L"上传医嘱");
    guiWidgetButtonSetColor(GUI_ID_DOCTOR_BUTTON_UPLOAD, (vec4){0x02 / 255.0f, 0xAA / 255.0f, 0xB0 / 255.0f, 1.0f}, (vec4){0x00 / 255.0f, 0xCD / 255.0f, 0xAC / 255.0f, 1.0f});
    guiWidgetButtonSetPos(GUI_ID_DOCTOR_BUTTON_UPLOAD, 250.0f, 416.0f - 300.0f - 120.0f - 200.0f - 20.0f, 300.0f, 80.0f);
    guiWidgetButtonSetBackCall(GUI_ID_DOCTOR_BUTTON_UPLOAD, DoctorButtonBackCall, 0, NULL);

    // 进入医生的Recv模式
    pthread_t thread;
    pthread_create(&thread, NULL, doctorWaitThread, NULL);
    pthread_detach(thread);
}

void *doctorWaitThread(void *data)
{
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(GUI_ID_IMG_CHOICE_LIST)->data1;
    ENCkem kem;

    // 告诉服务器进入医生的Recv模式
    webSendFlag(sockfd, WEB_MSG_DOCTOR_WAIT);

    // 清空所有信息
    memset(&key1, 0, sizeof(key1));
    memset(&key2, 0, sizeof(key2));
    guiWidgetImgChoiceDelete(GUI_ID_IMG_CHOICE_LIST); // 删除所有图像
    // 清空文本框
    SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)L"");  // 设置文本框
    SendMessageW(hEdit2, WM_SETTEXT, 0, (LPARAM)L""); // 设置文本框

    // 不许上传
    upload = false;

    // 等待服务器的消息
    while (webRecvFlag(sockfd) != WEB_MSG_DOCTOR_MSG)
        ;

    // 接受key1
    kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    uint8_t *key = encKEMDec(kem, &keyClient);
    key1 = *(uint64_t *)key;
    encKEMFree(kem);

    // debug
    DEBUG("key1 : ");
    putbin2hex((uint8_t *)&key1, sizeof(key1), stdout);
    DEBUG("\n\n");

    // 接受图像数量
    size_t count = webRecvFlag(sockfd);

    // 依次接受图像
    for (int i = 0; i < count; i++)
    {
        // 创建图像数据
        GuiwidgetImgChoiceData *img = malloc(sizeof(GuiwidgetImgChoiceData));
        memset(img, 0, sizeof(GuiwidgetImgChoiceData));

        // 接受图像宽高
        img->w = (int)webRecvFlag(sockfd);
        img->h = (int)webRecvFlag(sockfd);

        // debug
        DEBUG("图像宽高 : %d %d\n", img->w, img->h);

        // 接受图像12
        img->img1 = (uint8_t *)webRecvData(sockfd, NULL, NULL);
        img->img2 = (uint8_t *)webRecvData(sockfd, NULL, NULL);
        img->size = webRecvFlag(sockfd);
        img->m = (uint8_t *)webRecvData(sockfd, NULL, NULL);
        img->mSize = (int)webRecvFlag(sockfd);

        // debug
        uint8_t digest[SM3_DIGEST_SIZE];
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
        DEBUG("\n");

        // 使用key1解密图像
        rdhCombineImage((uint8_t *)img->img1, (uint8_t *)img->img2, img->w * img->h, (uint8_t **)&img->data);
        rdhUnshuffleImage((uint8_t *)img->data, img->w * img->h, key1);

        // 提取图像数据信息
        assert(RDH_SUCESS == rdhExtractData(img->img1, img->img2, img->w, img->h, img->m, img->mSize, (uint8_t **)&img->contant));

        // debug
        encHash((uint8_t *)img->contant, img->size, digest);
        DEBUG("加密文本的hash : ");
        putbin2hex(digest, sizeof(digest), stdout);
        DEBUG("\n\n");

        // 添加渲染任务
        guiTaskAddTask(doctorDrawTaskfun, 0, img);

        // 添加到链表中
        st->choice = listDataToNode(listCreateNode(), img, 0, false);
        listAddNodeInEnd(&st->imgList, st->choice);
    }

    // 禁用控件
    guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);
    guiWidgetSetControl(GUI_ID_DOCTOR_TEXT_WAIT1, GUI_WIDGET_CALLFLAG_DRAW, false);

    // 启用控件
    guiWidgetSetControl(GUI_ID_IMG_CHOICE_LIST, GUI_WIDGET_CALLFLAG_ALL, true);
    guiWidgetSetControl(GUI_ID_DOCTOR_IMG_IMG, GUI_WIDGET_CALLFLAG_ALL, true);
    guiWidgetSetControl(GUI_ID_DOCTOR_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_DRAW, true);
    guiWidgetSetControl(GUI_ID_DOCTOR_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_ALL, true);
    guiWidgetSetControl(GUI_ID_DOCTOR_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);
    ShowWindow(hEdit, SW_SHOW);
    ShowWindow(hEdit2, SW_SHOW);
    UpdateWindow(hEdit);
    UpdateWindow(hEdit2);

    return NULL;
}

bool doctorDrawTaskfun(size_t flag, void *data)
{
    GuiwidgetImgChoiceData *img = data;

    // 创建纹理
    img->texture = guiTextureCreate2Gary(img->data, img->w, img->h);
    assert(img->texture != 0);

    img->img = guiDrawRRTCreate(128, 128, 10, 0.02);
    guiDrawRRTBindTexture(img->img, img->texture);

    guiWidgetImgSetTexture2(GUI_ID_DOCTOR_IMG_IMG, img->texture, img->w, img->h); // 设置纹理
    SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)img->contant);                     // 设置文本框

    return true;
}

void DoctorButtonBackCall(GUIid id, size_t flag, void *data)
{
    pthread_t thread;
    switch (id)
    {
        // 获取key2
    case GUI_ID_DOCTOR_BUTTON_RESULT:
        // 只能获取一次，禁用按钮事件响应
        guiWidgetSetControl(GUI_ID_DOCTOR_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_EVENT, false);
        guiWidgetLogAddMsg(GUI_ID_LOG, L"正在请求key2", GUI_WIDGET_LOG_MSG);

        // 启用线程等待用户发送key2
        pthread_create(&thread, NULL, doctorWait2Thread, NULL);
        pthread_detach(thread);

        break;

        // 上传
    case GUI_ID_DOCTOR_BUTTON_UPLOAD:
        if (upload == false)
        {
            guiWidgetLogAddMsg(GUI_ID_LOG, L"请先请求key2", GUI_WIDGET_LOG_WARN);
            break;
        }

        // 发送医嘱
        size_t doctorAdviceLen = GetWindowTextLengthW(hEdit2);
        wchar_t *doctorAdvice = (wchar_t *)malloc((doctorAdviceLen + 1) * sizeof(wchar_t));
        GetWindowTextW(hEdit2, doctorAdvice, doctorAdviceLen + 1); // 获取文本框内容
        doctorAdvice[doctorAdviceLen] = L'\0';                     // 添加结束符

        ENCkem kem = encKEMEnc((uint8_t *)doctorAdvice, (doctorAdviceLen + 1) * sizeof(wchar_t), &keyServer);
        webSendData(sockfd, (char *)kem, encKEMSizeKEM(kem)); // 发送加密的医嘱
        encKEMFree(kem);

        free(doctorAdvice);
        guiWidgetLogAddMsg(GUI_ID_LOG, L"上传医嘱成功", GUI_WIDGET_LOG_MSG);

        // 禁用控件
        guiWidgetSetControl(GUI_ID_IMG_CHOICE_LIST, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_DOCTOR_IMG_IMG, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_DOCTOR_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_DRAW, false);
        guiWidgetSetControl(GUI_ID_DOCTOR_BUTTON_RESULT, GUI_WIDGET_CALLFLAG_ALL, false);
        guiWidgetSetControl(GUI_ID_DOCTOR_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, false);
        ShowWindow(hEdit, SW_HIDE);
        ShowWindow(hEdit2, SW_HIDE);
        UpdateWindow(hEdit);
        UpdateWindow(hEdit2);

        // 启用控件
        guiWidgetSetControl(GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);
        guiWidgetSetControl(GUI_ID_DOCTOR_TEXT_WAIT1, GUI_WIDGET_CALLFLAG_DRAW, true);

        // 创建线程
        pthread_create(&thread, NULL, doctorWaitThread, NULL);
        pthread_detach(thread);
        break;
    }
}

void *doctorWait2Thread(void *data)
{
    webSendFlag(sockfd, WEB_MSG_DOCTOR_MSG); // 发送请求key2的消息

    // 等待服务器的消息
    while (webRecvFlag(sockfd) != WEB_MSG_DOCTOR_MSG)
        ;

    // 接受key2
    ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    uint8_t *key = encKEMDec(kem, &keyClient);
    memcpy(key2, key, sizeof(key2));
    encKEMFree(kem);
    free(key);

    // debug
    DEBUG("key2 : ");
    putbin2hex(key2, sizeof(key2), stdout);
    DEBUG("\n\n");

    // 使用key2解密content
    GuiwidgetImgChoice *st = (GuiwidgetImgChoice *)GUI_ID2WIDGET(GUI_ID_IMG_CHOICE_LIST)->data1;
    list *node = st->imgList.bk;
    while (node != &st->imgList)
    {
        GuiwidgetImgChoiceData *img = (GuiwidgetImgChoiceData *)node->data;

        void *ciphertext = img->contant;
        size_t ciphertext_size = img->size;
        img->contant = malloc(img->size);
        memset(img->contant, 0, img->size);

        // debug
        uint8_t digest[SM3_DIGEST_SIZE];
        encHash((uint8_t *)ciphertext, img->size, digest);
        DEBUG("解密前的数据的hash : ");
        putbin2hex(digest, sizeof(digest), stdout);
        DEBUG("\n");
        DEBUG("解密前的数据长度 : %ld\n", img->size);

        // 使用key2解密数据
        SM4_CBC_CTX sm4_key;
        size_t size = 0;
        sm4_cbc_decrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
        sm4_cbc_decrypt_update(&sm4_key, ciphertext, img->size, (uint8_t *)img->contant, &img->size);
        sm4_cbc_decrypt_finish(&sm4_key, ((uint8_t *)img->contant) + img->size, &size);
        img->size += size;
        
        // debug
        DEBUG("解密出来的文本长度:%d\n", img->size);
        wprintf(L"解密出来的文本:[%ls]\n\n", (wchar_t *)img->contant);

        // 设置文本框
        if (st->choice == node)
            SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)img->contant); // 设置文本框

        free(ciphertext);

        node = node->bk;
    }
    upload = true;
}
