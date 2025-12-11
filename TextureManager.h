#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include "raylib.h"
#include "MazeParser.h"
#include <unordered_map>
#include <string>
#include <stdexcept>

// 纹理管理类（封装纹理加载、映射、释放）
class TextureManager {
public:
    // 构造：加载所有纹理（传入纹理路径映射表）
    TextureManager(const std::unordered_map<BlockType, std::string>& texPaths);
    // 析构：释放所有加载的纹理
    ~TextureManager();

    // 获取指定地块类型的纹理
    Texture2D getTexture(BlockType type) const;

    // 禁用拷贝（避免纹理重复释放）
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

private:
    // 地块类型 → 纹理的映射表
    std::unordered_map<BlockType, Texture2D> textureMap;
};

#endif // TEXTURE_MANAGER_H