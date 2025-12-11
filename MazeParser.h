#ifndef MAZE_PARSER_H
#define MAZE_PARSER_H
#include <string>
#include <vector>
#include <stdexcept>

// 地块类型枚举（与maze0.txt中的数字对应）
enum class BlockType {
    WALL = 1,        // 墙
    START = -1,      // 起点
    GROUND = 0,      // 普通地面
    END = -2,        // 终点
    GRASS = 2,       // 草地
    LAVA = 3         // 熔岩
};

// 迷宫数据类（封装行数、列数、地块数据）
class Maze {
public:
    int rows;                  // 迷宫行数
    int cols;                  // 迷宫列数
    std::vector<std::vector<BlockType>> mapData; // 二维地块数据

    // 构造/析构
    Maze() : rows(0), cols(0) {}
    ~Maze() = default;
};

// 迷宫文件解析类
class MazeParser {
public:
    // 静态方法：从文件加载迷宫数据（C++11及以上）
    static Maze loadFromFile(const std::string& filePath);

private:
    // 辅助方法：将文件中的数字转换为BlockType
    static BlockType numToBlockType(int num);
};

#endif // MAZE_PARSER_H