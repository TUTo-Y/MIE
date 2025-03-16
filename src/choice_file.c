
#define GLFW_EXPOSE_NATIVE_WIN32
#include <winsock2.h>
#include <windows.h>
#include <commdlg.h>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "gui_ID.h"
#include "choice_file.h"

char *choiceImageFile()
{
    OPENFILENAME ofn; // 结构体存储文件选择对话框的信息
    char szFile[260]; // 缓冲区存储文件名

    // 初始化 OPENFILENAME 结构体
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = glfwGetWin32Window(((GUIwin *)guiIDGet(GUI_ID_WINDOW))->window);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "image file\0*.JPEG;*.JPG;*.PNG;*.TGA;*.BMP;*.PSD;*.GIF;*.HDR;*.PIC;*.PNM;*.jpeg;*.jpg;*.png;*.tga;*.bmp;*.psd;*.gif;*.hdr;*.pic;*.pnm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 显示文件选择对话框
    if (GetOpenFileName(&ofn) == TRUE)
    {
        // 返回选择的文件路径
        return strdup(ofn.lpstrFile);
    }

    return NULL;
}