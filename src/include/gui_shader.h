/**
 * @file guishader.h
 * @brief GUI着色器渲染
 */
#ifndef GUI_SHADER_H
#define GUI_SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

#define guiShaderUse(program) glUseProgram(program)
#define guiShaderUniform(program, name, ntype, ...) glUniform##ntype(glGetUniformLocation(program, name), __VA_ARGS__)
#define guiShaderUniformMatrix(program, name, ntype, ...) glUniformMatrix##ntype(glGetUniformLocation(program, name), 1, GL_FALSE, __VA_ARGS__)
// guiShaderUniform(program, "color", 4f, 1.0f, 0.0f, 0.0f, 1.0f);
// guiShaderUniformMatrix(program, "transform", 4fv, transform);
#define guiShaderDelete(program) glDeleteProgram(program)

/**
 * \brief 创建着色器
 * \param shaderType 着色器类型
 * \param shaderSource 着色器源码
 * \return 着色器
 */
GLuint guiShaderCreate(GLenum shaderType, const char* shaderSource);

/**
 * \brief 创建着色器程序
 * \param vertexShaderSource 顶点着色器源码
 * \param fragmentShaderSource 片段着色器源码
 * \param geometryShaderSource 几何着色器源码
 * \return 着色器程序ID, 0表示失败
 */
GLuint guiShaderCreateProgram(const char *vertexShaderSource, const char *fragmentShaderSource, const char *geometryShaderSource);


#endif // GUI_SHADER_H