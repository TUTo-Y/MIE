#include "gui_shader.h"

GLuint guiShaderCreate(GLenum shaderType, const char *shaderSource)
{
    if (shaderSource == NULL)
        return 0;

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL); // 附加到着色器对象上
    glCompileShader(shader);                        // 编译着色器

    // 检查编译错误
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        // 顶点，片段，几何
        ERROR("%s着色器编译失败:%s\n", shaderType == GL_VERTEX_SHADER ? "顶点" : (shaderType == GL_FRAGMENT_SHADER ? "片段" : "几何"), infoLog);
        return 0;
    }

    return shader;
}

/**
 * \brief 创建着色器程序
 * \param vertexShaderSource 顶点着色器源码
 * \param fragmentShaderSource 片段着色器源码
 * \param geometryShaderSource 几何着色器源码
 * \return 着色器程序ID, 0表示失败
 */
GLuint guiShaderCreateProgram(const char *vertexShaderSource, const char *fragmentShaderSource, const char *geometryShaderSource)
{
    if (vertexShaderSource == NULL || fragmentShaderSource == NULL)
    {
        ERROR("着色器程序创建失败，参数错误");
        return 0;
    }

    // 创建顶点着色器
    GLuint vertexShader = guiShaderCreate(GL_VERTEX_SHADER, vertexShaderSource);
    if (vertexShader == 0)
    {
        ERROR("顶点着色器创建失败");
        return 0;
    }

    // 创建片段着色器
    GLuint fragmentShader = guiShaderCreate(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (fragmentShader == 0)
    {
        ERROR("片段着色器创建失败");
        return 0;
    }

    GLuint geometryShader = guiShaderCreate(GL_GEOMETRY_SHADER, geometryShaderSource);

    // 创建着色器程序
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (geometryShader != 0)
        glAttachShader(program, geometryShader);
    glLinkProgram(program);

    // 检查链接错误
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        ERROR("着色器程序链接失败:%s\n", infoLog);
        return 0;
    }

    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != 0)
        glDeleteShader(geometryShader);

    guiShaderUse(program);
    return program;
}
