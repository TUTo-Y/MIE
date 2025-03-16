#include "patient.h"

struct
{
    char *img;
    int w, h;

    PACK pack;
} upload;

void *patientButtonUploadThread(void *data);

bool patientChoiceImage(size_t wh, void *data)
{
    int width = wh >> 32;
    int height = wh & 0xffffffff;
    unsigned char *img = (unsigned char *)data;

    // 删除选择控件
    guiControlDeleteWidget(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BIGCHOICE);

    // 启用图片显示控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_IMG,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_SCROLL,
                       800, 100, true);
    guiWidgetImgSetTexture(GUI_ID_PATIENT_IMG, img, width, height);
    guiWidgetImgSetRect(GUI_ID_PATIENT_IMG, (GUIrect){-640.0f, 400.0f, 500.0f, 500.0f});

    // 启用三个输入框
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_INPUT_NAME,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_INPUT_AGE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_INPUT_STATE,
                       GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_CHAR_MODS | GUI_WIDGET_CALLFLAG_EVENT_KEY | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);

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

    // 启用两个按钮
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_DRAW | GUI_WIDGET_CALLFLAG_EVENT_MOUSE_BUTTON | GUI_WIDGET_CALLFLAG_EVENT_CURSOR_POS,
                       800, 100, true);

    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_CHOICE, L"重新选择");
    guiWidgetButtonSetText(GUI_ID_PATIENT_BUTTON_UPLOAD, L"上传图像");

    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_CHOICE, -550, -200, 300, 100);
    guiWidgetButtonSetPos(GUI_ID_PATIENT_BUTTON_UPLOAD, 250, -200, 300, 100);

    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_CHOICE, (vec4){0x90 / 255.0f, 0xF7 / 255.0f, 0xEC / 255.0f, 1.0f}, (vec4){0x32 / 255.0f, 0xCC / 255.0f, 0xBC / 255.0f, 1.0f});
    guiWidgetButtonSetColor(GUI_ID_PATIENT_BUTTON_UPLOAD, (vec4){0xFA / 255.0f, 0xD7 / 255.0f, 0xA1 / 255.0f, 1.0f}, (vec4){0xE9 / 255.0f, 0x6D / 255.0f, 0x71 / 255.0f, 1.0f});

    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_CHOICE, patientButtonChoiceBackcall, 0, NULL);
    guiWidgetButtonSetBackCall(GUI_ID_PATIENT_BUTTON_UPLOAD, patientButtonUploadBackcall, 0, NULL);

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

    // 禁用按钮
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_UPLOAD, GUI_WIDGET_CALLFLAG_ALL, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_PATIENT_BUTTON_CHOICE, GUI_WIDGET_CALLFLAG_ALL, false);

    // 启用控件
    guiWidgetToControl(GUI_ID_WINDOW, GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, 800, 100, true);
    mat4 model;
    glm_translate_make(model, (vec3){0.0f, -300.0f, 0.0f});
    guiWidgetWaitSetModel(GUI_ID_WAIT, model);

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
    pthread_create(&thread, NULL, patientButtonUploadThread, NULL);
}
// 任务函数
void *patientButtonUploadThread(void *data)
{
    /* 生成图像加密密钥key1 */
    uint64_t key1 = rand();
    DEBUG("key1: %llx\n", key1);
    /* 生成图像加密密钥key2 */
    uint8_t key2[SM4_KEY_SIZE * 2];
    encSm4KeyIv(&key2[0], &key2[SM4_KEY_SIZE]);
    DEBUG("key2: %llx %llx %llx %llx\n", ((uint64_t *)key2)[0], ((uint64_t *)key2)[1], ((uint64_t *)key2)[2], ((uint64_t *)key2)[3]);
    SM4_CBC_CTX sm4_key;
    sm4_cbc_encrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);

    /* 加密数据 */
    PACKbuf packbuf;
    size_t packlen;
    size_t packlen2;
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

    /* 上传数据 */
    webSendFlag(sockfd, WEB_MSG_PAITENT_UPLOAD); // 发送消息

    // 打包密钥并使用kem加密后发送
    uint8_t key_pack[sizeof(uint64_t) + SM4_KEY_SIZE * 2];
    memcpy(key_pack, &key1, sizeof(uint64_t));
    memcpy(key_pack + sizeof(uint64_t), key2, SM4_KEY_SIZE * 2);
    ENCkem kem = encKEMEnc(key_pack, sizeof(key_pack), &keyServer);
    webSendData(sockfd, (char *)kem, encKEMSizeKEM(kem)); // 发送数据

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
    free(m);
    free(img1);
    free(img2);

    /* 开始监听模式, 直到下载 */
    while (webRecvFlag(sockfd) == WEB_MSG_PAITENT_DOWNLOAD)
        ;

    /* 下载服务器的请求 */

    return NULL;
}