#include "GameManager.h"
#include "raylib.h"
#include <string>

// 处理按键输入（保持不变）
void GameManager::handleInput() {
    switch (gameState) {
    case GameState::START_SCREEN:
        if (IsKeyPressed(KEY_SPACE)) {
            gameState = GameState::PLAYING;
        }
        break;
    case GameState::PLAYING:
        if (IsKeyPressed(KEY_R)) {
            player.reset(findStartPoint(maze));
        }
        break;
    case GameState::WIN:
    case GameState::GAME_OVER:
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

// 更新游戏状态（保持不变）
void GameManager::update(float deltaTime) {
    if (gameState != GameState::PLAYING) return;

    player.update(maze, deltaTime);

    // 胜利判定（后续可优化，当前保持原逻辑）
    Point playerPos = player.getPosition();
    if (maze.mapData[playerPos.row][playerPos.col] == BlockType::END) {
        gameState = GameState::WIN;
    }

    // 失败判定（保持不变）
    if (player.getLavaStepCount() >= 2) {
        gameState = GameState::GAME_OVER;
    }
}

// 绘制游戏内容（修复WIN分支作用域问题）
void GameManager::draw() const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (gameState) {
    case GameState::START_SCREEN: {
        // 1. 获取屏幕中心坐标
        int screenCenterX = GetScreenWidth() / 2;
        int screenCenterY = GetScreenHeight() / 2;

        // 2. 标题文本："MAZE GAME"（40号字体）
        const char* titleText = "MAZE GAME";
        int titleFontSize = 40;
        // 计算文本宽度（关键：通过MeasureText获取文本实际宽度）
        int titleWidth = MeasureText(titleText, titleFontSize);
        // 计算文本左上角坐标（中心X - 文本宽度/2，垂直向上偏移50像素）
        int titleX = screenCenterX - titleWidth / 2;
        int titleY = screenCenterY - 50;
        // 绘制标题（深蓝色，醒目）
        DrawText(titleText, titleX, titleY, titleFontSize, DARKBLUE);

        // 3. 开始提示："Press SPACE to Start"（20号字体）
        const char* startText = "Press SPACE to Start";
        int startFontSize = 20;
        int startWidth = MeasureText(startText, startFontSize);
        int startX = screenCenterX - startWidth / 2;
        int startY = screenCenterY + 20; // 垂直向下偏移20像素
        DrawText(startText, startX, startY, startFontSize, GRAY);

        // 4. 规则提示："Press R to Reset | Avoid Lava (2 steps = Game Over)"（16号字体）
        const char* ruleText = "Press R to Reset | Avoid Lava (2 steps = Game Over)";
        int ruleFontSize = 16;
        int ruleWidth = MeasureText(ruleText, ruleFontSize);
        int ruleX = screenCenterX - ruleWidth / 2;
        int ruleY = screenCenterY + 50; // 垂直向下偏移50像素
        DrawText(ruleText, ruleX, ruleY, ruleFontSize, LIGHTGRAY);

        break;
    }

    case GameState::PLAYING:
        // 绘制层级：仅迷宫 + 小人（保持不变）
        MazeRenderer::drawMaze(maze, texManager);
        player.draw();
        // 简化UI提示（保持不变）
        DrawText(("Lava Steps: " + std::to_string(player.getLavaStepCount()) + "/2").c_str(), 10, 10, 16, RED);
        DrawText("WASD/Arrow Keys to Move", 10, 40, 14, GRAY);
        break;

        // 核心修复：给WIN分支加上大括号，形成局部作用域
    case GameState::WIN: {
        // 胜利界面（优化：字体居中）
        MazeRenderer::drawMaze(maze, texManager);
        player.draw();
        // 半透明矩形背景（保持原尺寸）
        int winRectX = GetScreenWidth() / 2 - 150;
        int winRectY = GetScreenHeight() / 2 - 80;
        int winRectWidth = 300;
        int winRectHeight = 160;
        DrawRectangle(winRectX, winRectY, winRectWidth, winRectHeight, Color{ 0, 255, 0, 180 });

        // 计算矩形中心（用于文本居中）
        int winRectCenterX = winRectX + winRectWidth / 2;
        int winRectCenterY = winRectY + winRectHeight / 2;

        // 1. YOU WIN! 文本
        const char* winText = "YOU WIN!";
        int winFontSize = 32;
        int winTextWidth = MeasureText(winText, winFontSize);
        int winTextX = winRectCenterX - winTextWidth / 2;
        int winTextY = winRectCenterY - 40;
        DrawText(winText, winTextX, winTextY, winFontSize, WHITE);

        // 2. Press R to Restart 文本
        const char* winRestartText = "Press R to Restart";
        int winRestartFontSize = 18;
        int winRestartWidth = MeasureText(winRestartText, winRestartFontSize);
        int winRestartX = winRectCenterX - winRestartWidth / 2;
        int winRestartY = winRectCenterY + 10;
        DrawText(winRestartText, winRestartX, winRestartY, winRestartFontSize, WHITE);

        // 3. Press ESC to Exit 文本
        const char* winExitText = "Press ESC to Exit";
        int winExitFontSize = 18;
        int winExitWidth = MeasureText(winExitText, winExitFontSize);
        int winExitX = winRectCenterX - winExitWidth / 2;
        int winExitY = winRectCenterY + 30;
        DrawText(winExitText, winExitX, winExitY, winExitFontSize, WHITE);
        break;
    }

    case GameState::GAME_OVER: {
        // 失败界面（核心修改：字体居中）
        MazeRenderer::drawMaze(maze, texManager);
        player.draw();
        // 半透明红色矩形背景（保持原尺寸）
        int gameOverRectX = GetScreenWidth() / 2 - 150;
        int gameOverRectY = GetScreenHeight() / 2 - 80;
        int gameOverRectWidth = 300;
        int gameOverRectHeight = 160;
        DrawRectangle(gameOverRectX, gameOverRectY, gameOverRectWidth, gameOverRectHeight, Color{ 255, 0, 0, 180 });

        // 计算矩形中心（用于文本居中，关键！）
        int gameOverRectCenterX = gameOverRectX + gameOverRectWidth / 2;
        int gameOverRectCenterY = gameOverRectY + gameOverRectHeight / 2;

        // 1. GAME OVER 文本
        const char* gameOverText = "GAME OVER";
        int gameOverFontSize = 32;
        // 计算文本宽度
        int gameOverTextWidth = MeasureText(gameOverText, gameOverFontSize);
        // 居中计算：矩形中心X - 文本宽度/2
        int gameOverTextX = gameOverRectCenterX - gameOverTextWidth / 2;
        int gameOverTextY = gameOverRectCenterY - 40;
        DrawText(gameOverText, gameOverTextX, gameOverTextY, gameOverFontSize, WHITE);

        // 2. Too Many Lava Steps! 文本
        const char* lavaText = "Too Many Lava Steps!";
        int lavaFontSize = 18;
        int lavaTextWidth = MeasureText(lavaText, lavaFontSize);
        int lavaTextX = gameOverRectCenterX - lavaTextWidth / 2;
        int lavaTextY = gameOverRectCenterY + 10;
        DrawText(lavaText, lavaTextX, lavaTextY, lavaFontSize, WHITE);

        // 3. Press R to Restart 文本
        const char* restartText = "Press R to Restart";
        int restartFontSize = 18;
        int restartTextWidth = MeasureText(restartText, restartFontSize);
        int restartTextX = gameOverRectCenterX - restartTextWidth / 2;
        int restartTextY = gameOverRectCenterY + 30;
        DrawText(restartText, restartTextX, restartTextY, restartFontSize, WHITE);
        break;
    }
    }

    EndDrawing();
}