#include "gui_widget_mousemove.h"

bool move = false;
double xpos_old, ypos_old;

bool gui_widget_mousemove_eventCall(GUIid id, const GUIevent *event)
{
    // 鼠标左键事件
    if (event->type == GUI_EVENT_TYPE_MOUSE_BUTTON &&
        event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (event->MouseButton.action == GLFW_PRESS)
        {
            move = true;

            // 记录旧位置
            xpos_old = event->mousePos.x;
            ypos_old = event->mousePos.y;
        }
        else if (event->MouseButton.action == GLFW_RELEASE)
        {
            move = false;
        }
    }
    // 光标按下移动事件
    else if (event->type == GUI_EVENT_TYPE_CURSOR_POS && move == true)
    {
        // 计算偏移
        double dx = event->mousePos.x - xpos_old;
        double dy = event->mousePos.y - ypos_old;

        // 更新旧位置
        int win_x, win_y;
        glfwGetWindowPos(((GUIwin *)guiIDGet(GUI_ID_WINDOW))->window, &win_x, &win_y);
        glfwSetWindowPos(((GUIwin *)guiIDGet(GUI_ID_WINDOW))->window, win_x + (int)dx, win_y - (int)dy);
    }

    return true;
}
