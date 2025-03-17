#include "doctor.h"

struct
{
    char *img;
    int w, h;

    union
    {
        PACK pack;
        PACKbuf packbuf;
    };

} download;

// 医生等待任务线程
void *doctorWaitThread(void *data);

// 医生检查任务
bool doctorCheckTask(size_t flag, void *data);

// 医生能否治疗回调函数
void doctorCheckBackcall(GUIid id, size_t flag, void *data);

// 医生被拒绝
bool doctorQuit(size_t flag, void *data);

// 医生等待密钥线程2
void *doctorWait2Thread(void *);

// 医生建议界面任务
bool doctorAdviceTask(size_t flag, void *data);

// 发送任务
void doctorSend(GUIid id, size_t flag, void *data);

// 发送线程
void *doctorSendThread(void *data);

void doctorInit()
{
    // guiSetDefaultClearColor(0xF1 / 255.0f, 0xF8 / 255.0f, 0xE9 / 255.0f, 1.0f);
    guiSetDefaultClearColor(0.85f, 0.9f, 0.95f, 1.0f);
    GUI_SET_DEFAULT_CLEARCOLOR();

    // 设置医生等待界面
    mat4 model;
    glm_translate_make(model, (vec3){0.0f, -250.0f, 0.0f});
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_WAIT), L"等待中...");

    // 添加医生等待界面
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 0, 100, true);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 0, 100, true);

    // 医生检查是否按钮
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_BUTTON_YES, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_BUTTON_NO, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);
    // 设置按钮
    guiWidgetButtonSetText(GUI_ID_DOCTOR_BUTTON_YES, L"可以治疗");
    guiWidgetButtonSetPos(GUI_ID_DOCTOR_BUTTON_YES, -300, -250, 200, 100);
    guiWidgetButtonSetColor(GUI_ID_DOCTOR_BUTTON_YES, (vec4){0x81 / 255.0f, 0xC7 / 255.0f, 0x84 / 255.0f, 1.0f}, (vec4){0x43 / 255.0f, 0xA0 / 255.0f, 0x47 / 255.0f, 1.0f});
    guiWidgetButtonSetBackCall(GUI_ID_DOCTOR_BUTTON_YES, doctorCheckBackcall, 1, NULL);
    guiWidgetButtonSetText(GUI_ID_DOCTOR_BUTTON_NO, L"放弃治疗");
    guiWidgetButtonSetPos(GUI_ID_DOCTOR_BUTTON_NO, 100, -250, 200, 100);
    guiWidgetButtonSetColor(GUI_ID_DOCTOR_BUTTON_NO, (vec4){0xFF / 255.0f, 0x8A / 255.0f, 0x65 / 255.0f, 1.0f}, (vec4){0xF4 / 255.0f, 0x51 / 255.0f, 0x1E / 255.0f, 1.0f});
    guiWidgetButtonSetBackCall(GUI_ID_DOCTOR_BUTTON_NO, doctorCheckBackcall, 0, NULL);

    // 图像显示控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_IMG,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_SCROLL,
                       800, 100, false);

    // 患者信息控件及其其他文本控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_NAME,
                       GUI_WIDGET_CALLFLAG_DRAW,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_AGE,
                       GUI_WIDGET_CALLFLAG_DRAW,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_STATE,
                       GUI_WIDGET_CALLFLAG_DRAW,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_TEXT_ADVICE,
                       GUI_WIDGET_CALLFLAG_DRAW,
                       800, 100, false);

    GUIstr *name = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_NAME);
    GUIstr *age = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_AGE);
    GUIstr *state = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_STATE);
    GUIstr *advice = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_ADVICE);
    GUIstr *check = guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_CHECK);
    guiStrSetMode(name, GUI_STR_MOD_LEFT_TOP);
    guiStrSetMode(age, GUI_STR_MOD_LEFT_TOP);
    guiStrSetMode(state, GUI_STR_MOD_LEFT_TOP);
    guiStrSetMode(advice, GUI_STR_MOD_LEFT_TOP);
    guiStrSetMode(check, GUI_STR_MOD_CENTER);
    guiStrAppMode(name);
    guiStrAppMode(age);
    guiStrAppMode(state);
    guiStrAppMode(advice);
    guiStrAppMode(check);

    glm_translate_make(model, (vec3){-640.0f, -240.0f, 0.0f});
    guiStrSetModel(check, model);
    glm_translate_make(model, (vec3){-640.0f, -240.0f, 0.0f});
    guiStrSetModel(advice, model);
    glm_translate_make(model, (vec3){100.0f, 0.0f, 0.0f});
    guiStrSetModel(state, model);
    glm_translate(model, (vec3){0.0f, 190.0f, 0.0f});
    guiStrSetModel(age, model);
    glm_translate(model, (vec3){0.0f, 190.0f, 0.0f});
    guiStrSetModel(name, model);

    guiStrCpy(advice, L"建议:");
    guiStrCpy(check, L"是否可以治疗?");

    // 建议输入框和发送按钮
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_INPUT_ADVICE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_DOCTOR_BUTTON_SEND, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);

    // 设置输入框
    guiWidgetInputSetPos(GUI_ID_DOCTOR_INPUT_ADVICE, (vec3){-50.0f, -320.0f, 0.0f});
    guiWidgetInputSetDefaultText(GUI_ID_DOCTOR_INPUT_ADVICE, L"请输入诊断建议");
    guiWidgetInputDefaultTextColor(GUI_ID_DOCTOR_INPUT_ADVICE, (vec4){0.5f, 0.5f, 0.5f, 0.9f});
    guiWidgetInputSetTextColor(GUI_ID_DOCTOR_INPUT_ADVICE, (vec4){0.26f, 0.26f, 0.26f, 1.0f});
    guiWidgetInputSetLineSize(GUI_ID_DOCTOR_INPUT_ADVICE, 800.0f);
    guiWidgetInputSetMax(GUI_ID_DOCTOR_INPUT_ADVICE, 0x15);

    // 设置按钮
    guiWidgetButtonSetText(GUI_ID_DOCTOR_BUTTON_SEND, L"发送");
    guiWidgetButtonSetPos(GUI_ID_DOCTOR_BUTTON_SEND, 400, -250, 200, 100);
    guiWidgetButtonSetColor(GUI_ID_DOCTOR_BUTTON_SEND, (vec4){0x0b / 255.0f, 0xe8 / 255.0f, 0x81 / 255.0f, 1.0f}, (vec4){0x05 / 255.0f, 0xc4 / 255.0f, 0x6b / 255.0f, 1.0f});
    guiWidgetButtonSetBackCall(GUI_ID_DOCTOR_BUTTON_SEND, doctorSend, 0, NULL);

    // 进入医生的Recv模式
    pthread_t thread;
    pthread_create(&thread, NULL, doctorWaitThread, NULL);
    pthread_detach(thread);
}

