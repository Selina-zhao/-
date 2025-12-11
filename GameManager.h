#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "MazeParser.h"
#include "Player.h"
#include "PathFinder.h"
#include "TextureManager.h"
#include "MazeRenderer.h"
#include "raylib.h" // 新增：需要包含raylib头文件以使用Texture2D

enum class GameState {
    START_SCREEN,  // 开始界面
    PLAYING,       // 游戏中
    WIN,           // 胜利
    GAME_OVER      // 游戏失败
};

class GameManager {
public:
    // 构造函数声明（保持不变）
    GameManager(const Maze& maze, const TextureManager& texManager, const std::string& playerTexPath);

    // 新增：析构函数声明
    ~GameManager();

    // 处理输入（逻辑不变）
    void handleInput();
    // 更新游戏状态（逻辑不变）
    void update(float deltaTime);
    // 绘制游戏内容（逻辑不变）
    void draw() const;

private:
    const Maze& maze;
    const TextureManager& texManager;
    Player player;
    PathFinder pathFinder;
    GameState gameState;
    Texture2D startBgTexture; // 存储背景图纹理

    // 查找迷宫起点（逻辑不变）
    Point findStartPoint(const Maze& maze) const {
        for (int row = 0; row < maze.rows; ++row) {
            for (int col = 0; col < maze.cols; ++col) {
                if (maze.mapData[row][col] == BlockType::START) {
                    return { row, col };
                }
            }
        }
        throw std::runtime_error("Start point not found in maze!");
    }
};
#endif // GAME_MANAGER_H