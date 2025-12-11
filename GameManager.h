#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "MazeParser.h"
#include "Player.h"
#include "PathFinder.h"
#include "TextureManager.h"
#include "MazeRenderer.h"

enum class GameState {
    START_SCREEN,  // 开始界面
    PLAYING,       // 游戏中
    WIN,           // 胜利
    GAME_OVER      // 游戏失败
};

class GameManager {
public:
    // 构造：新增小人纹理路径参数（传递给Player）
    GameManager(const Maze& maze, const TextureManager& texManager, const std::string& playerTexPath)
        : maze(maze), texManager(texManager),
        // 调用修改后的Player构造函数，传入起点和小人纹理路径
        player(findStartPoint(maze), playerTexPath),
        pathFinder(maze),
        gameState(GameState::START_SCREEN) {
    }

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