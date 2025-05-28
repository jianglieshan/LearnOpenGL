cmake_minimum_required(VERSION 3.31)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 11)

set(GLAD_SRC ${CMAKE_SOURCE_DIR}/src/glad.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(LearnOpenGL src/main.cpp ${GLAD_SRC})

# 现代CMake链接方式（自动处理头文件和链接）
target_link_libraries(LearnOpenGL PRIVATE
        glfw3  # 直接使用GLFW官方目标名
        opengl32
)