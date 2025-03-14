#ifndef GUI_BASE_H
#define GUI_BASE_H

#include <glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 954


#define WINDOW_POS_2_GL_POS_x(x, W) (-((W / 2) - (x)))
#define WINDOW_POS_2_GL_POS_y(y, H) ((H / 2) - (y))

#define PI 3.14159265358979323846

#define CALL(fun, ...) ((fun) ? (fun)(__VA_ARGS__) : false)

// 设置默认视图
#define GUI_SET_DEFAULT_VIEWPORT() glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
// 默认清屏颜色
#define GUI_SET_DEFAULT_CLEARCOLOR() glClearColor(0.88f, 0.88f, 0.88f, 1.0f)

// flag操作
#define GUI_FLAG_CHECK(flag, type) ((flag) & (type))   // 检查
#define GUI_FLAG_SET(flag, type) ((flag) |= (type))    // 添加
#define GUI_FLAG_UNSET(flag, type) ((flag) &= ~(type)) // 删除

// 位置操作
typedef struct
{
    double x;
    double y;
} GUIpos;
typedef struct
{
    double width;
    double height;
} GUIsize;
typedef struct
{
    double x;
    double y;
    double width;
    double height;
} GUIrect;

static inline void guiPosCopy(GUIpos *src, GUIpos *dst)
{
    dst->x = src->x;
    dst->y = src->y;
}
static inline void guiSizeCopy(GUIsize *src, GUIsize *dst)
{
    dst->width = src->width;
    dst->height = src->height;
}
static inline void guiRectCopy(GUIrect *src, GUIrect *dst)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->width = src->width;
    dst->height = src->height;
}

#endif // GUI_BASE_H