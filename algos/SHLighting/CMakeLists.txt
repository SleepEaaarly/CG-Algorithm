add_executable(sh
    sh.cpp
)

# 1. 让编译器找得到 test 目录下的头文件
target_include_directories(sh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 2. 链接库
target_link_libraries(sh PRIVATE mylib ext_glad ext_glfw ext_assimp ext_stb)

# 3. 拷贝 DLL（Windows）
if(WIN32)
    add_custom_command(TARGET sh POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/external/assimp/lib/assimp-vc143-mt.dll
        $<TARGET_FILE_DIR:sh>)
endif()