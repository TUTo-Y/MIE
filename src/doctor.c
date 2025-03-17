#include "doctor.h"

struct
{
    char *img;
    int w, h;

    PACK pack;
} download;

bool doctorCheck(size_t flag, void *data);
void *doctorWait2Thread(void *data);

void *doctorWaitThread(void *data)
{
    // 告诉服务器进入医生的Recv模式
    webSendFlag(sockfd, WEB_MSG_DOCTOR_WAIT);

    // 等待服务器的消息
    while (webRecvFlag(sockfd) != WEB_MSG_DOCTOR_WAIT_OVER)
        ;

    // 接受密钥
    uint8_t *key;
    size_t key1;
    uint8_t key2[SM4_KEY_SIZE * 2];
    ENCkem kem = (ENCkem)webRecvData(sockfd, NULL, NULL);
    key = encKEMDec(kem, &keyClient);
    memcpy(&key1, key, sizeof(uint64_t));
    memcpy(key2, key + sizeof(uint64_t), SM4_KEY_SIZE * 2);
    free(key);
    encKEMFree(kem);

    // 接受图像
    size_t wh = webRecvFlag(sockfd);
    download.w = wh >> 32;
    download.h = wh & 0xffffffff;
    download.img = (char *)malloc(download.w * download.h * 2);
    uint8_t *img1 = (uint8_t *)webRecvData(sockfd, NULL, NULL);
    uint8_t *img2 = (uint8_t *)webRecvData(sockfd, NULL, NULL);

    int mSize = (int)webRecvFlag(sockfd);
    uint8_t *m = (uint8_t *)webRecvData(sockfd, NULL, NULL);

    // 提取数据
    char *buf;
    PACKbuf packbuf;
    size_t packlen;
    size_t packlen2;
    if (rdhExtractData(img1, img2, download.w, download.h, m, mSize, (uint8_t **)&buf) == RDH_ERROR)
    {
        guiWidgetLogAddMsg(GUI_ID_LOG, L"提取数据失败", GUI_WIDGET_LOG_ERROR);
        return NULL;
    }
    packlen = sizeof(PACKbuf);
    memcpy(&packbuf, buf, packlen);

    // 解密数据
    SM4_CBC_CTX sm4_key;
    sm4_cbc_decrypt_init(&sm4_key, &key2[0], &key2[SM4_KEY_SIZE]);
    sm4_cbc_decrypt_update(&sm4_key, (uint8_t *)&packbuf, sizeof(PACKbuf), (uint8_t *)&download.pack, &packlen);
    sm4_cbc_decrypt_finish(&sm4_key, ((char *)&packbuf) + packlen, &packlen2);
    packlen += packlen2;

    // 合成图像
    rdhCombineImage(img1, img2, download.w * download.h, (uint8_t **)&download.img);

    // 使用key1解密
    rdhUnshuffleImage((uint8_t *)download.img, download.w * download.h, key1);

    // 添加任务
    guiTaskAddTask(doctorCheck, 0, NULL);

    free(img1);
    free(img2);
    free(m);
    free(buf);

    return NULL;
}

bool doctorCheck(size_t flag, void *data)
{
    // 禁用等待控件
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);
    guiControlEnableCallback(GUI_ID2CONTROLP(GUI_ID_WINDOW), GUI_ID_DOCTOR_TEXT_WAIT, GUI_WIDGET_CALLFLAG_DRAW, false);

    // 设置控件
    guiWidgetImgSetTexture(GUI_ID_DOCTOR_IMG, (unsigned char *)download.img, download.w, download.h);
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
    pthread_create(&thread, NULL, doctorWait2Thread, NULL);
    pthread_detach(thread);

}

void *doctorWait2Thread(void *data)
{
    // 加密数据
    ENCkem kem = encKEMEnc((uint8_t *)&download.pack.advice, sizeof(download.pack.advice), &keyServer);
    
    // 发送数据
    webSendFlag(sockfd, WEB_MSG_DOCTOR_SEND);
    webSendData(sockfd, (uint8_t *)kem, encKEMSizeKEM(kem));

    encKEMFree(kem);

    // 不能删除 download.img
    memset(&download, 0, sizeof(download));

    // 再次进入等待线程
    pthread_t thread;
    pthread_create(&thread, NULL, doctorWaitThread, NULL);
    pthread_detach(thread);

    return NULL;
}