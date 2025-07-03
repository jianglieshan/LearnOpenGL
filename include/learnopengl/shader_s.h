//
// Created by jiangzheng on 2025/7/3.
//

#ifndef SHADER_S_H
#define SHADER_S_H



class shader_s {
public:
    shader_s();
    ~shader_s();
    
    // 获取Shader ID
    unsigned int getID() const { return m_ID; }
    
    // 编译Shader（现在有默认实现）
    bool compile(const char* source) const;
    
protected:
    unsigned int m_ID; // Shader ID
};

// 顶点着色器类
class vertex_shader : public shader_s {
public:
    vertex_shader();
    ~vertex_shader();
};

// 片段着色器类
class fragment_shader : public shader_s {
public:
    fragment_shader();
    ~fragment_shader();
};

#endif //SHADER_S_H