void *doctorWaitThread(void *data)
{
    // 告诉服务器进入医生的Recv模式
    webSendFlag(sockfd, WEB_MSG_DOCTOR_WAIT);

    // 等待服务器的消息
    while (webRecvFlag(sockfd) != WEB_MSG_DOCTOR_MSG)
        ;

    // 接受图像
    size_t wh = webRecvFlag(sockfd);
    download.w = wh >> 32;
    download.h = wh & 0xffffffff;
    uint8_t *img1 = (uint8_t *)webRecvData(sockfd, NULL, NULL);
    uint8_t *img2 = (uint8_t *)webRecvData(sockfd, NULL, NULL);
    int mSize = (int)webRecvFlag(sockfd);
    uint8_t *m = (uint8_t *)webRecvData(sockfd, NULL, NULL);

    // 接受key1
    size_t key1;
    ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    uint8_t *key = encKEMDec(kem, &keyClient);
    key1 = *(uint64_t *)key;
    free(key);
    encKEMFree(kem);

    // test
    DEBUG("key1 : ");
    putbin2hex((uint8_t *)&key1, sizeof(key1), stdout);
    DEBUG("");
    
    // 使用key1解密图像
    rdhCombineImage((uint8_t *)img1, (uint8_t *)img2, download.w * download.h, (uint8_t **)&download.img);
    rdhUnshuffleImage((uint8_t *)download.img, download.w * download.h, key1);

    // 提取图像数据信息
    char *buf;
    if (rdhExtractData(img1, img2, download.w, download.h, m, mSize, (uint8_t **)&buf) == RDH_ERROR)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"提取数据失败", GUI_WIDGET_LOG_ERROR);
        return NULL;
    }
    memcpy((void *)&download.pack, buf, sizeof(PACKbuf));

    free(img1);
    free(img2);
    free(m);
    free(buf);

    // 添加任务进入医生检查界面
    guiTaskAddTask(doctorCheckTask, 0, NULL);

    return NULL;
    // // 接受图像
    // size_t wh = webRecvFlag(sockfd);
    // download.w = wh >> 32;
    // download.h = wh & 0xffffffff;
    // download.img = (char *)malloc(download.w * download.h * 2);
    // uint8_t *img1 = (uint8_t *)webRecvData(sockfd, NULL, NULL);
    // uint8_t *img2 = (uint8_t *)webRecvData(sockfd, NULL, NULL);

    // int mSize = (int)webRecvFlag(sockfd);
    // uint8_t *m = (uint8_t *)webRecvData(sockfd, NULL, NULL);

    // // 接受密钥
    // uint8_t *key;
    // size_t key1;
    // uint8_t key2[SM4_KEY_SIZE * 2];
    // ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    // key = encKEMDec(kem, &keyClient);
    // memcpy(&key1, key, sizeof(uint64_t));
    // memcpy(key2, key + sizeof(uint64_t), SM4_KEY_SIZE * 2);
    // free(key);
    // encKEMFree(kem);

    // // 提取数据
    // char *buf;
    // PACKbuf packbuf;
    // size_t packlen;
    // size_t packlen2;
    // if (rdhExtractData(img1, img2, download.w, download.h, m, mSize, (uint8_t **)&buf) == RDH_ERROR)
    // {
    //     guiWidgetLogAddMsg(GUI_ID_LOG, L"提取数据失败", GUI_WIDGET_LOG_ERROR);
    //     return NULL;
    // }
    // packlen = sizeof(PACKbuf);
    // memcpy(&packbuf, buf, packlen);

    // // 解密数据
    // SM4_CBC_CTX sm4_key;
    // sm4_cbc_decrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
    // sm4_cbc_decrypt_update(&sm4_key, (uint8_t *)&packbuf, sizeof(PACKbuf), (uint8_t *)&download.pack, &packlen);
    // sm4_cbc_decrypt_finish(&sm4_key, ((char *)&packbuf) + packlen, &packlen2);
    // packlen += packlen2;

    // // 合成图像
    // rdhCombineImage(img1, img2, download.w * download.h, (uint8_t **)&download.img);

    // // 使用key1解密
    // rdhUnshuffleImage((uint8_t *)download.img, download.w * download.h, key1);

    // // 添加任务
    // guiTaskAddTask(doctorCheck, 0, NULL);

    // free(img1);
    // free(img2);
    // free(m);
    // free(buf);
}

