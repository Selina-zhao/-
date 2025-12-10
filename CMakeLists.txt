cmake_minimum_required(VERSION 3.16)
project(MazeGame CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Raylib库（需提前安装Raylib，或手动指定路径）
find_package(raylib 4.5 REQUIRED)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集所有源文件
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接Raylib库
target_link_libraries(${PROJECT_NAME} raylib)

# 拷贝资源文件夹到编译目录（方便运行）
file(COPY ${PROJECT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
# 拷贝maze0.txt到编译目录（如果文件不在根目录）
file(COPY ${PROJECT_SOURCE_DIR}/maze0.txt DESTINATION ${CMAKE_BINARY_DIR})