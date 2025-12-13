#include "MazeParser.h"
#include "TextureManager.h"
#include "GameManager.h"
#include "raylib.h"
#include <iostream>

int main() {
    try {
        // ================= 基础配置 =================
        const int WINDOW_WIDTH = 640;
        const int WINDOW_HEIGHT = 640;
        const std::string MAZE_FILE = "./maze0.txt";
        // 新小人纹理路径
        const std::string PLAYER_TEX_PATH = "./resource/character.png";

        // ================= 初始化窗口 =================
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze Game - Full Task");
        SetTargetFPS(60);

        // ================= 加载迷宫 =================
        Maze maze = MazeParser::loadFromFile(MAZE_FILE);
        if (maze.rows != 20 || maze.cols != 20) {
            throw std::runtime_error("Maze must be 20×20 (task requirement)!");
        }

        // ================= 配置纹理路径 =================
        std::unordered_map<BlockType, std::string> texPaths = {
            {BlockType::WALL,    "./resource/wall.png"},
            {BlockType::START,   "./resource/start.png"},
            {BlockType::GROUND,  "./resource/floor.png"},
            {BlockType::END,     "./resource/end.png"},
            {BlockType::GRASS,   "./resource/grass.png"},
            {BlockType::LAVA,    "./resource/lava.png"}
        };

        // ================= 加载纹理 =================
        TextureManager texManager(texPaths);

        // ================= 初始化游戏管理器：传入小人纹理路径 =================
        GameManager gameManager(maze, texManager, PLAYER_TEX_PATH);

        // ================= 主循环 =================
        while (!WindowShouldClose()) {
            gameManager.handleInput();
            gameManager.update(GetFrameTime());
            gameManager.draw();
        }

        // ================= 资源释放 =================
        CloseWindow();
    }
    catch (const std::exception& e) {
        TraceLog(LOG_ERROR, "Error: %s", e.what());
        system("pause");
        return -1;
    }

    return 0;
}