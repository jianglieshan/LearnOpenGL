cmake_minimum_required(VERSION 3.31)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

# 收集 src 目录下的所有 .c 和 .cpp 文件
file(GLOB SRC_FILES  "src/*.c" "src/*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(LearnOpenGL ${SRC_FILES})

target_link_directories(LearnOpenGL PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# 现代CMake链接方式（自动处理头文件和链接）
target_link_libraries(LearnOpenGL PRIVATE
        glfw3  # 直接使用GLFW官方目标名
        opengl32
)

# 定义资源文件列表
file(GLOB RESOURCES  RELATIVE "${CMAKE_SOURCE_DIR}" "resources/*.*")

# 复制资源文件到构建目录
foreach(resource ${RESOURCES})
    get_filename_component(dest_dir "${CMAKE_BINARY_DIR}/${resource}" DIRECTORY)

    # 确保目标目录存在
    file(MAKE_DIRECTORY ${dest_dir})

    configure_file(
            ${CMAKE_SOURCE_DIR}/${resource}
            ${CMAKE_BINARY_DIR}/${resource}
            COPYONLY
    )
endforeach()

# 针对 MinGW 的额外配置
if(MINGW)
    target_link_options(LearnOpenGL PRIVATE "-mwindows")
endif()