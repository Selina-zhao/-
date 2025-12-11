#include "Player.h"
#include "MazeRenderer.h"

// 草地减速逻辑
void Player::updateSpeed(BlockType blockType) {
    switch (blockType) {
    case BlockType::GRASS:
        currentSpeed = baseSpeed / 3.0f;
        break;
    default:
        currentSpeed = baseSpeed;
        break;
    }
}

// 更新移动方向（匹配精灵图行）
void Player::updateDirection(Vector2 moveDir) {
    if (moveDir.y == 1) {
        currDir = Direction::DOWN;  // 下键/S：第0行
    }
    else if (moveDir.x == -1) {
        currDir = Direction::LEFT;  // 左键/A：第1行
    }
    else if (moveDir.x == 1) {
        currDir = Direction::RIGHT; // 右键/D：第2行
    }
    else if (moveDir.y == -1) {
        currDir = Direction::UP;    // 上键/W：第3行
    }
    frameRect.y = (float)currDir * frameHeight;
}

// 动画帧切换逻辑
void Player::updateAnimation(float deltaTime) {
    Vector2 moveDir = { 0, 0 };
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) moveDir.y = -1;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) moveDir.y = 1;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) moveDir.x = -1;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) moveDir.x = 1;

    if (moveDir.x != 0 || moveDir.y != 0) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0.0f;
            currFrame = (currFrame + 1) % frameCols;
            frameRect.x = (float)currFrame * frameWidth;
        }
    }
    else {
        currFrame = 0;
        frameRect.x = 0.0f;
        timer = 0.0f;
    }
}

// 核心：脚部精确碰撞检测（避免穿墙）
bool Player::checkFootCollision(const Maze& maze, Vector2 newPixelPos) const {
    // 1. 计算脚部碰撞盒的四个顶点坐标（基于新像素位置）
    // 脚部碰撞盒：居中在精灵图底部，偏移footOffsetY像素（避免底部空白导致的判定错误）
    float footX = newPixelPos.x + (frameWidth - footCollisionWidth) / 2; // 碰撞盒X（水平居中）
    float footY = newPixelPos.y + frameHeight - footCollisionHeight - footOffsetY; // 碰撞盒Y（靠近底部）

    // 2. 定义脚部碰撞盒的四个角（左上、右上、左下、右下）
    Vector2 footCorners[4] = {
        {footX, footY},  // 左上
        {footX + footCollisionWidth, footY},  // 右上
        {footX, footY + footCollisionHeight},  // 左下
        {footX + footCollisionWidth, footY + footCollisionHeight}   // 右下
    };

    // 3. 检查四个角是否都在合法地块上（非墙+在迷宫范围内）
    for (auto& corner : footCorners) {
        // 转换角坐标到逻辑地块坐标
        int tileRow = static_cast<int>(corner.y) / MazeRenderer::BLOCK_SIZE;
        int tileCol = static_cast<int>(corner.x) / MazeRenderer::BLOCK_SIZE;

        // 检查是否越界
        if (tileRow < 0 || tileRow >= maze.rows || tileCol < 0 || tileCol >= maze.cols) {
            return false; // 越界=碰撞
        }

        // 检查是否是墙
        if (maze.mapData[tileRow][tileCol] == BlockType::WALL) {
            return false; // 墙=碰撞
        }
    }

    return true; // 四个角都合法=无碰撞
}

// 核心更新方法：整合移动、精确碰撞、动画
void Player::update(const Maze& maze, float deltaTime) {
    // 1. 读取移动输入
    Vector2 moveDir = { 0, 0 };
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) moveDir.y = -1;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) moveDir.y = 1;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) moveDir.x = -1;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) moveDir.x = 1;

    // 2. 归一化移动方向
    if (moveDir.x != 0 && moveDir.y != 0) {
        moveDir.x *= 0.7071f;
        moveDir.y *= 0.7071f;
    }

    // 3. 更新方向和动画
    if (moveDir.x != 0 || moveDir.y != 0) {
        updateDirection(moveDir);
    }
    updateAnimation(deltaTime);

    // 4. 计算新的像素坐标
    Vector2 newPixelPos = {
        pixelPos.x + moveDir.x * currentSpeed * deltaTime * 60,
        pixelPos.y + moveDir.y * currentSpeed * deltaTime * 60
    };

    // 5. 精确碰撞检测：检查脚部是否会穿墙
    if (checkFootCollision(maze, newPixelPos)) {
        // 6. 碰撞检测通过：更新位置和逻辑坐标
        pixelPos = newPixelPos;
        // 计算新的逻辑坐标（基于脚部中心）
        float footCenterX = newPixelPos.x + frameWidth / 2;
        /*float footCenterY = newPixelPos.y + frameHeight - footCollisionHeight / 2 - footOffsetY;*/
        float footCenterY = newPixelPos.y + frameHeight ;
        int newRow = static_cast<int>(footCenterY) / MazeRenderer::BLOCK_SIZE;
        int newCol = static_cast<int>(footCenterX) / MazeRenderer::BLOCK_SIZE;

        // 7. 处理熔岩计数
        if (newRow >= 0 && newRow < maze.rows && newCol >= 0 && newCol < maze.cols) {
            BlockType newBlock = maze.mapData[newRow][newCol];
            // 草地减速
            updateSpeed(newBlock);
            // 熔岩计数（仅从非熔岩地块进入时）
            if (newBlock == BlockType::LAVA && maze.mapData[pos.row][pos.col] != BlockType::LAVA) {
                lavaStepCount++;
            }
            // 更新逻辑坐标
            pos = { newRow, newCol };
        }
    }
}

// 绘制当前帧（可选：绘制脚部碰撞盒辅助调试）
void Player::draw() const {
    // 绘制精灵图当前帧
    DrawTextureRec(charTexture, frameRect, pixelPos, WHITE);

    // （调试用）绘制脚部碰撞盒（绿色半透明，可注释）
    float footX = pixelPos.x + (frameWidth - footCollisionWidth) / 2;
    float footY = pixelPos.y + frameHeight - footCollisionHeight - footOffsetY;
    DrawRectangle(
        static_cast<int>(footX), static_cast<int>(footY),
        static_cast<int>(footCollisionWidth), static_cast<int>(footCollisionHeight),
        Color{ 0, 255, 0, 100 } // 绿色半透明
    );
}
