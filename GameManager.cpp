#include "GameManager.h"
#include "raylib.h"
#include <string>

// 处理按键输入
void GameManager::handleInput() {
    switch (gameState) {
        case GameState::START_SCREEN:
            // 按空格键开始游戏（进阶任务要求）
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = GameState::PLAYING;
            }
            break;

        case GameState::PLAYING:
            // 按R键重置游戏
            if (IsKeyPressed(KEY_R)) {
                player.reset(findStartPoint(maze));
            }
            break;

        case GameState::WIN:
        case GameState::GAME_OVER:
            // 按R键重新开始，ESC键退出
            if (IsKeyPressed(KEY_R)) {
                player.reset(findStartPoint(maze));
                gameState = GameState::PLAYING;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
            }
            break;
    }
}

// 更新游戏状态
void GameManager::update(float deltaTime) {
    if (gameState != GameState::PLAYING) return;

    // 更新小人位置
    player.update(maze, deltaTime);

    // 胜利判定：到达终点（进阶任务要求）
    Point playerPos = player.getPosition();
    if (maze.mapData[playerPos.row][playerPos.col] == BlockType::END) {
        gameState = GameState::WIN;
    }

    // 失败判定：第二次踩熔岩（进阶任务要求）
    if (player.getLavaStepCount() >= 2) {
        gameState = GameState::GAME_OVER;
    }
}

// 绘制游戏内容
void GameManager::draw() const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (gameState) {
        case GameState::START_SCREEN:
            // 绘制开始界面（进阶任务要求）
            DrawText("MAZE GAME", GetScreenWidth()/2 - 80, GetScreenHeight()/2 - 50, 40, DARKBLUE);
            DrawText("Press SPACE to Start", GetScreenWidth()/2 - 100, GetScreenHeight()/2 + 20, 20, GRAY);
            DrawText("Press R to Reset | Avoid Lava (2 steps = Game Over)", GetScreenWidth()/2 - 190, GetScreenHeight()/2 + 50, 16, LIGHTGRAY);
            break;

        case GameState::PLAYING:
            // 绘制层级：仅迷宫 + 小人（无路径颜色标注）
            MazeRenderer::drawMaze(maze, texManager);
            player.draw();

            // 简化UI提示
            DrawText(("Lava Steps: " + std::to_string(player.getLavaStepCount()) + "/2").c_str(), 10, 10, 16, RED);
            DrawText("WASD/Arrow Keys to Move", 10, 40, 14, GRAY);
            break;

        case GameState::WIN:
            // 绘制胜利界面（进阶任务要求）
            MazeRenderer::drawMaze(maze, texManager);
            player.draw();
            DrawRectangle(GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 80, 300, 160, Color{0, 255, 0, 180});
            DrawText("YOU WIN!", GetScreenWidth()/2 - 60, GetScreenHeight()/2 - 30, 32, WHITE);
            DrawText("Press R to Restart", GetScreenWidth()/2 - 70, GetScreenHeight()/2 + 20, 18, WHITE);
            DrawText("Press ESC to Exit", GetScreenWidth()/2 - 65, GetScreenHeight()/2 + 50, 18, WHITE);
            break;

        case GameState::GAME_OVER:
            // 绘制失败界面（进阶任务要求）
            MazeRenderer::drawMaze(maze, texManager);
            player.draw();
            DrawRectangle(GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 80, 300, 160, Color{255, 0, 0, 180});
            DrawText("GAME OVER", GetScreenWidth()/2 - 70, GetScreenHeight()/2 - 30, 32, WHITE);
            DrawText("Too Many Lava Steps!", GetScreenWidth()/2 - 90, GetScreenHeight()/2 + 20, 18, WHITE);
            DrawText("Press R to Restart", GetScreenWidth()/2 - 70, GetScreenHeight()/2 + 50, 18, WHITE);
            break;
    }

    EndDrawing();
}