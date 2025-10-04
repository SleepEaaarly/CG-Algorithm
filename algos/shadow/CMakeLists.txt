add_executable(shadow
    shadow.cpp
)

# 1. 让编译器找得到 test 目录下的头文件
target_include_directories(shadow PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 2. 链接库
target_link_libraries(shadow PRIVATE mylib ext_glad ext_glfw ext_assimp ext_stb)

# 3. 拷贝 DLL（Windows）
if(WIN32)
    add_custom_command(TARGET shadow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/external/assimp/lib/assimp-vc143-mt.dll
        $<TARGET_FILE_DIR:shadow>)
endif()