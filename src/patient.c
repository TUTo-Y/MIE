#include "patient.h"

struct
{
    char *img; // 图像数据
    int w, h;  // 图片的宽和高
    PACK pack; // 数据包
} upload;

// 患者点击上传按钮的回调函数
void patientButtonUploadBackcall(GUIid id, size_t flag, void *data);

// 上传数据的线程函数
void *patientUploadThread(void *data);

// 患者检查医生任务
bool patientCheckDoctorTask(size_t flag, void *data);

// 患者检查医生的按钮回调函数
void patientCheckDoctorBackcall(GUIid id, size_t flag, void *data);

// 患者等待服务器发送医生的建议
void *patientWaitMsgThread(void *data);

// 添加医嘱的回调函数
bool patientAdvice(size_t flag, void *data);

// 医嘱阅读完毕的回调函数
void patientOver(GUIid id, size_t flag, void *data);

void patientInit()
{
    // 设置背景
    // guiSetDefaultClearColor(0xE0 / 255.0f, 0xF7 / 255.0f, 0xFA / 255.0f, 1.0f);
    guiSetDefaultClearColor(0.95f, 0.9f, 0.85f, 1.0f);
    GUI_SET_DEFAULT_CLEARCOLOR();

    // 患者加号界面，并启用
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BIGCHOICE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);

    // 图片显示控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_IMG,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_SCROLL,
                       800, 100, false);
    guiWidgetImgSetRect(GUI_ID_PATIENT_IMG, (GUIrect){-640.0f, 400.0f, 500.0f, 500.0f});

    // 三个姓名年龄描述的输入框
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_INPUT_NAME,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_INPUT_AGE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_INPUT_STATE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);

    guiWidgetInputSetDefaultText(GUI_ID_PATIENT_INPUT_NAME, L"请输入姓名");
    guiWidgetInputSetDefaultText(GUI_ID_PATIENT_INPUT_AGE, L"请输入年龄");
    guiWidgetInputSetDefaultText(GUI_ID_PATIENT_INPUT_STATE, L"请输入病情描述");

    guiWidgetInputDefaultTextColor(GUI_ID_PATIENT_INPUT_NAME, (vec4){0.5f, 0.5f, 0.5f, 0.9f});
    guiWidgetInputDefaultTextColor(GUI_ID_PATIENT_INPUT_AGE, (vec4){0.5f, 0.5f, 0.5f, 0.9f});
    guiWidgetInputDefaultTextColor(GUI_ID_PATIENT_INPUT_STATE, (vec4){0.5f, 0.5f, 0.5f, 0.9f});

    guiWidgetInputSetTextColor(GUI_ID_PATIENT_INPUT_NAME, (vec4){0.26f, 0.26f, 0.26f, 1.0f});
    guiWidgetInputSetTextColor(GUI_ID_PATIENT_INPUT_AGE, (vec4){0.26f, 0.26f, 0.26f, 1.0f});
    guiWidgetInputSetTextColor(GUI_ID_PATIENT_INPUT_STATE, (vec4){0.26f, 0.26f, 0.26f, 1.0f});

    guiWidgetInputSetLineSize(GUI_ID_PATIENT_INPUT_NAME, 600.0f);
    guiWidgetInputSetLineSize(GUI_ID_PATIENT_INPUT_AGE, 600.0f);
    guiWidgetInputSetLineSize(GUI_ID_PATIENT_INPUT_STATE, 600.0f);

    guiWidgetInputSetMax(GUI_ID_PATIENT_INPUT_NAME, 0x10);
    guiWidgetInputSetMax(GUI_ID_PATIENT_INPUT_AGE, 0x4);
    guiWidgetInputSetMax(GUI_ID_PATIENT_INPUT_STATE, 0x10);

    guiWidgetInputSetPos(GUI_ID_PATIENT_INPUT_NAME, (vec3){300.0f, 300.0f, 0.0f});
    guiWidgetInputSetPos(GUI_ID_PATIENT_INPUT_AGE, (vec3){300.0f, 150.0f, 0.0f});
    guiWidgetInputSetPos(GUI_ID_PATIENT_INPUT_STATE, (vec3){300.0f, 0.0f, 0.0f});

    // 重新选择和上传按钮
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, false);

    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_CHOICE, L"重新选择");
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_UPLOAD, L"上传图像");

    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_CHOICE, -550, -200, 300, 100);
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_UPLOAD, 250, -200, 300, 100);

    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_CHOICE, (vec4){0x90 / 255.0f, 0xF7 / 255.0f, 0xEC / 255.0f, 1.0f}, (vec4){0x32 / 255.0f, 0xCC / 255.0f, 0xBC / 255.0f, 1.0f});
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_UPLOAD, (vec4){0xFA / 255.0f, 0xD7 / 255.0f, 0xA1 / 255.0f, 1.0f}, (vec4){0xE9 / 255.0f, 0x6D / 255.0f, 0x71 / 255.0f, 1.0f});

    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_CHOICE, patientButtonChoiceBackcall, 0, NULL);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_UPLOAD, patientButtonUploadBackcall, 0, NULL);

    // 等待控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    mat4 model;
    glm_translate_make(model, (vec3){0.0f, -300.0f, 0.0f});
    guiWidgetWaitSetModel(GUI_ID_WAIT, model);

    // 检查和选择控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_TEXT_CHECK, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON1_CHECK, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS, 800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON2_CHECK, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS, 800, 100, false);

    glm_translate_make(model, (vec3){0.0f, -250.0f, 0.0f});
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_CHECK), L"Text");

    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON1_CHECK, L"允许");
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON2_CHECK, L"不允许");

    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON1_CHECK, -550, -300, 300, 100);
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON2_CHECK, 250, -300, 300, 100);

    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON1_CHECK, (vec4){0xe4 / 255.0f, 0xf9 / 255.0f, 0xf5 / 255.0f, 1.0f}, (vec4){0x30 / 255.0f, 0xe3 / 255.0f, 0xca / 255.0f, 1.0f});
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON2_CHECK, (vec4){0xff / 255.0f, 0xaa / 255.0f, 0xa6 / 255.0f, 1.0f}, (vec4){0xff / 255.0f, 0x8c / 255.0f, 0x94 / 255.0f, 1.0f});

    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON1_CHECK, patientCheckDoctorBackcall, 1, NULL);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON2_CHECK, patientCheckDoctorBackcall, 0, NULL);

    // 显示建议的控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, false);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_ADVICE, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS, 800, 100, false);

    GUIstr *advice = guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_ADVICE);
    glm_translate_make(model, (vec3){0.0f, -200.0f, 0.0f});
    guiStrSetModel(advice, model);

    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_ADVICE, L"确定");
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_ADVICE, -100, -260, 200, 100);
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_ADVICE, (vec4){0x0b / 255.0f, 0xe8 / 255.0f, 0x81 / 255.0f, 1.0f}, (vec4){0x05 / 255.0f, 0xc4 / 255.0f, 0x6b / 255.0f, 1.0f});
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_ADVICE, patientOver, 0, NULL);
}

