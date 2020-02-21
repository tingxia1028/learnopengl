cmake_minimum_required(VERSION 3.15)
project(opengl)

set(CMAKE_CXX_STANDARD 11)

# 添加头文件
set(GLFW_H /usr/local/Cellar/glfw/3.3.1/include)
set(INCLUDE_H /usr/local/include)
include_directories(${GLFW_H} ${INCLUDE_H})

# 添加目标链接
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.1/lib/libglfw.3.dylib)
link_libraries(${GLFW_LINK})

# 执行编译命令
add_executable(opengl src/main.cpp src/glad.c src/renderengine/shader.cpp src/renderengine/shader.h src/model/rawmodel.cpp src/model/rawmodel.h src/renderengine/loader.cpp src/renderengine/loader.h src/renderengine/render.cpp src/renderengine/render.h src/renderengine/DisplayManager.cpp src/renderengine/DisplayManager.h src/renderengine/stb_image.cpp)
