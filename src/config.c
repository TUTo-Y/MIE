#include "config.h"

cfg config = {
    .default_ttf_path = "resource\\Y-B008YeZiGongChangDanDanHei-2.ttf",
    .loginback_path = "resource\\loginback.jpg",
    .img_vert = "resource\\img.vert",
    .img_frag = "resource\\img.frag",
    .loginback_vert = "resource\\loginback.vert",
    .loginback_frag = "resource\\loginback.frag",
    .rrt_vert = "resource\\rrt.vert",
    .rrt_frag = "resource\\rrt.frag",
    .rrc_vert = "resource\\rrc.vert",
    .rrc_frag = "resource\\rrc.frag",
    .gaussblur_vert = "resource\\gaussblur.vert",
    .gaussblur_frag = "resource\\gaussblur.frag"
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
        fprintf(fp, "loginback_vert = %s\n", config.loginback_vert);
        fprintf(fp, "loginback_frag = %s\n", config.loginback_frag);
        fprintf(fp, "rrt_vert = %s\n", config.rrt_vert);
        fprintf(fp, "rrt_frag = %s\n", config.rrt_frag);
        fprintf(fp, "rrc_vert = %s\n", config.rrc_vert);
        fprintf(fp, "rrc_frag = %s\n", config.rrc_frag);
        fprintf(fp, "gaussblur_vert = %s\n", config.gaussblur_vert);
        fprintf(fp, "gaussblur_frag = %s\n", config.gaussblur_frag);

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
        else if (strcmp(key, "loginback_vert") == 0)
        {
            strcpy(config.loginback_vert, value);
            continue;
        }
        else if (strcmp(key, "loginback_frag") == 0)
        {
            strcpy(config.loginback_frag, value);
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
        else
        {
            ERROR("第[%2d]行错误配置 : %s\n", lineCount, line);
            ERROR("%s %s\n", key, value);
        }
    }

    fclose(fp);
    return true;
}