bool patientChoiceImage(size_t wh, void *data)
{
    // int width = wh >> 32;
    // int height = wh & 0xffffffff;
    // unsigned char *img = (unsigned char *)data;

    // 删除患者加号界面
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BIGCHOICE);

    // // 设置控件
    // guiWidgetImgSetTexture(GUI_ID_PATIENT_IMG, img, width, height);

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_IMG, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);

    return true;
}

void patientButtonChoiceBackcall(GUIid id, size_t flag, void *data)
{
    // 选择图片
    char *img_str = choiceImageFile();
    if (!img_str)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"未选择图片", GUI_WIDGET_LOG_WARN);
        return;
    }

    // 读取图片
    int width, height, channels;
    unsigned char *img = stbi_load(img_str, &width, &height, &channels, 1);
    if (!img)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"加载图片失败", GUI_WIDGET_LOG_ERROR);
        free(img_str);
        return;
    }

    // 选择图片
    guiWidgetImgSetTexture(GUI_ID_PATIENT_IMG, img, width, height);

    // 释放内存
    free(img_str);
}

void patientButtonUploadBackcall(GUIid id, size_t flag, void *data)
{
    guiWidgetLogAddMsg(GUI_ID_LOG, L"上传图像", GUI_WIDGET_LOG_WARN);

    // 禁用文本框的消息回调函数
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_EVENT, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_EVENT, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_EVENT, false);

    // 禁用按钮
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用等待控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, true);

    // 保存数据
    GUIwidgetimg *st = (GUIwidgetimg *)GUI_ID2WIDGET(GUI_ID_PATIENT_IMG)->data1;
    upload.img = (char *)malloc(st->w * st->h);
    memcpy(upload.img, st->data, st->w * st->h);
    upload.w = st->w;
    upload.h = st->h;
    packInit(&upload.pack);
    packSetName(&upload.pack, ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_PATIENT_INPUT_NAME)->data1)->textplain);
    packSetAge(&upload.pack, ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_PATIENT_INPUT_AGE)->data1)->textplain);
    packSetState(&upload.pack, ((GUIwidgetInputData *)GUI_ID2WIDGET(GUI_ID_PATIENT_INPUT_STATE)->data1)->textplain);

    // 创建线程用来上传数据
    pthread_t thread; // 线程标识符
    pthread_create(&thread, NULL, patientUploadThread, NULL);
    pthread_detach(thread);
}