// 医生检查任务
bool doctorCheckTask(size_t flag, void *data)
{
    // 禁用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);

    // 设置控件
    guiWidgetImgSetRect(GUI_ID_DOCTOR_IMG, (GUIrect){-250.0f, 400.0f, 500.0f, 500.0f});
    guiWidgetImgSetTexture(GUI_ID_DOCTOR_IMG, (unsigned char *)download.img, download.w, download.h);

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_IMG, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_BUTTON_YES, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_BUTTON_NO, GUI_WIDGET_CALLFLAG_ALL, true);

    return true;
}

void doctorCheckBackcall(GUIid id, size_t flag, void *data)
{
    // 禁用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_IMG, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_BUTTON_YES, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_BUTTON_NO, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);

    if (flag) // 可以治疗
    {
        // 发送消息
        webSendFlag(sockfd, WEB_MSG_DOCTOR_YES);

        // 进入等待线程2
        pthread_t thread;
        pthread_create(&thread, NULL, doctorWait2Thread, NULL);
        pthread_detach(thread);
    }
    // 无法治疗
    else
    {
        // 发送消息
        webSendFlag(sockfd, WEB_MSG_DOCTOR_NO);

        memset(&download, 0, sizeof(download));

        // 进入医生的Recv模式
        pthread_t thread;
        pthread_create(&thread, NULL, doctorWaitThread, NULL);
        pthread_detach(thread);
    }
}

