//#pragma once
//#ifndef MAZE_RENDERER_H
//#define MAZE_RENDERER_H
//
//#include "MazeParser.h"
//#include "TextureManager.h"
//#include "raylib.h"
//#include "PathFinder.h"
//
//// 迷宫绘制类（封装绘制逻辑）
//class MazeRenderer {
//public:
//    static const int BLOCK_SIZE = 32; // 单个地块像素尺寸（32×32）
//    static void drawShortestPath(const std::vector<Point>& path);
//
//    // 静态方法：绘制整个迷宫
//    static void drawMaze(const Maze& maze, const TextureManager& texManager);
//
//private:
//    // 辅助方法：计算地块的绘制坐标
//    static Vector2 getBlockPosition(int row, int col);
//};
//
//#endif // MAZE_RENDERER_H
//
//
////// MazeRenderer.h
////#ifndef MAZE_RENDERER_H
////#define MAZE_RENDERER_H
////
////#include "MazeParser.h"
////#include "TextureManager.h"
////#include "PathFinder.h" // 必须包含，因为用到Point
////#include "raylib.h"
////#include <vector>       // 必须包含，因为用到std::vector
////
////class MazeRenderer {
////public:
////    static const int BLOCK_SIZE = 32;
////    static void drawMaze(const Maze& maze, const TextureManager& texManager);
////    // 声明必须和调用完全一致：参数是const std::vector<Point>&
////    static void drawShortestPath(const std::vector<Point>& path);
////
////private:
////    static Vector2 getBlockPosition(int row, int col);
////};
////
////#endif // MAZE_RENDERER_H

#ifndef MAZE_RENDERER_H
#define MAZE_RENDERER_H

#include "MazeParser.h"
#include "TextureManager.h"
#include "PathFinder.h"
#include "raylib.h"
#include <vector>
#include <iostream>

class MazeRenderer {
public:
    static const int BLOCK_SIZE = 32; // 放大地块尺寸，窗口更清晰
    static void drawMaze(const Maze& maze, const TextureManager& texManager);
    static void drawDijkstraPath(const std::vector<Point>& path); // 原Dijkstra路径（红色）
    static void drawAllDFSPaths(const std::vector<std::vector<Point>>& allPaths); // 所有路径（蓝色）
private:
    static Vector2 getBlockPosition(int row, int col);
};

#endif // MAZE_RENDERER_H