// 任务函数
void *patientUploadThread(void *data)
{
    /* 生成图像加密密钥key1 */
    uint64_t key1 = rand();

    /* 生成图像加密密钥key2 */
    uint8_t key2[SM4_KEY_SIZE * 2];
    encSm4KeyIv(&key2[0], &key2[SM4_KEY_SIZE]);

    // test
    DEBUG("key1 : ");
    putbin2hex((uint8_t *)&key1, sizeof(key1), stdout);
    DEBUG("");
    
    DEBUG("key2 : ");
    putbin2hex((uint8_t *)key2, sizeof(key2), stdout);
    DEBUG("");

    /* 加密数据信息数据 */
    PACKbuf packbuf;
    size_t packlen;
    size_t packlen2;
    SM4_CBC_CTX sm4_key;
    sm4_cbc_encrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
    sm4_cbc_encrypt_update(&sm4_key, (uint8_t *)&upload.pack, sizeof(PACK), (uint8_t *)&packbuf, &packlen);
    sm4_cbc_encrypt_finish(&sm4_key, ((char *)&packbuf) + packlen, &packlen2);
    packlen += packlen2;

    /* 使用key1打乱图像 */
    rdhShuffleImage((uint8_t *)upload.img, upload.w * upload.h, key1);

    /* 分割图像 */
    uint8_t *img1, *img2;
    rdhSplitImage((uint8_t *)upload.img, upload.w * upload.h, &img1, &img2);
    
    /* 嵌入数据 */
    uint8_t *m = NULL;
    int mSize = 0;
    if (rdhEmbedData(img1, img2, upload.w, upload.h, &m, &mSize, (uint8_t *)&packbuf, packlen) == RDH_ERROR)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"嵌入数据失败", GUI_WIDGET_LOG_ERROR);
        return NULL;
    }

    /* 向服务器发送上传数据的请求 */
    webSendFlag(sockfd, WEB_MSG_PAITENT_UPLOAD); // 发送消息

    /* 打包密钥并使用kem加密后发送 */
    uint8_t key_pack[sizeof(uint64_t) + SM4_KEY_SIZE * 2];
    memcpy(key_pack, &key1, sizeof(uint64_t));
    memcpy(key_pack + sizeof(uint64_t), key2, SM4_KEY_SIZE * 2);
    ENCkem kem = encKEMEnc(key_pack, sizeof(key_pack), &keyServer);
    webSendData(sockfd, (char *)kem, encKEMSizeKEM(kem));

    // 上传图像
    webSendFlag(sockfd, (((size_t)upload.w) << 32) | upload.h); // 发送wh
    webSendData(sockfd, img1, upload.w * upload.h);             // 上传图像
    webSendData(sockfd, img2, upload.w * upload.h);             // 上传图像
    free(img1);
    free(img2);

    // 上传m
    webSendFlag(sockfd, mSize);
    webSendData(sockfd, m, mSize);
    free(m);

    // 释放冗余数据
    encKEMFree(kem);
    free(upload.img);

    /* 开始监听模式, 直到选择 */
    while (webRecvFlag(sockfd) != WEB_MSG_PAITENT_MSG)
    {
    }

    /* 添加任务让用户确认 */
    kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    uint8_t *name = encKEMDec(kem, &keyClient);
    guiTaskAddTask(patientCheckDoctorTask, 0, name);

    return NULL;
}

