#include "gui_program.h"

#define GUI_PROGRAM_BIND_UNIFORM_BLOCK(p, str) glUniformBlockBinding((p), glGetUniformBlockIndex((p), str), 0)

// program 保存所有的着色器程序
GUIprogram program = {0};

// 默认PV矩阵
static mat4 PV = GLM_MAT4_IDENTITY_INIT;

// 全局PV块
static GLuint uboPVBlock = 0;

bool guiProgramInit()
{
    /**
     * 创建默认PV矩阵和PV块
     */
    guiProgramSetPV(WINDOW_WIDTH, WINDOW_HEIGHT, PV);

    // 加载基础图像着色器
    if (0 == (program.img = guiShaderCreateProgram(resGetFile(config.img_vert, NULL, NULL, false),
                                                   resGetFile(config.img_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.img);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.img, "Matrices");
    guiShaderUniform(program.img, "Texture", 1i, 0);

    // 加载圆角纹理矩形着色器
    if (0 == (program.rrt = guiShaderCreateProgram(resGetFile(config.rrt_vert, NULL, NULL, false),
                                                   resGetFile(config.rrt_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.rrt);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.rrt, "Matrices");
    guiShaderUniform(program.rrt, "Texture", 1i, 0);

    // 加载圆角颜色矩形着色器
    if (0 == (program.rrc = guiShaderCreateProgram(resGetFile(config.rrc_vert, NULL, NULL, false),
                                                   resGetFile(config.rrc_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.rrc);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.rrc, "Matrices");

    // 加载纹理矩形着色器
    if (0 == (program.rt = guiShaderCreateProgram(resGetFile(config.rt_vert, NULL, NULL, false),
                                                  resGetFile(config.rt_frag, NULL, NULL, false),
                                                  NULL)))
        return false;
    guiShaderUse(program.rt);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.rt, "Matrices");
    guiShaderUniform(program.rt, "Texture", 1i, 0);

    // 加载颜色矩形着色器
    if (0 == (program.rc = guiShaderCreateProgram(resGetFile(config.rc_vert, NULL, NULL, false),
                                                  resGetFile(config.rc_frag, NULL, NULL, false),
                                                  NULL)))
        return false;
    guiShaderUse(program.rc);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.rc, "Matrices");

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
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.icon, "Matrices");
    guiShaderUniform(program.icon, "Texture", 1i, 0);

    // 加载默认字体着色器
    if (0 == (program.font = guiShaderCreateProgram(resGetFile(config.font_vert, NULL, NULL, false),
                                                    resGetFile(config.font_frag, NULL, NULL, false),
                                                    NULL)))
        return false;
    guiShaderUse(program.font);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.font, "Matrices");
    guiShaderUniform(program.font, "Texture", 1i, 0);

    // 加载圆颜色着色器
    if (0 == (program.cc = guiShaderCreateProgram(resGetFile(config.cc_vert, NULL, NULL, false),
                                                  resGetFile(config.cc_frag, NULL, NULL, false),
                                                  NULL)))
        return false;
    guiShaderUse(program.cc);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.cc, "Matrices");

    // 加载圆颜色2着色器
    if (0 == (program.cc2 = guiShaderCreateProgram(resGetFile(config.cc2_vert, NULL, NULL, false),
                                                   resGetFile(config.cc2_frag, NULL, NULL, false),
                                                   NULL)))
        return false;
    guiShaderUse(program.cc2);
    GUI_PROGRAM_BIND_UNIFORM_BLOCK(program.cc2, "Matrices");

    // 创建PV块
    glGenBuffers(1, &uboPVBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, uboPVBlock);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4), &PV, GL_STATIC_DRAW);//GL_DYNAMIC_DRAW
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboPVBlock);
    return true;
}

void guiProgramDestroy()
{
    // 删除基础图像着色器
    glDeleteProgram(program.img);
}

void guiProgramUpdatePV(mat4 pv)
{
    glBindBuffer(GL_UNIFORM_BUFFER, uboPVBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &pv);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboPVBlock);
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