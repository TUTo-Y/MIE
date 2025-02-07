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
#define guiShaderUniformGetLocation(program, name) glGetUniformLocation(program, name)

#define guiShaderUniformFromID(ID, ntype, ...) glUniform##ntype(ID, __VA_ARGS__)
#define guiShaderUniformMatrixFromID(ID, ntype, ...) glUniformMatrix##ntype(ID, 1, GL_FALSE, __VA_ARGS__)

#define guiShaderUniform(program, name, ntype, ...) guiShaderUniformFromID(guiShaderUniformGetLocation(program, name), ntype, __VA_ARGS__)
#define guiShaderUniformMatrix(program, name, ntype, ...) guiShaderUniformMatrixFromID(guiShaderUniformGetLocation(program, name), ntype, __VA_ARGS__)

#define guiShaderDelete(program) glDeleteProgram(program)

/**
 * \brief 创建着色器
 * \param shaderType 着色器类型
 * \param shaderSource 着色器源码
 * \return 着色器
 */
GLuint guiShaderCreate(GLenum shaderType, const char *shaderSource);

/**
 * \brief 创建着色器程序
 * \param vertexShaderSource 顶点着色器源码
 * \param fragmentShaderSource 片段着色器源码
 * \param geometryShaderSource 几何着色器源码
 * \return 着色器程序ID, 0表示失败
 */
GLuint guiShaderCreateProgram(const char *vertexShaderSource, const char *fragmentShaderSource, const char *geometryShaderSource);

#endif // GUI_SHADER_H