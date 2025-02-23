#include "config.h"

cfg config = {
    .width = 1000,
    .height = 618,
    .res_path = "resource\\"};

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
        fprintf(fp, "width = %d\n", config.width);
        fprintf(fp, "height = %d\n", config.height);
        fprintf(fp, "res_path = %s\n", config.res_path);

        fclose(fp);
        return true;
    }

    // 读取配置文件
    char line[64];
    char key[32];
    char value[32];
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
        if (strcmp(key, "width") == 0)
        {
            config.width = atoi(value);
            continue;
        }
        else if (strcmp(key, "height") == 0)
        {
            config.height = atoi(value);
            continue;
        }
        else if (strcmp(key, "res_path") == 0)
        {
            strcpy(config.res_path, value);
            continue;
        }
        else
        {
            ERROR("第[%2d]行错误配置 : %s\n", lineCount, line);
        }
    }

    fclose(fp);
    return true;
}