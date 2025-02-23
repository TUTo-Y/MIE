#include "gui_widget_mousemove.h"

bool move = false;
double xpos_old, ypos_old;
bool guiWidgetMouseMoveEvent(GUIwin *win, GUIwidget *widget, const GUIevent *event)
{
    if (event->type == GUI_EVENT_TYPE_MOUSE_BUTTON) // 鼠标事件
    {
        if (event->MouseButton.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (event->MouseButton.action == GLFW_PRESS)
            {
                move = true;
                glfwGetCursorPos(win->window, &xpos_old, &ypos_old);
            }
            else if (event->MouseButton.action == GLFW_RELEASE)
            {
                move = false;
            }
        }
    }
    else if (event->type == GUI_EVENT_TYPE_CURSOR_POS) // 光标事件
    {
        if (move == true)
        {
            int win_x, win_y;
            glfwGetWindowPos(win->window, &win_x, &win_y);

            double dx = event->CursorPos.xpos - xpos_old;
            double dy = event->CursorPos.ypos - ypos_old;

            glfwSetWindowPos(win->window, win_x + (int)dx, win_y + (int)dy);
        }
    }

    return true;
}
