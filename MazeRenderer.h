#ifndef MAZE_RENDERER_H
#define MAZE_RENDERER_H
#include "MazeParser.h"
#include "TextureManager.h"
#include "PathFinder.h"
#include "raylib.h"
#include <vector>
#include <iostream>

// 迷宫绘制类（封装绘制逻辑）
class MazeRenderer {
public:
    static const int BLOCK_SIZE = 32; // 单个地块像素尺寸（32×32）

    // 绘制整个迷宫（纹理，核心功能）
    static void drawMaze(const Maze& maze, const TextureManager& texManager);

    // 辅助方法：计算地块的绘制坐标（col→X，row→Y）
    static Vector2 getBlockPosition(int row, int col);
};

#endif // MAZE_RENDERER_H