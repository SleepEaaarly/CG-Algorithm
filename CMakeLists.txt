cmake_minimum_required(VERSION 3.10)
project(CG-Algorithm)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加 glad 静态库
add_subdirectory(lib/glad)

# 查找源文件
file(GLOB SRC_FILES src/*.cpp)

# 添加可执行文件
add_executable(main ${SRC_FILES})

# 链接 glad 静态库
target_link_libraries(main glad)

# 链接 glfw 和 assimp 静态库
target_link_libraries(main
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    ${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mt.lib
)

# Windows 平台需要链接 opengl32
if (WIN32)
    target_link_libraries(main opengl32)
endif()