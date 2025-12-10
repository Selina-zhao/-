#include "PathFinder.h"
#include <iostream>
#include <algorithm>

PathFinder::PathFinder(const Maze& maze) : maze(maze) {
    // 找起点/终点
    for (int row = 0; row < maze.rows; ++row) {
        for (int col = 0; col < maze.cols; ++col) {
            if (maze.mapData[row][col] == BlockType::START) {
                startPoint = { row, col };
            }
            else if (maze.mapData[row][col] == BlockType::END) {
                endPoint = { row, col };
            }
        }
    }
}

// ========== DFS找所有可达终点的路径 ==========
std::vector<std::vector<Point>> PathFinder::findAllPathsByDFS() {
    std::vector<std::vector<Point>> allPaths;
    std::vector<Point> curPath;
    std::vector<std::vector<bool>> visited(maze.rows, std::vector<bool>(maze.cols, false));

    // 起点初始化
    curPath.push_back(startPoint);
    visited[startPoint.row][startPoint.col] = true;
    dfsHelper(startPoint.row, startPoint.col, curPath, visited, allPaths);

    return allPaths;
}

// DFS递归核心
void PathFinder::dfsHelper(int row, int col, std::vector<Point>& curPath,
    std::vector<std::vector<bool>>& visited,
    std::vector<std::vector<Point>>& allPaths) {
    // 到达终点：保存路径
    if (row == endPoint.row && col == endPoint.col) {
        allPaths.push_back(curPath);
        return;
    }

    // 遍历四个方向
    for (auto& dir : dirs) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        // 合法（非墙+未越界）+ 未访问
        if (isLegal(newRow, newCol) && !visited[newRow][newCol]) {
            visited[newRow][newCol] = true;
            curPath.push_back({ newRow, newCol });
            dfsHelper(newRow, newCol, curPath, visited, allPaths);
            curPath.pop_back(); // 回溯
            visited[newRow][newCol] = false; // 回溯
        }
    }
}

// ========== Dijkstra算法（加权最短路径） ==========
bool PathFinder::isLegal(int row, int col) {
    if (row < 0 || row >= maze.rows || col < 0 || col >= maze.cols) return false;
    return maze.mapData[row][col] != BlockType::WALL;
}

int PathFinder::getCost(int row, int col) {
    BlockType type = maze.mapData[row][col];
    switch (type) {
    case BlockType::GROUND: return 1;
    case BlockType::GRASS: return 3;
    case BlockType::LAVA: return 1000; // 岩浆高成本
    case BlockType::START:
    case BlockType::END: return 1;
    default: return INT_MAX;
    }
}

std::vector<Point> PathFinder::findShortestPath() {
    std::unordered_map<Point, int> dist;
    std::unordered_map<Point, Point> prev;
    std::priority_queue<std::pair<int, Point>,
        std::vector<std::pair<int, Point>>,
        std::greater<std::pair<int, Point>>> pq;

    dist[startPoint] = 0;
    pq.push({ 0, startPoint });

    while (!pq.empty()) {
        std::pair<int, Point> topPair = pq.top();
        pq.pop();
        int curDist = topPair.first;
        Point curPoint = topPair.second;

        if (curPoint == endPoint) break;

        for (auto& dir : dirs) {
            int newRow = curPoint.row + dir[0];
            int newCol = curPoint.col + dir[1];
            Point newPoint = { newRow, newCol };

            if (!isLegal(newRow, newCol)) continue;

            int cost = getCost(newRow, newCol);
            int newDist = curDist + cost;

            if (dist.find(newPoint) == dist.end() || newDist < dist[newPoint]) {
                dist[newPoint] = newDist;
                prev[newPoint] = curPoint;
                pq.push({ newDist, newPoint });
            }
        }
    }

    // 回溯路径
    std::vector<Point> path;
    Point cur = endPoint;
    while (true) {
        path.push_back(cur);
        if (cur == startPoint) break;
        if (prev.find(cur) == prev.end()) {
            path.clear();
            return path;
        }
        cur = prev[cur];
    }
    std::reverse(path.begin(), path.end());
    return path;
}