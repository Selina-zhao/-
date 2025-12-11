#include "MazeRenderer.h"

// 计算地块的绘制坐标（col→X轴，row→Y轴）
Vector2 MazeRenderer::getBlockPosition(int row, int col) {
    return {
        static_cast<float>(col * MazeRenderer::BLOCK_SIZE),
        static_cast<float>(row * MazeRenderer::BLOCK_SIZE)
    };
}

// 绘制整个迷宫（纹理，基础任务核心功能）
void MazeRenderer::drawMaze(const Maze& maze, const TextureManager& texManager) {
    for (int row = 0; row < maze.rows; ++row) {
        for (int col = 0; col < maze.cols; ++col) {
            BlockType type = maze.mapData[row][col];
            Texture2D tex = texManager.getTexture(type);
            Vector2 pos = getBlockPosition(row, col);
            // 修正Raylib API参数：DrawTexture接受int坐标，避免类型转换警告
            DrawTexture(tex, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
        }
    }
}