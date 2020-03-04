cmake_minimum_required(VERSION 3.15)
project(opengl)

set(CMAKE_CXX_STANDARD 11)

# 添加头文件
set(GLFW_H /usr/local/Cellar/glfw/3.3.1/include)
set(INCLUDE_H /usr/local/include)
set(GLM_H /usr/local/Cellar/glm/0.9.9.5/include)
include_directories(${GLFW_H} ${INCLUDE_H} ${GLM_H})

# 添加目标链接
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.1/lib/libglfw.3.dylib)
link_libraries(${GLFW_LINK})

# 执行编译命令
add_executable(opengl src/main.cpp src/glad.c src/renderengine/shader.cpp src/renderengine/shader.h src/renderengine/loader.cpp src/renderengine/loader.h src/renderengine/render.cpp src/renderengine/render.h src/renderengine/displaymanager.cpp src/renderengine/displaymanager.h src/renderengine/stb_image.cpp src/transformation/rotate.cpp src/transformation/rotate.h src/transformation/transformation.cpp src/transformation/transformation.h src/camera/camera.cpp src/camera/camera.h src/model/model.cpp src/model/model.h src/model/Scene.cpp src/model/Scene.h src/light/light.cpp src/light/light.h src/material/material.cpp src/material/material.h src/light/directionallight.cpp src/light/directionallight.h src/light/pointlight.cpp src/light/pointlight.h src/light/spotlight.cpp src/light/spotlight.h src/light/flashlight.cpp src/light/flashlight.h)


if (APPLE)
    target_link_libraries(opengl "-framework OpenGL")
endif()