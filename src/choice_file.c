
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
    OPENFILENAMEW ofn; 
    wchar_t szFile[260]; 

    // 初始化 OPENFILENAME 结构体
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = glfwGetWin32Window(((GUIwin *)guiIDGet(GUI_ID_WINDOW))->window);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"图像文件\0*.JPEG;*.JPG;*.PNG;*.TGA;*.BMP;*.PSD;*.GIF;*.HDR;*.PIC;*.PNM;*.jpeg;*.jpg;*.png;*.tga;*.bmp;*.psd;*.gif;*.hdr;*.pic;*.pnm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 显示文件选择对话框
    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        // 将宽字符路径转换为 UTF-8
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, szFile, -1, NULL, 0, NULL, NULL);
        char *utf8Path = (char *)malloc(size_needed);
        if (utf8Path)
        {
            WideCharToMultiByte(CP_UTF8, 0, szFile, -1, utf8Path, size_needed, NULL, NULL);
            return utf8Path;
        }
    }

    return NULL;
}

char *choiceTextFile()
{
    OPENFILENAMEW ofn; 
    wchar_t szFile[260]; 

    // 初始化 OPENFILENAME 结构体
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = glfwGetWin32Window(((GUIwin *)guiIDGet(GUI_ID_WINDOW))->window);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"文本文件\0*.txt\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 显示文件选择对话框
    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        // 将宽字符路径转换为 UTF-8
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, szFile, -1, NULL, 0, NULL, NULL);
        char *utf8Path = (char *)malloc(size_needed);
        if (utf8Path)
        {
            WideCharToMultiByte(CP_UTF8, 0, szFile, -1, utf8Path, size_needed, NULL, NULL);
            return utf8Path;
        }
    }

    return NULL;
}