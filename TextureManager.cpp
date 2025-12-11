#include "TextureManager.h"

// 构造：加载所有纹理
TextureManager::TextureManager(const std::unordered_map<BlockType, std::string>& texPaths) {
    for (const auto& pair : texPaths) {
        BlockType type = pair.first;
        const std::string& path = pair.second;

        // 加载纹理
        Texture2D tex = LoadTexture(path.c_str());
        if (tex.id == 0) { // 纹理加载失败
            throw std::runtime_error("Failed to load texture: " + path);
        }

        textureMap[type] = tex;
    }
}

// 析构：释放所有纹理
TextureManager::~TextureManager() {
    for (auto& pair : textureMap) {
        UnloadTexture(pair.second);
    }
    textureMap.clear();
}

// 获取指定地块类型的纹理
Texture2D TextureManager::getTexture(BlockType type) const {
    auto it = textureMap.find(type);
    if (it == textureMap.end()) {
        throw std::runtime_error("No texture found for block type!");
    }
    return it->second;
}