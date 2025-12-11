#include "MazeParser.h"
#include <fstream>
#include <sstream>

// 从文件加载迷宫数据
Maze MazeParser::loadFromFile(const std::string& filePath) {
    Maze maze;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open maze file: " + filePath);
    }

    // 第一步：读取行数和列数（第一行）
    file >> maze.rows >> maze.cols;
    if (maze.rows <= 0 || maze.cols <= 0) {
        throw std::runtime_error("Invalid maze size: rows=" + std::to_string(maze.rows) +
            ", cols=" + std::to_string(maze.cols));
    }

    // 第二步：读取后续地块数据
    maze.mapData.resize(maze.rows, std::vector<BlockType>(maze.cols));
    int num;
    for (int row = 0; row < maze.rows; ++row) {
        for (int col = 0; col < maze.cols; ++col) {
            if (!(file >> num)) {
                throw std::runtime_error("Maze data incomplete at row=" + std::to_string(row) +
                    ", col=" + std::to_string(col));
            }
            maze.mapData[row][col] = numToBlockType(num);
        }
    }

    file.close();
    return maze;
}

// 数字转地块类型
BlockType MazeParser::numToBlockType(int num) {
    switch (num) {
    case 1:  return BlockType::WALL;
    case -1: return BlockType::START;
    case 0:  return BlockType::GROUND;
    case -2: return BlockType::END;
    case 2:  return BlockType::GRASS;
    case 3:  return BlockType::LAVA;
    default: throw std::runtime_error("Unknown block type number: " + std::to_string(num));
    }
}