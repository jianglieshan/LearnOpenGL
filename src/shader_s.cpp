//
// Created by jiangzheng on 2025/7/3.
//

#include "learnopengl/shader_s.h"
#include <glad/glad.h>
#include <iostream>


// 构造函数
shader_s::shader_s() : m_ID(0) {
}

// 析构函数
shader_s::~shader_s() {
    if (m_ID != 0) {
        glDeleteShader(m_ID);
    }
}

bool shader_s::compile(const char* source) const {
    if (m_ID == 0) return false;

    glShaderSource(m_ID, 1, &source, NULL);
    glCompileShader(m_ID);

    // 检查编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "shader compilation failed: " << infoLog << std::endl;
        return false;
    }

    return true;
}

// 顶点着色器实现
vertex_shader::vertex_shader() {
    m_ID = glCreateShader(GL_VERTEX_SHADER);
}

vertex_shader::~vertex_shader() {
    // 使用基类析构函数处理资源释放
    shader_s::~shader_s();
}

// 片段着色器实现
fragment_shader::fragment_shader() {
    m_ID = glCreateShader(GL_FRAGMENT_SHADER);
}

fragment_shader::~fragment_shader() {
    // 使用基类析构函数处理资源释放
    shader_s::~shader_s();
}
