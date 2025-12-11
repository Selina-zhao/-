#include "PathFinder.h"
#include <algorithm>
#include <stdexcept>

// 构造：初始化迷宫并查找起点/终点（修正未初始化问题）
PathFinder::PathFinder(const Maze& maze)
    : maze(maze),
    startPoint({ -1, -1 }),  // 初始化为无效坐标，避免未初始化
    endPoint({ -1, -1 }) {
    // 遍历迷宫查找起点和终点
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

    // 检查起点和终点是否存在（避免后续逻辑崩溃）
    if (startPoint.row == -1 || endPoint.row == -1) {
        throw std::runtime_error("Maze must contain both start (-1) and end (-2) points!");
    }
}

// 合法性检查：坐标是否在迷宫范围内且非墙
bool PathFinder::isLegal(int row, int col) const {
    if (row < 0 || row >= maze.rows || col < 0 || col >= maze.cols) {
        return false; // 越界
    }
    return maze.mapData[row][col] != BlockType::WALL; // 非墙
}

// 成本计算：根据地块类型返回移动成本（基础任务要求）
int PathFinder::getCost(int row, int col) const {
    BlockType type = maze.mapData[row][col];
    switch (type) {
    case BlockType::GROUND:  return 1;    // 普通地面：成本1
    case BlockType::GRASS:   return 3;    // 草地：成本≥3（任务要求）
    case BlockType::LAVA:    return 1000; // 熔岩：成本≥1000（任务要求）
    case BlockType::START:
    case BlockType::END:     return 1;    // 起点/终点：成本1
    default:                 return INT_MAX; // 非法地块（墙）：成本无穷大
    }
}

// 1. 基础任务：DFS找出所有可达终点的路径
std::vector<std::vector<Point>> PathFinder::findAllPathsByDFS() {
    std::vector<std::vector<Point>> allPaths;
    std::vector<Point> curPath;
    // 访问标记：避免重复访问同一地块
    std::vector<std::vector<bool>> visited(maze.rows, std::vector<bool>(maze.cols, false));

    // 起点初始化
    curPath.push_back(startPoint);
    visited[startPoint.row][startPoint.col] = true;

    // 递归遍历所有路径
    dfsHelper(startPoint.row, startPoint.col, curPath, visited, allPaths);

    // 若无路径，抛出异常（便于调试）
    if (allPaths.empty()) {
        throw std::runtime_error("No path found from start to end!");
    }

    return allPaths;
}

// DFS递归辅助函数
void PathFinder::dfsHelper(int row, int col, std::vector<Point>& curPath,
    std::vector<std::vector<bool>>& visited,
    std::vector<std::vector<Point>>& allPaths) {

    // 终止条件：到达终点，保存当前路径
    if (row == endPoint.row && col == endPoint.col) {
        allPaths.push_back(curPath);
        return;
    }

    // 遍历四个方向
    for (const auto& dir : dirs) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        // 若地块合法且未访问，递归遍历
        if (isLegal(newRow, newCol) && !visited[newRow][newCol]) {
            visited[newRow][newCol] = true;
            curPath.push_back({ newRow, newCol });
            dfsHelper(newRow, newCol, curPath, visited, allPaths);
            curPath.pop_back();         // 回溯：移除当前地块
            visited[newRow][newCol] = false; // 回溯：取消访问标记
        }
    }
}

// 2. 基础任务：BFS找出最短路径（无权图，步数最少）
std::vector<Point> PathFinder::findShortestPathByBFS() {
    std::unordered_map<Point, Point> prev; // 前驱节点映射：用于回溯路径
    std::queue<Point> q;                  // BFS队列
    std::vector<std::vector<bool>> visited(maze.rows, std::vector<bool>(maze.cols, false));

    // 起点初始化
    q.push(startPoint);
    visited[startPoint.row][startPoint.col] = true;

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();

        // 终止条件：到达终点，回溯路径
        if (cur == endPoint) {
            std::vector<Point> path;
            Point temp = cur;
            while (true) {
                path.push_back(temp);
                if (temp == startPoint) break;

                // 若无前驱节点，说明无路径
                auto it = prev.find(temp);
                if (it == prev.end()) {
                    throw std::runtime_error("No BFS path found from start to end!");
                }
                temp = it->second;
            }
            std::reverse(path.begin(), path.end()); // 反转路径：起点→终点
            return path;
        }

        // 遍历四个方向
        for (const auto& dir : dirs) {
            int newRow = cur.row + dir[0];
            int newCol = cur.col + dir[1];
            Point newPos = { newRow, newCol };

            // 若地块合法且未访问，加入队列
            if (isLegal(newRow, newCol) && !visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                prev[newPos] = cur; // 记录前驱节点
                q.push(newPos);
            }
        }
    }

    // 若队列空仍未找到终点，抛出异常
    throw std::runtime_error("No BFS path found from start to end!");
}

