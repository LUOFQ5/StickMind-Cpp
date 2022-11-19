#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "luofq";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "SSENNESSNESSWW";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needs` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
//输入参数：start-指向迷宫单元入口点的指针；moves-由纯字符 “N”、“S”、“E” 和 “W” 组成的字符串，表示逃离迷宫的路径；
//        needs-收集所有必须物品的字符串集合
//输出参数：给定路径是否能收集到所有物品
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) {
    /* TODO: Your code here */
    MazeCell* pt = start;
    Set<string> result;
    // 1. 天选之子：开局一把屠龙刀，直接飞升
    if (!start->contents.empty()) {
        result.add(start->contents);
        if (result == needs) {
            return true;
        }
    }
    // 2. 打怪升级：迭代所有路径
    for (char ch: moves) {
        // 2a, 判断某个字符所代表的指针方向是否有效，有效则进入该相邻点
        switch (ch) {
        case 'N':
            if (!pt->north) {
                return false;
            }
            pt = pt->north;
            break;

        case 'S':
            if (!pt->south) {
                return false;
            }
            pt = pt->south;
            break;

        case 'E':
            if (!pt->east) {
                return false;
            }
            pt = pt->east;
            break;

        case 'W':
            if (!pt->west) {
                return false;
            }
            pt = pt->west;
            break;
        // 2b: 该字符为无效字母，报错
        default:
            error("The path in the moves-str has invalid item! ");
        }
        //2c. 判断是否捡到宝藏
        if (!pt->contents.empty()) {
            result.add(pt->contents);
        }
        //2d. 是否集齐
        if (result == needs) {
            return true;
        }
    }
    // 3. 迭代结束，路径没问题，但没有集齐宝藏
    return false;
}


/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}