bool doctorQuit(size_t flag, void *data)
{
    memset(&download, 0, sizeof(download));

    // 进入医生的Recv模式
    pthread_t thread;
    pthread_create(&thread, NULL, doctorWaitThread, NULL);
    pthread_detach(thread);
}

void *doctorWait2Thread(void *t)
{
    // 患者不接受治疗
    if (webRecvFlag(sockfd) == WEB_MSG_DOCTOR_NO)
        guiTaskAddTask(doctorQuit, 0, NULL);

    // 接受key2
    uint8_t key2[SM4_KEY_SIZE * 2];
    ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    uint8_t *key = encKEMDec(kem, &keyClient);
    memcpy(key2, key, SM4_KEY_SIZE * 2);
    free(key);
    encKEMFree(kem);

    DEBUG("key2 : ");
    putbin2hex((uint8_t *)key2, sizeof(key2), stdout);
    DEBUG("");

    // 解密图像数据
    PACKbuf packbuf;
    size_t packlen = sizeof(PACKbuf);
    size_t packlen2;
    SM4_CBC_CTX sm4_key;
    sm4_cbc_decrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
    sm4_cbc_decrypt_update(&sm4_key, (uint8_t *)&download.pack, sizeof(PACKbuf), (uint8_t *)&packbuf, &packlen);
    sm4_cbc_decrypt_finish(&sm4_key, ((char *)&packbuf) + packlen, &packlen2);
    packlen += packlen2;
    memcpy(&download.pack, &packbuf, sizeof(PACKbuf));

    // 添加任务进入医生建议
    guiTaskAddTask(doctorAdviceTask, 0, NULL);
}

bool doctorAdviceTask(size_t flag, void *data)
{
    // 禁用等待控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);

    // 设置控件
    guiWidgetImgSetRect(GUI_ID_DOCTOR_IMG, (GUIrect){-640.0f, 400.0f, 500.0f, 500.0f});

    wchar_t str[0x50];
    wcscpy(str, L"姓名: ");
    wcscat(str, download.pack.name);
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_NAME), str);

    wcscpy(str, L"年龄: ");
    wcscat(str, download.pack.age);
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_AGE), str);

    wcscpy(str, L"描述: ");
    wcscat(str, download.pack.state);
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_DOCTOR_TEXT_STATE), str);

    // 清空建议
    GUIwidgetInputData *st = (GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_DOCTOR_INPUT_ADVICE)->data1;
    guiStrCpy(st->textRender, L"");
    wcscpy(st->textplain, L"");

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_IMG, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_NAME, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_AGE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_STATE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_INPUT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_BUTTON_SEND, GUI_WIDGET_CALLFLAG_ALL, true);

    return true;
}

void doctorSend(GUIid id, size_t flag, void *data)
{
    // 禁用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_IMG, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_NAME, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_AGE, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_STATE, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_INPUT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_BUTTON_SEND, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);

    // 复制建议
    wchar_t *advice = ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_DOCTOR_INPUT_ADVICE)->data1)->textplain;
    wcscpy(download.pack.advice, advice);

    // 添加建议
    guiWidgetLogAddMsg(GUI_ID_LOG, L"感谢您的建议", GUI_WIDGET_LOG_MSG);

    // 启用发送线程
    pthread_t thread;
    pthread_create(&thread, NULL, doctorSendThread, NULL);
    pthread_detach(thread);
}

void *doctorSendThread(void *data)
{
    // 发送数据
    ENCkem kem = encKEMEnc((uint8_t *)&download.pack.advice, sizeof(download.pack.advice), &keyServer);
    webSendData(sockfd, (uint8_t *)kem, encKEMSizeKEM(kem));
    encKEMFree(kem);

    doctorQuit(0, NULL);

    return NULL;
}