// 3. 基础任务：Dijkstra找出加权最短路径（考虑地块成本）
std::vector<Point> PathFinder::findShortestPathByDijkstra() {
    // 距离映射：Point → 从起点到该点的最短距离
    std::unordered_map<Point, int> dist;
    // 前驱映射：Point → 前驱节点（用于回溯路径）
    std::unordered_map<Point, Point> prev;
    // 优先队列（小根堆）：按距离升序排列，存储（距离，坐标）
    using PQElement = std::pair<int, Point>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> pq;

    // 起点初始化：距离0，加入队列
    dist[startPoint] = 0;
    pq.push({ 0, startPoint });

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int curDist = top.first;
        Point curPos = top.second;

        // 终止条件：到达终点，提前退出（优先队列保证此时为最短距离）
        if (curPos == endPoint) break;

        // 若当前距离大于已知最短距离，跳过（冗余节点）
        if (curDist > dist[curPos]) continue;

        // 遍历四个方向
        for (const auto& dir : dirs) {
            int newRow = curPos.row + dir[0];
            int newCol = curPos.col + dir[1];
            Point newPos = { newRow, newCol };

            // 跳过非法地块
            if (!isLegal(newRow, newCol)) continue;

            // 计算新距离：当前距离 + 新地块成本
            int newDist = curDist + getCost(newRow, newCol);

            // 若新距离更短，更新距离和前驱，并加入队列
            if (dist.find(newPos) == dist.end() || newDist < dist[newPos]) {
                dist[newPos] = newDist;
                prev[newPos] = curPos;
                pq.push({ newDist, newPos });
            }
        }
    }

    // 回溯路径：从终点到起点
    std::vector<Point> path;
    Point cur = endPoint;
    while (true) {
        path.push_back(cur);
        if (cur == startPoint) break;

        // 若无前驱节点，说明无路径
        auto it = prev.find(cur);
        if (it == prev.end()) {
            throw std::runtime_error("No Dijkstra path found from start to end!");
        }
        cur = it->second;
    }

    std::reverse(path.begin(), path.end()); // 反转路径：起点→终点
    return path;
}

// 4. 进阶任务：允许踩1次熔岩的最短路径（熔岩不计成本）
std::vector<Point> PathFinder::findShortestPathWithOneLava() {
    // 距离映射：(坐标, 是否已踩过熔岩) → 最短距离
    std::unordered_map<std::pair<Point, bool>, int> dist;
    // 前驱映射：(坐标, 是否已踩过熔岩) → (前驱坐标, 前驱是否已踩过熔岩)
    std::unordered_map<std::pair<Point, bool>, std::pair<Point, bool>> prev;
    // 优先队列（小根堆）：存储DijkstraNode（包含是否踩过熔岩）
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<>> pq;

    // 起点初始化：未踩过熔岩，距离0
    auto startKey = std::make_pair(startPoint, false);
    dist[startKey] = 0;
    pq.push({ startPoint, 0, false });

    while (!pq.empty()) {
        auto curNode = pq.top();
        pq.pop();
        Point curPos = curNode.pos;
        int curDist = curNode.dist;
        bool curHasLava = curNode.hasSteppedLava;

        // 终止条件：到达终点，回溯路径
        if (curPos == endPoint) {
            std::vector<Point> path;
            auto curKey = std::make_pair(curPos, curHasLava);

            while (true) {
                path.push_back(curKey.first);
                // 终止：回溯到起点（未踩过熔岩）
                if (curKey.first == startPoint && !curKey.second) break;

                // 若无前驱节点，说明无路径
                auto it = prev.find(curKey);
                if (it == prev.end()) {
                    throw std::runtime_error("No lava-once path found from start to end!");
                }
                curKey = it->second;
            }

            std::reverse(path.begin(), path.end()); // 反转路径：起点→终点
            return path;
        }

        // 若当前距离大于已知最短距离，跳过（冗余节点）
        auto curKey = std::make_pair(curPos, curHasLava);
        if (curDist > dist[curKey]) continue;

        // 遍历四个方向
        for (const auto& dir : dirs) {
            int newRow = curPos.row + dir[0];
            int newCol = curPos.col + dir[1];
            Point newPos = { newRow, newCol };

            // 跳过非法地块（墙）
            if (!isLegal(newRow, newCol)) continue;

            BlockType newBlock = maze.mapData[newRow][newCol];
            bool newHasLava = curHasLava;

            // 处理熔岩地块：未踩过则标记，已踩过则跳过
            if (newBlock == BlockType::LAVA) {
                if (curHasLava) continue; // 已踩过1次，不能再踩
                newHasLava = true;        // 首次踩熔岩，标记状态
            }

            // 计算新距离：熔岩不计成本（任务要求），其他地块按原成本
            int cost = (newBlock == BlockType::LAVA) ? 0 : getCost(newRow, newCol);
            int newDist = curDist + cost;

            // 新状态键：(新坐标, 新熔岩状态)
            auto newKey = std::make_pair(newPos, newHasLava);

            // 若新距离更短，更新距离和前驱，并加入队列
            if (dist.find(newKey) == dist.end() || newDist < dist[newKey]) {
                dist[newKey] = newDist;
                prev[newKey] = curKey;
                pq.push({ newPos, newDist, newHasLava });
            }
        }
    }

    // 若队列空仍未找到终点，抛出异常
    throw std::runtime_error("No lava-once path found from start to end!");
}