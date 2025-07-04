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

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

# 递归获取所有资源文件
file(GLOB_RECURSE RES_FILES
        RELATIVE ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_SOURCE_DIR}/resources/*)

# 为每个资源创建复制规则
foreach(file ${RES_FILES})
    set(src ${CMAKE_SOURCE_DIR}/resources/${file})
    set(dst ${CMAKE_BINARY_DIR}/resources/${file})
    add_custom_command(
            OUTPUT ${dst}
            COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst}
            DEPENDS ${src}
    )
    list(APPEND ALL_DST ${dst})
endforeach()

add_custom_target(ResourceCopy ALL DEPENDS ${ALL_DST})

add_dependencies(LearnOpenGL ResourceCopy)  # 确保先复制资源

# 针对 MinGW 的额外配置
if(MINGW)
    target_link_options(LearnOpenGL PRIVATE "-mwindows")
endif()