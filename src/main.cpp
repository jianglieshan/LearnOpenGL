#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <learnopengl/shader_s.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

auto vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        })";

auto vertexShaderSource1 = R"(#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出
uniform float offset; // 在OpenGL程序代码中设定这个变量
        void main()
        {
            gl_Position = vec4(-aPos.x - offset, aPos.y, aPos.z, 1.0);
            vertexColor = gl_Position;
        })";


auto fragmentShaderSource = R"(#version 330 core
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            } )";

auto fragmentYellowShaderSource = R"(#version 330 core
            out vec4 FragColor;
            in vec4 vertexColor;
            void main()
            {
                FragColor = vertexColor;
            } )";


void do_work(unsigned int VAO, unsigned int VBO, unsigned int EBO, float vertices[],int size_v, unsigned int indices[],int size_i) {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size_v, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, indices, GL_STATIC_DRAW);
}

unsigned int getShaderProgram(const char * vertexShaderSource,const char * fragmentShaderSource) {
    const auto *vs = new vertex_shader();
    int vs_res = vs->compile(vertexShaderSource);
    const auto *fs = new fragment_shader();
    int fs_res = fs->compile(fragmentShaderSource);
    if (vs_res && fs_res) {
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs->getID());
        glAttachShader(shaderProgram, fs->getID());
        glLinkProgram(shaderProgram);

        //程序链接以后，即可删除编译产物
        delete vs;
        delete fs;
        return shaderProgram;
    }
    return 0;
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/container.jpg", &width, &height, &nrChannels, 0);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    unsigned int VAO[2],VBO[2],EBO[2];

    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        1.0f,0.0f,0.0f
    };
    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 2, // 第一个三角形
        0, 1, 3,  // 第二个三角形
        1, 3, 4
    };

    do_work(VAO[0], VBO[0], EBO[0], vertices,sizeof(vertices), indices,sizeof(indices));

    float vertices1[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -1.0f,0.0f,0.0f
    };
    unsigned int indices1[] = {
        0, 1, 2, // 第一个三角形
    };
    do_work(VAO[1], VBO[1], EBO[1], vertices1,sizeof(vertices1), indices1,sizeof(indices1));


    unsigned int shaderProgram = getShaderProgram(vertexShaderSource,fragmentShaderSource);

    unsigned int yellowShaderProgram = getShaderProgram(vertexShaderSource1,fragmentYellowShaderSource);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glUseProgram(yellowShaderProgram);

        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float offset = std::sin(timeValue) / 2.0f + 0.5f;
        int offsetLocation = glGetUniformLocation(yellowShaderProgram, "offset");
        glUniform1f(offsetLocation, offset);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteProgram(yellowShaderProgram);
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}