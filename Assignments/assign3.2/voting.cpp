// TODO: remove and replace this file header comment

#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

#include "map.h"

// help func: cal sum of elements for a Vector<i>
int sumVector(Vector<int>& input) {
    int sum = 0;
    for (int i: input) {
        sum += i;
    }
    return sum;
}


// Func: 街区投票系统的计数函数，对每一个目标块-item，计算关键选票次数
// 输入参数：winTarget-超过该数值投票就能赢，item-当前目标块，restBlocks-其他所有区块组成的向量，index-递归过程中的计数器， sofarSum-当前联盟的综合
// 输出参数：对所有可能联盟进行递归后，当前目标块的关键选票次数
#define version1

#ifdef version1
int computePowerIndexesRec(const int& winTarget, const int& item, Vector<int>& restBlocks, int index, int sofarSum) {
    int powerIndex = 0;
    // base case1: 提前终止，没有目标区块时该联盟能获胜，因此不是关键选票
    if (sofarSum > winTarget) {
        return 0;
    }
    // base case2：判定条件，如果没有目标区块该联盟会输、加上目标区块后该联盟能赢，因此是关键选票
    if (index == restBlocks.size()) {
        if (sofarSum <= winTarget && sofarSum + item > winTarget) {
            powerIndex++;
        }
    }
    // 递归实现：
    else {
        // choose
        int element = restBlocks[index];
        index++;
        // include
        powerIndex += computePowerIndexesRec(winTarget, item, restBlocks, index, sofarSum + element);
        // exclude
        powerIndex += computePowerIndexesRec(winTarget, item, restBlocks, index, sofarSum);

    }
    return powerIndex;
}

// Func: 街区投票系统的计数函数 ，输入N向量（区块计数），
// 输出 N向量（每个街区的banzhaf影响力指数）
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // 优化点：通过Map存储已有的目标块-关键选票数
    Map<int,int> resultMap {};
    int winTarget = sumVector(blocks) / 2;
    //迭代所有的街区，依次选择作为目标块
    for (int i = 0; i < blocks.size(); i++) {
        // 求出除了目标区块之外的所有区块
        Vector rest = blocks.subList(0, i) + blocks.subList(i+1);

        int sofarSum = 0;
        //如果该区块投票数已经被计算过了，则忽略
        if (!resultMap.containsKey(blocks[i])) {
            int nPower = computePowerIndexesRec(winTarget, blocks[i], rest, 0, sofarSum);
            resultMap[blocks[i]] = nPower;
        }
        result.add(resultMap[blocks[i]]);
    }
    //由关键选票次数计算 PowerIndex
    int total = sumVector(result);
    for (int& num: result) {
        num = num * 100 / total;
    }

    return result;
}
#endif

// buggy version
#ifdef version2
int computePowerIndexesRec(const int& winTarget, const int& item, Vector<int>& restBlocks, int index, int sofarSum) {
    int powerIndex = 0;
    // base case1: 提前终止，没有目标区块时该联盟能获胜，因此不是关键选票
    if (sofarSum > winTarget) {
        return 0;
    }
    // base case2：判定条件，如果没有目标区块该联盟会输、加上目标区块后该联盟能赢，因此是关键选票
    if (index == restBlocks.size()) {
        if (sofarSum <= winTarget && sofarSum + item > winTarget) {
            powerIndex++;
        }
    }
    // 递归实现：
    for (int i = index; i < restBlocks.size(); i++) {
        sofarSum += restBlocks[i];
        powerIndex += computePowerIndexesRec(winTarget, item, restBlocks, i + 1, sofarSum);
        sofarSum -= restBlocks[i];
    }
    return powerIndex;
}

// Func: 街区投票系统的计数函数 ，输入N向量（区块计数），
// 输出 N向量（每个街区的banzhaf影响力指数）
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // 优化点：通过Map存储已有的目标块-关键选票数
    Map<int,int> resultMap {};
    int winTarget = sumVector(blocks) / 2;
    //迭代所有的街区，依次选择作为目标块
    for (int i = 0; i < blocks.size(); i++) {
        // 求出除了目标区块之外的所有区块
        Vector rest = blocks.subList(0, i) + blocks.subList(i+1);

        int sofarSum = 0;
        //如果该区块投票数已经被计算过了，则忽略
        if (!resultMap.containsKey(blocks[i])) {
            int nPower = computePowerIndexesRec(winTarget, blocks[i], rest, 0, sofarSum);
            resultMap[blocks[i]] = nPower;
        }
        result.add(resultMap[blocks[i]]);
    }
    //由关键选票次数计算 PowerIndex
    int total = sumVector(result);
    for (int& num: result) {
        num = num * 100 / total;
    }

    return result;
}
#endif
/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time test for power index operation:  blocks EU post-Nice.....27 element?") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation, random blocks ... from 10 to 30 blocks") {
    Vector<int> blocks;
    for (int i = 0; i < 10; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));

    blocks.clear();
    for (int i = 0; i < 14; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));

    blocks.clear();
    for (int i = 0; i < 18; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));

    blocks.clear();
    for (int i = 0; i < 22; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));

    blocks.clear();
    for (int i = 0; i < 26; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));

    blocks.clear();
    for (int i = 0; i < 27; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 0; i < 28; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 0; i < 29; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 0; i < 30; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}



