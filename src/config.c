#include "config.h"

cfg config = {
    .default_ttf_path = "resource\\Y-B008YeZiGongChangDanDanHei-2.ttf",
    .loginback_path = "resource\\loginback.jpg",
    .img_vert = "resource\\img.vert",
    .img_frag = "resource\\img.frag",
    .rrt_vert = "resource\\rrt.vert",
    .rrt_frag = "resource\\rrt.frag",
    .rrc_vert = "resource\\rrc.vert",
    .rrc_frag = "resource\\rrc.frag",
    .rt_vert = "resource\\rt.vert",
    .rt_frag = "resource\\rt.frag",
    .rc_vert = "resource\\rc.vert",
    .rc_frag = "resource\\rc.frag",
    .gaussblur_vert = "resource\\gaussblur.vert",
    .gaussblur_frag = "resource\\gaussblur.frag",
    .icon_vert = "resource\\icon.vert",
    .icon_frag = "resource\\icon.frag",
    .font_vert = "resource\\font.vert",
    .font_frag = "resource\\font.frag",
    .cc_vert = "resource\\cc.vert",
    .cc_frag = "resource\\cc.frag",
    .cc2_vert = "resource\\cc2.vert",
    .cc2_frag = "resource\\cc2.frag",
    .user_icon_path = "resource\\user.png",
    .pass_icon_path = "resource\\pass.png",
};

