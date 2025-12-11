#ifndef PATH_FINDER_H
#define PATH_FINDER_H
#include "MazeParser.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <functional>
#include <utility>

// 坐标结构体（基础数据结构，用于路径表示）
struct Point {
    int row;
    int col;

    // 重载==：用于判断坐标是否相等
    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }

    // 重载<：用于优先队列排序（Dijkstra算法）
    bool operator<(const Point& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

// 为Point实现哈希（用于unordered_map的键）
namespace std {
    template<> struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.row) ^ (hash<int>()(p.col) << 1);
        }
    };

    // 为pair<Point, bool>实现哈希（用于熔岩1次路径算法）
    template<> struct hash<std::pair<Point, bool>> {
        size_t operator()(const std::pair<Point, bool>& p) const {
            auto h1 = hash<Point>{}(p.first);
            auto h2 = hash<bool>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
}

// 路径查找类（基础任务：DFS/BFS/Dijkstra；进阶任务：熔岩1次路径）
class PathFinder {
public:
    // 构造：传入迷宫，初始化起点和终点
    PathFinder(const Maze& maze);

    // 1. 基础任务：DFS找出所有可达路径（返回第一条路径用于绘制）
    std::vector<std::vector<Point>> findAllPathsByDFS();

    // 2. 基础任务：BFS找出最短路径（无权图最短路径）
    std::vector<Point> findShortestPathByBFS();

    // 3. 基础任务：Dijkstra找出加权最短路径（考虑地块成本）
    std::vector<Point> findShortestPathByDijkstra();

    // 4. 进阶任务：允许踩1次熔岩的最短路径（熔岩不计成本）
    std::vector<Point> findShortestPathWithOneLava();

private:
    // 辅助结构体：Dijkstra节点（包含是否已踩过熔岩，进阶任务用）
    struct DijkstraNode {
        Point pos;
        int dist;
        bool hasSteppedLava;

        // 优先队列排序规则（距离小的优先，相同距离则未踩熔岩的优先）
        bool operator>(const DijkstraNode& other) const {
            if (dist != other.dist) return dist > other.dist;
            return hasSteppedLava > other.hasSteppedLava;
        }
    };

    // DFS递归辅助函数（基础任务：遍历所有路径）
    void dfsHelper(int row, int col, std::vector<Point>& curPath,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::vector<Point>>& allPaths);

    // 合法性检查：坐标是否在迷宫范围内且非墙（通用辅助方法）
    bool isLegal(int row, int col) const;

    // 成本计算：根据地块类型返回移动成本（基础任务：Dijkstra算法用）
    int getCost(int row, int col) const;

    const Maze& maze;          // 迷宫数据（只读，避免修改）
    Point startPoint;          // 起点坐标（初始化时查找）
    Point endPoint;            // 终点坐标（初始化时查找）
    const int dirs[4][2] = {   // 四个移动方向（上下左右）
        {-1,0}, {1,0}, {0,-1}, {0,1}
    };
};

#endif // PATH_FINDER_H