#ifndef PLAYER_H
#define PLAYER_H
#include "MazeParser.h"
#include "raylib.h"
#include "MazeRenderer.h"

class Player {
public:
    // 构造：传入起点、精灵图路径、关键帧行列数（默认3列4行）
    Player(const Point& startPos, const std::string& charTexPath, int frameCols = 3, int frameRows = 4)
        : pos(startPos), lavaStepCount(0), baseSpeed(5.0f), currentSpeed(baseSpeed),
        frameCols(frameCols), frameRows(frameRows), currFrame(0), currDir(Direction::DOWN),
        timer(0.0f), frameTime(0.3f), footOffsetY(2.0f) { // footOffsetY：脚部偏移（可调整）
        // 1. 加载精灵图（含所有方向+帧）
        Image spriteSheet = LoadImage(charTexPath.c_str());
        if (spriteSheet.data == nullptr) {
            throw std::runtime_error("Failed to load sprite sheet: " + charTexPath);
        }
        // 2. 转换为显存纹理，释放内存图片
        charTexture = LoadTextureFromImage(spriteSheet);
        UnloadImage(spriteSheet);
        if (charTexture.id == 0) {
            throw std::runtime_error("Failed to create texture from sprite sheet");
        }
        // 3. 计算单个关键帧尺寸（精灵图均分）
        frameWidth = charTexture.width / frameCols;
        frameHeight = charTexture.height / frameRows;
        // 4. 初始化关键帧区域（默认向下走第0帧）
        frameRect = { 0.0f, (float)currDir * frameHeight, (float)frameWidth, (float)frameHeight };
        // 5. 初始化像素坐标（居中显示在32×32地块上）
        pixelPos = {
            MazeRenderer::getBlockPosition(pos.row, pos.col).x + (MazeRenderer::BLOCK_SIZE - frameWidth) / 2,
            MazeRenderer::getBlockPosition(pos.row, pos.col).y + (MazeRenderer::BLOCK_SIZE - frameHeight) / 2
        };
        // 6. 初始化脚部碰撞检测区域（基于精灵图尺寸）
        footCollisionWidth = frameWidth * 0.5f; // 脚部碰撞宽度（精灵图宽度的50%，避免过宽）
        footCollisionHeight = 4.0f;            // 脚部碰撞高度（4像素，模拟脚底）

    }

    // 析构：释放纹理资源
    ~Player() {
        UnloadTexture(charTexture);
    }

    // 更新：移动逻辑+动画帧切换+精确碰撞检测
    void update(const Maze& maze, float deltaTime);
    // 绘制：绘制当前方向的当前帧
    void draw() const;
    // 重置：恢复初始状态
    void reset(const Point& startPos) {
        pos = startPos;
        lavaStepCount = 0;
        currentSpeed = baseSpeed;
        currFrame = 0;
        currDir = Direction::DOWN;
        timer = 0.0f;
        // 重置关键帧区域和位置
        frameRect.x = 0.0f;
        frameRect.y = (float)currDir * frameHeight;
        pixelPos = {
            MazeRenderer::getBlockPosition(pos.row, pos.col).x + (MazeRenderer::BLOCK_SIZE - frameWidth) / 2,
            MazeRenderer::getBlockPosition(pos.row, pos.col).y + (MazeRenderer::BLOCK_SIZE - frameHeight) / 2
        };
    }

    Point getPosition() const { return pos; }
    int getLavaStepCount() const { return lavaStepCount; }



private:
    // 移动方向枚举（匹配精灵图：0=下、1=左、2=右、3=上）
    enum class Direction {
        DOWN = 0,  // 第0行：向下走
        LEFT = 1,  // 第1行：向左走
        RIGHT = 2, // 第2行：向右走
        UP = 3     // 第3行：向上走
    };

    // 核心属性
    Point pos;                  // 逻辑坐标（地块行列）
    int lavaStepCount;          // 熔岩踩中次数
    const float baseSpeed;      // 基础移动速度
    float currentSpeed;         // 当前速度（受草地影响）
    Vector2 pixelPos;           // 像素坐标（平滑移动用）

    // 动画相关属性
    Texture2D charTexture;      // 精灵图纹理
    Rectangle frameRect;        // 当前关键帧截取区域
    int frameCols;              // 精灵图列数
    int frameRows;              // 精灵图行数
    int frameWidth;             // 单个帧宽度
    int frameHeight;            // 单个帧高度
    int currFrame;              // 当前帧序号
    Direction currDir;          // 当前移动方向
    float timer;                // 帧切换计时器
    const float frameTime;      // 每帧播放时间

    // 碰撞检测相关属性（解决穿墙问题）
    const float footOffsetY;    // 脚部相对于精灵图底部的偏移（单位：像素）
    float footCollisionWidth;   // 脚部碰撞盒宽度
    float footCollisionHeight;  // 脚部碰撞盒高度

    // 辅助方法
    void updateDirection(Vector2 moveDir);       // 更新移动方向
    void updateAnimation(float deltaTime);       // 更新动画帧
    void updateSpeed(BlockType blockType);       // 草地减速
    bool checkFootCollision(const Maze& maze, Vector2 newPixelPos) const; // 脚部碰撞检测
};

#endif // PLAYER_H