# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.6)

project("myapplication")

# 设置 NDK 路径（可选，如果 CMake 找不到 NDK）
set(ANDROID_NDK "D:/Android/android-ndk-r21e")

# 添加头文件路径
include_directories(
        src/main/cpp/native_app_glue/  # 本地头文件
)

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(myapplication SHARED
        main.cpp
        AndroidOut.cpp
        Renderer.cpp
        Shader.cpp
        TextureAsset.cpp
        Utility.cpp
        native_app_glue/android_native_app_glue.c)

#target_compile_options(myapplication PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#set_target_properties(myapplication PROPERTIES LINK_FLAGS -fsanitize=address)

# Configure libraries CMake uses to link your target library.
target_link_libraries(myapplication
        #android_native_app_glue
        EGL
        GLESv3
        jnigraphics
        android
        log)
