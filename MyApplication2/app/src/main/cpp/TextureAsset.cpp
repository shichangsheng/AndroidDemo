#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureAsset.h"
#include "AndroidOut.h"
#include "Utility.h"

std::shared_ptr<TextureAsset>
TextureAsset::loadAsset(AAssetManager *assetManager, const std::string &assetPath) {
    // 1. 打开资源文件
    AAsset* asset = AAssetManager_open(
            assetManager,
            assetPath.c_str(),
            AASSET_MODE_BUFFER);
    if (!asset) {
        aout << "Failed to open asset: " << assetPath << std::endl;
        return nullptr;
    }

    // 2. 读取资源数据到内存
    const void* assetData = AAsset_getBuffer(asset);
    const off_t assetLength = AAsset_getLength(asset);

    // 3. 使用stb_image加载图像
    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(assetData),
            static_cast<int>(assetLength),
            &width,
            &height,
            &channels,
            STBI_rgb_alpha); // 强制RGBA格式

    if (!imageData) {
        aout << "Failed to load image: " << stbi_failure_reason() << std::endl;
        AAsset_close(asset);
        return nullptr;
    }

    // 4. 创建OpenGL纹理
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 上传纹理数据
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            imageData
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    // 5. 清理资源
    stbi_image_free(imageData);
    AAsset_close(asset);

    return std::make_shared<TextureAsset>(textureId);
}

TextureAsset::~TextureAsset() {
    glDeleteTextures(1, &textureID_);
    textureID_ = 0;
}