add_library(mylib STATIC)
file(GLOB MYLIB_SRC CONFIGURE_DEPENDS src/*.cpp)
target_sources(mylib PRIVATE ${MYLIB_SRC})
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# 需要时可链接第三方
target_link_libraries(mylib 
    PUBLIC  ext_assimp ext_glad ext_glfw ext_stb ext_glm
)    

