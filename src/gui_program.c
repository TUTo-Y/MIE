#include "gui_program.h"

GUIprogram program = {0};

/**
 * \brief 初始化所有的着色器程序
 */
bool guiProgramInit()
{
    // 获取pv
    mat4 PV;
    guiProgramSetPV(WINDOW_WIDTH, WINDOW_HEIGHT, PV);

    // 加载基础图像着色器
    if (0 == (program.img = guiShaderCreateProgram(resGetFile(config.img_vert, NULL, NULL, false),
                                                   resGetFile(config.img_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.img);
    guiShaderUniform(program.img, "Texture", 1i, 0);
    guiShaderUniformMatrix(program.img, "PV", 4fv, (float *)PV);

    // 加载圆角纹理矩形着色器
    if (0 == (program.rrt = guiShaderCreateProgram(resGetFile(config.rrt_vert, NULL, NULL, false),
                                                   resGetFile(config.rrt_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.rrt);
    guiShaderUniformMatrix(program.rrt, "PV", 4fv, (float *)PV);
    guiShaderUniform(program.rrt, "Texture", 1i, 0);

    // 加载圆角颜色矩形着色器
    if (0 == (program.rrc = guiShaderCreateProgram(resGetFile(config.rrc_vert, NULL, NULL, false),
                                                   resGetFile(config.rrc_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.rrc);
    guiShaderUniformMatrix(program.rrc, "PV", 4fv, (float *)PV);

    // 加载纹理矩形着色器
    if (0 == (program.rt = guiShaderCreateProgram(resGetFile(config.rt_vert, NULL, NULL, false),
                                                  resGetFile(config.rt_frag, NULL, NULL, false),
                                                  NULL)))
        return false;
    guiShaderUse(program.rt);
    guiShaderUniformMatrix(program.rt, "PV", 4fv, (float *)PV);
    guiShaderUniform(program.rt, "Texture", 1i, 0);

    // 加载颜色矩形着色器
    if (0 == (program.rc = guiShaderCreateProgram(resGetFile(config.rc_vert, NULL, NULL, false),
                                                  resGetFile(config.rc_frag, NULL, NULL, false),
                                                  NULL)))
        return false;
    guiShaderUse(program.rc);
    guiShaderUniformMatrix(program.rc, "PV", 4fv, (float *)PV);

    // 加载高斯模糊着色器
    if (0 == (program.gaussblur = guiShaderCreateProgram(resGetFile(config.gaussblur_vert, NULL, NULL, false),
                                                         resGetFile(config.gaussblur_frag, NULL, NULL, false),
                                                         NULL)))
        return false;
    guiShaderUse(program.gaussblur);
    guiShaderUniform(program.gaussblur, "Texture", 1i, 0);

    // 加载图标着色器
    if (0 == (program.icon = guiShaderCreateProgram(resGetFile(config.icon_vert, NULL, NULL, false),
                                                    resGetFile(config.icon_frag, NULL, NULL, false),
                                                    NULL)))
        return false;
    guiShaderUse(program.icon);
    guiShaderUniformMatrix(program.icon, "PV", 4fv, (float *)PV);
    guiShaderUniform(program.icon, "Texture", 1i, 0);

    return true;
}

/**
 * \brief 销毁所有的着色器程序
 */
void guiProgramDestroy()
{
    // 删除基础图像着色器
    glDeleteProgram(program.img);
}

void guiProgramSetPV(int w, int h, mat4 PV)
{
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;

    float z = (h / 2.0f) / tanf(glm_rad(45.0f) / 2.0f);
    glm_perspective(glm_rad(45.0f),
                    (float)w / (float)h,
                    0.01f,
                    z * 4,
                    projection);
    glm_lookat((vec3){0.0f, 0.0f, z},
               (vec3){0.0f, 0.0f, 0.0f},
               (vec3){0.0f, 1.0f, 0.0f},
               view);
    glm_mat4_mul(projection, view, PV);
}