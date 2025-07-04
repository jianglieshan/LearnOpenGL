#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出
uniform float offset; // 在OpenGL程序代码中设定这个变量
void main()
{
    gl_Position = vec4(-aPos.x - offset, aPos.y, aPos.z, 1.0);
    vertexColor = gl_Position;
}