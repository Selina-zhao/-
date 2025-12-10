//#pragma once
//#ifndef PATH_FINDER_H
//#define PATH_FINDER_H
//
//#include "MazeParser.h"
//#include <vector>
//#include <queue>
//#include <unordered_map>
//#include <climits>
//
//// 定义坐标结构体
////struct Point {
////    int row;
////    int col;
////
////    // 重载==和哈希，用于unordered_map
////    bool operator==(const Point& other) const {
////        return row == other.row && col == other.col;
////    }
////
////    // 用于优先队列的比较（Dijkstra需要）
////    bool operator>(const Point& other) const {
////        return false; // 仅占位，实际比较距离
////    }
////};
//struct Point {
//    int row;
//    int col;
//
//    // 新增：重载==运算符
//    bool operator==(const Point& other) const {
//        return row == other.row && col == other.col;
//    }
//
//    bool operator<(const Point& other) const {
//        if (row != other.row) return row < other.row;
//        return col < other.col;
//    }
//
//};
//
//
//
//// 为Point实现哈希（用于unordered_map）
//namespace std {
//    template<> struct hash<Point> {
//        size_t operator()(const Point& p) const {
//            return hash<int>()(p.row) ^ (hash<int>()(p.col) << 1);
//        }
//    };
//}
//
//// 路径查找类
//class PathFinder {
//public:
//    // 构造：传入迷宫数据
//    PathFinder(const Maze& maze);
//
//    // 核心方法：计算起点到终点的最短路径（Dijkstra算法）
//    std::vector<Point> findShortestPath();
//
//private:
//    // 获取某个地块的移动成本（按规则：普通0=1，草地2≥3，熔岩3≥1000，墙1不可走）
//    int getCost(int row, int col);
//
//    // 检查坐标是否合法（在迷宫范围内+不是墙）
//    bool isLegal(int row, int col);
//
//    // 迷宫数据
//    const Maze& maze;
//    // 起点/终点坐标
//    Point startPoint;
//    Point endPoint;
//};
//
//#endif // PATH_FINDER_H


#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "MazeParser.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <functional>

struct Point {
    int row;
    int col;
    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }
    bool operator<(const Point& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

namespace std {
    template<> struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.row) ^ (hash<int>()(p.col) << 1);
        }
    };
}

class PathFinder {
public:
    PathFinder(const Maze& maze);
    // 1. DFS：找出所有可达终点的路径（新增）
    std::vector<std::vector<Point>> findAllPathsByDFS();
    // 2. 原Dijkstra：加权最短路径（保留）
    std::vector<Point> findShortestPath();

private:
    // DFS递归辅助函数
    void dfsHelper(int row, int col, std::vector<Point>& curPath,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::vector<Point>>& allPaths);
    bool isLegal(int row, int col); // 合法性检查
    int getCost(int row, int col);  // 成本计算（原Dijkstra用）

    const Maze& maze;
    Point startPoint;
    Point endPoint;
    const int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} }; // 上下左右
};

#endif // PATH_FINDER_H
