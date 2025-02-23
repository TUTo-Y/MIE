#include "gui.h"

mat4 PV = GLM_MAT4_IDENTITY_INIT;

void guiSetPV(int w, int h)
{
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    
    // 计算z
    float z = (h / 2.0f) / tanf(glm_rad(45.0f) / 2.0f);

    // 投影操作
    glm_perspective(glm_rad(45.0f),
                    (float)w / (float)h,
                    0.01f,
                    z * 4,
                    projection);

    // 设置视图矩阵
    glm_lookat((vec3){0.0f, 0.0f, z},
               (vec3){0.0f, 0.0f, 0.0f},
               (vec3){0.0f, 1.0f, 0.0f},
               view);

    // 复合
    glm_mat4_mul(projection, view, PV);
}