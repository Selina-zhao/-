#include "MazeRenderer.h"


Vector2 MazeRenderer::getBlockPosition(int row, int col) {
    // 行列坐标正确映射（col→X，row→Y），避免迷宫错位
    return {
        static_cast<float>(col * MazeRenderer::BLOCK_SIZE),
        static_cast<float>(row * MazeRenderer::BLOCK_SIZE)
    };
}

// 绘制基础迷宫（保留原逻辑）
void MazeRenderer::drawMaze(const Maze& maze, const TextureManager& texManager) {
    TraceLog(LOG_INFO, "进入drawMaze，迷宫行列数：%d行%d列", maze.rows, maze.cols);
    for (int row = 0; row < maze.rows; ++row) {
        for (int col = 0; col < maze.cols; ++col) {
            BlockType type = maze.mapData[row][col];
            Texture2D tex = texManager.getTexture(type);
            Vector2 pos = getBlockPosition(row, col);
            DrawTexture(tex, pos.x, pos.y, WHITE);
        }
    }
}


// 绘制Dijkstra加权最短路径（红色，醒目）
void MazeRenderer::drawDijkstraPath(const std::vector<Point>& path) {
    if (path.empty()) return;
    for (const auto& point : path) {
        Vector2 pos = getBlockPosition(point.row, point.col);
        // 红色半透明块（覆盖地块，不遮挡纹理）
        DrawRectangle(pos.x + 2, pos.y + 2,
            BLOCK_SIZE - 4, BLOCK_SIZE - 4,
            Color{ 255, 182, 193, 255 });
        // 红色中心点（强化标记）
        //DrawCircle(pos.x + BLOCK_SIZE / 2, pos.y + BLOCK_SIZE / 2, 5, RED);
    }
}

// 绘制所有DFS可达路径（统一蓝色，半透明）
void MazeRenderer::drawAllDFSPaths(const std::vector<std::vector<Point>>& allPaths) {
    if (allPaths.empty()) return;
    for (auto& path : allPaths) {
        for (const auto& point : path) {
            Vector2 pos = getBlockPosition(point.row, point.col);
            // 蓝色半透明块（透明度更低，避免遮盖Dijkstra路径）
            DrawRectangle(pos.x + 4, pos.y + 4,
                BLOCK_SIZE - 8, BLOCK_SIZE - 8,
                Color{ 100, 149, 237, 255 });
        }
        // 绘制路径线条（蓝色细线条，连接路径点）
        for (int i = 1; i < path.size(); ++i) {
            Vector2 p1 = getBlockPosition(path[i - 1].row, path[i - 1].col);
            Vector2 p2 = getBlockPosition(path[i].row, path[i].col);
            DrawLine(p1.x + BLOCK_SIZE / 2, p1.y + BLOCK_SIZE / 2,
                p2.x + BLOCK_SIZE / 2, p2.y + BLOCK_SIZE / 2,
                Color{ 0, 0, 255, 120 });
        }
    }
}