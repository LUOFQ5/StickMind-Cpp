/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

#include "queue.h"


/*
 * Help-Function: 返回单词的分数值，=字符串长度-3
 */
int points(string str) {
    /* 需注意：字符串<=3时要返回0*/
    return (str.size() <= 3) ? 0 : (str.size() - 3);
//    return str.length() - 3;
}

// Help-Function: generateValidMove
// 输入已经访问过的格点（Vetor），基于最近访问的元素，生成相邻的元素集合
Set<GridLocation> generateValidMove(Grid<char>& board, const Vector<GridLocation>& visitedLocs) {
    Set<GridLocation> neighbors;
    //如果为空，直接返回
    if (visitedLocs.isEmpty()) {
        return neighbors;
    }
    //提取最近访问的格点元素
    GridLocation curLoc = visitedLocs[visitedLocs.size() - 1];
    //通过两重for循环来迭代当前格点的8个位置，对于每一个位置进行判断
    //当前位置（i=0, j=0）已经在visitedLocs中，不是有效的移动（在下面 mark中判定）
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            GridLocation next(curLoc.row + i, curLoc.col + j);
            //判定移动后的格点是否在格子范围内inBounds，且字符为字母isalpha，
            //为了壁面已经访问过的格点，需要对visitedLocs进行遍历，如未出现过、则添加到neighbor集合；
            if (board.inBounds(next) && isalpha(board[next]))
            {
                bool mark = false;
                for (const auto& loc: visitedLocs) {
                    if (loc == next) {
                        mark = true;
                    }
                }
                if (!mark) {
                    neighbors.add(next);
                }
            }
        }
    }
    return neighbors;
}

// Help-Function: 从格点Vetor中返回按顺序组成的单词-字符串
string LoctoWord(Grid<char>& board, Vector<GridLocation>& visitedLocs) {
    string result ="";
    for (const auto& loc: visitedLocs) {
        result += board.get(loc);
    }
    return result;
}

/*
 * Func: 拼字游戏的实际函数，判断一个boggle可以完成多少个有效的单词；
 * 返回值：根据有效单词计算最终得分值。
 */
#define  dfs

//
// 参考了作业课的思路讲解，仿照迷宫代码
#ifdef dfs

int scoreBoardRec(Grid<char>& board, Lexicon& lex, Set<string>& resultWords, Vector<GridLocation>& visitedLocs) {
    // 根据目前已访问的格点坐标，返回当前单词
    string curWord = LoctoWord(board, visitedLocs);

    // Base Case1 提前终止：根据已有单词前缀判定，如不存在该前缀则直接返回——说明该条路线没有希望
    if (!lex.containsPrefix(curWord)) {
        return 0;
    }
    // Base Case2 过程结束：如果没有有效相邻点-即empty，说明该条路线也到头了，则进行判定
    Set<GridLocation> neigs = generateValidMove(board, visitedLocs);
    if (neigs.isEmpty()) {
        // 	判定a: 当前单词有效且为首次出现 ，则添加该单词到集合中，直接返回该单词的分数值
        if (lex.contains(curWord) && !resultWords.contains(curWord)) {
            resultWords.add(curWord);
            return points(curWord);
        }
        // 判定b：else否则该单词无效，该路线得分为0
        return 0;
    }
    //过程进行中：对于一条有希望的路线，需要不断前进-降低问题规模-回溯
    else {
        int score =0;  //一条递归-回溯的完整路线中，可能存在多个有效单词，因此定义局部变量
        //递归过程中：如果出现一个有效单词，则添加到集合中，并累积其返回的分数值
        if (curWord.length() > 3 && lex.contains(curWord) && !resultWords.contains(curWord)) {
            resultWords.add(curWord);
            score += points(curWord);
        }

        //递归-回溯：
        for (const GridLocation& loc: neigs) {
           // Choose加入一个元素
           visitedLocs.add(loc);
           // Explore 探索
           score += scoreBoardRec(board, lex, resultWords, visitedLocs);
           // Unchoose-倒退一步 ，返回for循环分析其他loc路线
           visitedLocs.remove(visitedLocs.size() - 1);
        }
        return score;
    }
}
// 函数原型
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    Set<string> resultWords;
    int total = 0;
    //迭代：从每一个格点开始
    for (GridLocation loc: board.locations()) {
        Vector<GridLocation> visitedLocs = {{loc}};
        total += scoreBoardRec(board, lex, resultWords, visitedLocs);
    }
    return total;
}
#endif
/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("generateValidNext") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','B','J','K'},
                        {'-','M','N','P'},
                        {'Q','R','S','T'}};
    Vector<GridLocation> cur = {{0, 1}};
    Set<GridLocation> expected = {{0,0}, {1,0}, {1,1}, {1,2}, {0,2}};
    EXPECT_EQUAL(generateValidMove(board, cur), expected);
    cur = {{2, 2}};
    expected = {{1,1}, {1,2}, {1,3}, {2,1}, {2,3}, {3,1}, {3,2}, {3,3}};
    EXPECT_EQUAL(generateValidMove(board, cur), expected);
    cur = {{3, 1}};
    expected = {{3,0}, {2,1}, {2,2}, {3,2}};
    EXPECT_EQUAL(generateValidMove(board, cur), expected);

    board = {{'L','?','_','_'},
             {'-','E','_','_'},
             {'_','0','_','_'},
             {'_','_','_','_'}};
    cur = {{1, 1}};
    expected = {{0,0}};
    EXPECT_EQUAL(generateValidMove(board, cur), expected);
}
