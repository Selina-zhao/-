#include "PathFinder.h"
#include "MazeRenderer.h"
#include "raylib.h"
#include <iostream>

int main() {
    try {
        // 1. 配置迷宫尺寸（基于BLOCK_SIZE和迷宫行列数）
        const int MAZE_ROWS = 20;  // maze0.txt为10行10列
        const int MAZE_COLS = 20;
        const int WINDOW_WIDTH = /*MAZE_COLS * MazeRenderer::BLOCK_SIZE*/640;
        const int WINDOW_HEIGHT = /*MAZE_ROWS * MazeRenderer::BLOCK_SIZE*/640;

        // 2. 初始化Raylib窗口
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze Game");
        SetTargetFPS(60); // 设置帧率

        // 3. 加载迷宫数据（替换为你的maze0.txt路径）
        Maze maze = MazeParser::loadFromFile("./maze0.txt");

        // 4. 配置纹理路径并加载纹理
        std::unordered_map<BlockType, std::string> texPaths = {
            {BlockType::WALL,    "./resource/wall.png"},
            {BlockType::START,   "./resource/start.png"},
            {BlockType::GROUND,  "./resource/floor.png"},
            {BlockType::END,     "./resource/end.png"},
            {BlockType::GRASS,   "./resource/grass.png"},
            {BlockType::LAVA,    "./resource/lava.png"}
        };
        TextureManager texManager(texPaths);

        // 3. 路径计算
        PathFinder pathFinder(maze);
        std::vector<std::vector<Point>> allPaths = pathFinder.findAllPathsByDFS(); // 所有可达路径
        std::vector<Point> dijkstraPath = pathFinder.findShortestPath(); // Dijkstra最短路径

         //日志输出（调试用）
        TraceLog(LOG_INFO, "找到%d条可达终点的路径", allPaths.size());
        TraceLog(LOG_INFO, "Dijkstra最短路径步数：%d", dijkstraPath.size());

        // 4. 主循环
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // 绘制层级：先迷宫 → 再所有路径（蓝色） → 最后Dijkstra路径（红色，置顶）
            MazeRenderer::drawMaze(maze, texManager);
            MazeRenderer::drawAllDFSPaths(allPaths);
            MazeRenderer::drawDijkstraPath(dijkstraPath);

            EndDrawing();
        }

        // 资源释放
        CloseWindow();
    }
    catch (const std::exception& e) {
        TraceLog(LOG_ERROR, "错误：%s", e.what());
        system("pause");
        return -1;
    }
    return 0;
}