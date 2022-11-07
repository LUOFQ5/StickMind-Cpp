/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"
#include "strlib.h"

using namespace std;

/*
 * Func: extract the operator characters from the input string
 * Input: a string
 * Output: a new string consists of only operators...
 */
string operatorsFrom(string str) {
    // TODO:
    //存储六个括号运算符
    string operators = "{}[]()";

    //最简单的情况：如果是空字符，则返回空
    if (str.empty()) {
        return "";
    }
    //递归策略：首个字符的判断返回值 + 剩下字符的递归函数返回值
    // 1) 判断当前首个字符是否为括号，如是括号则返回该括号，如否则返回空字符；
    // 2) 调用该函数继续判断剩下的字符串，把他们的返回结果加到结果中
    else {
        return ((operators.find(str[0]) != operators.npos) ?charToString(str[0]): "") + operatorsFrom(str.substr(1));
    }
}

STUDENT_TEST("operatorsFrom on simple example2") {
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom("0["), "[");
    EXPECT_EQUAL(operatorsFrom("abc{]}[[])"), "{]}[[])");
}

/*
 * Func: conrims that those operators are balanced
 * Output: bool value
 * strlib... stringContains(string, substr)--return bool    stringReplace(str, old, new)--return a new string
 */
bool operatorsAreMatched(string ops) {
    /* TODO: */
    // 最简单的情况：空字符串则返回 true
    if (ops.empty()) {
        return true;
    }
    //递归策略：判断当前是否存在一对括号（分别判断三种类型, ||），如果存在则去除该种类型的对括号（将括号Replace为“”）
    //剩下的字符串继续调用函数进行判断，直至最后为空字符则 true;
    else {
        return (stringContains(ops, "()") && operatorsAreMatched(stringReplace(ops, "()", ""))) ||
                (stringContains(ops, "[]") && operatorsAreMatched(stringReplace(ops, "[]", ""))) ||
                (stringContains(ops, "{}") && operatorsAreMatched(stringReplace(ops, "{}", "")));
    }
}

STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched(""));
    EXPECT(operatorsAreMatched("[(){}]"));
    EXPECT(operatorsAreMatched("(){([])(())}"));
    EXPECT_EQUAL(operatorsAreMatched("[]{{}"), false);
}
/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