bool patientCheckDoctorTask(size_t flag, void *data)
{
    // 禁用等待状态
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    wchar_t name[0x40];
    wcscpy(name, L"是否允许");
    mbstowcs(name + wcslen(name), data, strlen(data));
    wcscat(name, L"检查?");
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_CHECK), name);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_TEXT_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON1_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON2_CHECK, GUI_WIDGET_CALLFLAG_ALL, true);
    free(data);

    return true;
}

// 允许
void patientCheckDoctorBackcall(GUIid id, size_t flag, void *data)
{
    // 禁用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_TEXT_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON1_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON2_CHECK, GUI_WIDGET_CALLFLAG_ALL, false);

    if (flag) // 允许
    {
        // 告诉服务器允许上传
        webSendFlag(sockfd, WEB_MSG_PAITENT_YES);

        // 启用控件
        guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, true);

        // 创建等待线程
        pthread_t thread;
        pthread_create(&thread, NULL, patientWaitMsgThread, NULL);
        pthread_detach(thread);
    }
    else // 不允许
    {
        // 告诉服务器不允许上传
        webSendFlag(sockfd, WEB_MSG_PAITENT_NO);

        // 文本框的消息
        guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_EVENT, true);
        guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_EVENT, true);
        guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_EVENT, true);

        // 按钮
        guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);
        guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, true);
    }
}
void *patientWaitMsgThread(void *t)
{
    // 接受数据
    ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    uint8_t *data = encKEMDec(kem, &keyClient);
    size_t data_size = encKEMSizeText(kem);
    memcpy(&upload.pack.advice, data, data_size);

    // 释放数据
    encKEMFree(kem);
    free(data);

    // 添加控件启动
    guiTaskAddTask(patientAdvice, 0, NULL);

    return NULL;
}

bool patientAdvice(size_t flag, void *data)
{
    // 禁用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_ALL, false);

    // 设置控件
    wchar_t advice[0x50];
    wcscpy(advice, L"医生建议: ");
    wcscat(advice, upload.pack.advice);
    guiStrCpy(guiWidgetTextGetStr(GUI_ID_PATIENT_TEXT_ADVICE), advice);

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_ADVICE, GUI_WIDGET_CALLFLAG_ALL, true);

    return true;
}

void patientOver(GUIid id, size_t flag, void *data)
{
    // 禁用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_TEXT_ADVICE, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_ADVICE, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_NAME, GUI_WIDGET_CALLFLAG_EVENT, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_AGE, GUI_WIDGET_CALLFLAG_EVENT, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_INPUT_STATE, GUI_WIDGET_CALLFLAG_EVENT, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, true);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, true);
}