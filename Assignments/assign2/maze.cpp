// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO:
 * 生成符合迷宫要求的格点，输入为 maze-迷宫二维grid, cur-当前所处的格点GridLocation
 * 输出为有效GridLocation的集合；
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;   //存储有效格点的数据类型
    /* TODO: Fill in the remainder of this function. */
    //当前位置的格点-行列值
    int row = cur.row;
    int col = cur.col;
    //遍历当前位置向东南西北移动一步，判定是否符合要求
    //二重for-loop来生成东南西北四个方向，  abs(i+j)==1保证只移动一步
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (abs(i + j) == 1)
            {
                GridLocation nextMove(row + i, col + j);

                //判定移动后的格点是否在迷宫范围内，如符合要求，则添加到neighbor集合；
                //两个if条件顺序不能反：先inBounds确定数据在迷宫范围内, 再断定是否为通道（bool == true）
                if (maze.inBounds(nextMove) && maze[nextMove])
                {
                    neighbors.add(nextMove);
                }
            }
        }
    }
    return neighbors;
}

/* TODO:
 * 判断所生成的路径是否可以顺利走出迷宫：输入迷宫地图maze、Stack数据结构的路径path
 * path的要求：从入口到出口，每一步移动都符合规则，路径不能出现循环
*/

void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    //定义迷宫的出口mazeExit
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //断定path的最后一个点是否为出口，如否则肯定不通过
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */
    //初始化已经访问点的集合
    Set<GridLocation> visitedLocs;

    //从路径的末端-出口开始判断，直至到达入口
    while (!path.isEmpty()) {
        //取出当前格点
        GridLocation curGrid = path.pop();
        //如果当前为路径的开始，则判断是否为入口(0, 0)
        if (path.isEmpty() && (curGrid.col != 0 || curGrid.row != 0)) {
            error("Path does not start at maze entry");
        }

        //如果当前点已经走过了，则报错
        if (visitedLocs.contains(curGrid)) {
            error("Path contains repeated GridPosition~");
        }

        //生成当前点的有效邻点MoveGrids；
        //如果该路径的上一个点不在有效点集内，则路径有问题，报错
        Set<GridLocation> MoveGrids = generateValidMoves(maze, curGrid);
        if (!path.isEmpty() && !MoveGrids.contains(path.peek())) {
            error("Path contains invalid GridLocation~~");
        }
        //将当前点加入到 已经访问过的点集；
        visitedLocs.add(curGrid);
    }
    return;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
//    1. 创建一个空的队列，用于添加路径。每个路径为Stack<GridLocation>
    Queue<Stack<GridLocation>> pathsInProcess;
//    2. 创建一个仅包含入口的路径，并添加到队列中。
    GridLocation Entry = {0, 0};
    Stack<GridLocation> onlyEntryPath = {Entry};
    pathsInProcess.enqueue(onlyEntryPath);

    GridLocation Exit = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation> visitedLocs;

//    3. 如果队列仍然有多余的路径需要处理，则
    while (!pathsInProcess.isEmpty()) {
//      1. 从队列中取出当前路径
        path = pathsInProcess.dequeue();
//      2. 如果当前路径已经到达出口
//        1. 搜索结束。当前路径就是最短路径。
        GridLocation curLoc = path.peek();
        if (curLoc == Exit) {
            return path;
        }
//      3. 否则
//        1. 根据当前路径的末端，找出所有可以继续前进的位置。需要注意的是，不要回头。所以这些位置，不能包括之前走过的点位。
        Set<GridLocation> neighborLocs = generateValidMoves(maze, curLoc);
        for (auto element: neighborLocs) {
            if (!visitedLocs.contains(element)) {
                Stack<GridLocation> copyPath = path;
                copyPath.push(element);
                pathsInProcess.enqueue(copyPath);
            }
        }
        visitedLocs.add(curLoc);
        MazeGraphics::highlightPath(path, "red", 5);
    }
//    MazeGraphics::highlightPath(path, "red", 5);
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here

STUDENT_TEST("~~~generateValidMoves on location on the side of 3x3 grid with all walls") {
    Grid<bool> maze = {{false, false, false},
                       {true, false, false},
                       {false, false, false}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("~~~generateValidMoves on location on the side of 3x3 grid with only one way") {
    Grid<bool> maze = {{false, false, false},
                       {true, true, false},
                       {false, false, false}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("~~~generateValidMoves on location on the side of 3x5 grid") {
    Grid<bool> maze = {{false, false, false, true, true},
                       {true, true, false, false, true},
                       {false, false, false, true, true}};
    GridLocation side = {1, 4};
    Set<GridLocation> expected = {{0, 4}, {2, 4}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("~~~validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, true},
                       {true, false, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0}, {0, 1} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {1, 0}, {1, 1}, {2, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 0},{0, 2} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

STUDENT_TEST("~~~validatePath on correct solution loaded from 17x37 maze file") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/17x37.maze", maze);
    readSolutionFile("res/17x37.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("~~~validatePath on correct solution loaded from 19x35 maze file") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/19x35.maze", maze);
    readSolutionFile("res/19x35.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