bool confInit(const char *configFile)
{
    FILE *fp = fopen(configFile, "r");
    if (fp == NULL)
    {
        // 创建默认配置文件
        DEBUG("创建新的配置文件...\n");
        fp = fopen(configFile, "w");
        if (fp == NULL)
        {
            ERROR("创建配置文件失败\n");
            return false;
        }
        DEBUG("写入默认配置...\n");

        // 写入新的配置文件
        fprintf(fp, "default_ttf_path = %s\n", config.default_ttf_path);
        fprintf(fp, "loginback_path = %s\n", config.loginback_path);
        fprintf(fp, "img_vert = %s\n", config.img_vert);
        fprintf(fp, "img_frag = %s\n", config.img_frag);
        fprintf(fp, "rrt_vert = %s\n", config.rrt_vert);
        fprintf(fp, "rrt_frag = %s\n", config.rrt_frag);
        fprintf(fp, "rrc_vert = %s\n", config.rrc_vert);
        fprintf(fp, "rrc_frag = %s\n", config.rrc_frag);
        fprintf(fp, "rrc_vert = %s\n", config.rrc_vert);
        fprintf(fp, "rrc_frag = %s\n", config.rrc_frag);
        fprintf(fp, "rt_vert = %s\n", config.rt_vert);
        fprintf(fp, "rt_frag = %s\n", config.rt_frag);
        fprintf(fp, "rc_vert = %s\n", config.rc_vert);
        fprintf(fp, "rc_frag = %s\n", config.rc_frag);
        fprintf(fp, "gaussblur_vert = %s\n", config.gaussblur_vert);
        fprintf(fp, "gaussblur_frag = %s\n", config.gaussblur_frag);
        fprintf(fp, "icon_vert = %s\n", config.icon_vert);
        fprintf(fp, "icon_frag = %s\n", config.icon_frag);
        fprintf(fp, "font_vert = %s\n", config.font_vert);
        fprintf(fp, "font_frag = %s\n", config.font_frag);
        fprintf(fp, "cc_vert = %s\n", config.cc_vert);
        fprintf(fp, "cc_frag = %s\n", config.cc_frag);
        fprintf(fp, "cc2_vert = %s\n", config.cc2_vert);
        fprintf(fp, "cc2_frag = %s\n", config.cc2_frag);
        fprintf(fp, "user_icon_path = %s\n", config.user_icon_path);
        fprintf(fp, "pass_icon_path = %s\n", config.pass_icon_path);

        fclose(fp);
        return true;
    }

    // 读取配置文件
    char line[128];
    char key[64];
    char value[64];
    size_t lineCount = 0;

    while (fgets(line, 64, fp) != NULL)
    {
        lineCount++;

        // 检查当前行是否由空字符组成
        if (strspn(line, " \t\r\n") == strlen(line))
        {
            continue;
        }

        // 去除行尾的换行符
        line[strcspn(line, "\n")] = 0;

        // 解析配置
        if (sscanf(line, "%s = %s", key, value) != 2)
        {
            ERROR("第[%2d]行错误配置 : %s\n", lineCount, line);
            continue;
        }

        // 匹配配置
        if (strcmp(key, "loginback_path") == 0)
        {
            strcpy(config.loginback_path, value);
            continue;
        }
        else if (strcmp(key, "default_ttf_path") == 0)
        {
            strcpy(config.default_ttf_path, value);
            continue;
        }
        else if (strcmp(key, "img_vert") == 0)
        {
            strcpy(config.img_vert, value);
            continue;
        }
        else if (strcmp(key, "img_frag") == 0)
        {
            strcpy(config.img_frag, value);
            continue;
        }
        else if (strcmp(key, "rrt_vert") == 0)
        {
            strcpy(config.rrt_vert, value);
            continue;
        }
        else if (strcmp(key, "rrt_frag") == 0)
        {
            strcpy(config.rrt_frag, value);
            continue;
        }
        else if (strcmp(key, "rrc_vert") == 0)
        {
            strcpy(config.rrc_vert, value);
            continue;
        }
        else if (strcmp(key, "rrc_frag") == 0)
        {
            strcpy(config.rrc_frag, value);
            continue;
        }
        else if (strcmp(key, "rt_vert") == 0)
        {
            strcpy(config.rt_vert, value);
            continue;
        }
        else if (strcmp(key, "rt_frag") == 0)
        {
            strcpy(config.rt_frag, value);
            continue;
        }
        else if (strcmp(key, "rc_vert") == 0)
        {
            strcpy(config.rc_vert, value);
            continue;
        }
        else if (strcmp(key, "rc_frag") == 0)
        {
            strcpy(config.rc_frag, value);
            continue;
        }
        else if (strcmp(key, "gaussblur_vert") == 0)
        {
            strcpy(config.gaussblur_vert, value);
            continue;
        }
        else if (strcmp(key, "gaussblur_frag") == 0)
        {
            strcpy(config.gaussblur_frag, value);
            continue;
        }
        else if (strcmp(key, "icon_vert") == 0)
        {
            strcpy(config.icon_vert, value);
            continue;
        }
        else if (strcmp(key, "icon_frag") == 0)
        {
            strcpy(config.icon_frag, value);
            continue;
        }
        else if (strcmp(key, "font_vert") == 0)
        {
            strcpy(config.font_vert, value);
            continue;
        }
        else if (strcmp(key, "font_frag") == 0)
        {
            strcpy(config.font_frag, value);
            continue;
        }
        else if (strcmp(key, "cc_vert") == 0)
        {
            strcpy(config.cc_vert, value);
            continue;
        }
        else if (strcmp(key, "cc_frag") == 0)
        {
            strcpy(config.cc_frag, value);
            continue;
        }
        else if (strcmp(key, "cc2_vert") == 0)
        {
            strcpy(config.cc2_vert, value);
            continue;
        }
        else if (strcmp(key, "cc2_frag") == 0)
        {
            strcpy(config.cc2_frag, value);
            continue;
        }
        else if (strcmp(key, "user_icon_path") == 0)
        {
            strcpy(config.user_icon_path, value);
            continue;
        }
        else if (strcmp(key, "pass_icon_path") == 0)
        {
            strcpy(config.pass_icon_path, value);
            continue;
        }
        else
        {
            ERROR("第[%2d]行错误配置 : %s\n", lineCount, line);
            ERROR("%s %s\n", key, value);
        }
    }

    fclose(fp);
    return true;
}