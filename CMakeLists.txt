cmake_minimum_required(VERSION 3.22)
project(CG-Algorithm)

# 统一把 external 下的 include 提升为系统头文件
add_library(ext_glad STATIC external/glad/src/glad.c)
target_include_directories(ext_glad SYSTEM PUBLIC external/glad/include)


add_library(ext_assimp INTERFACE IMPORTED)
target_include_directories(ext_assimp SYSTEM INTERFACE external/assimp/include)
target_link_libraries(ext_assimp INTERFACE
                      ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/lib/assimp-vc143-mt.lib)

add_library(ext_glfw INTERFACE IMPORTED)
target_include_directories(ext_glfw SYSTEM INTERFACE external/glfw/include)
target_link_libraries(ext_glfw INTERFACE
                      ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib/glfw3.lib)

# 纯头文件库
add_library(ext_glm INTERFACE IMPORTED)
target_include_directories(ext_glm SYSTEM INTERFACE external/glm)

add_library(ext_stb INTERFACE IMPORTED)
target_include_directories(ext_stb SYSTEM INTERFACE external/stb)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin)

# 构建顺序
add_subdirectory(mylib)
add_subdirectory(algos)
