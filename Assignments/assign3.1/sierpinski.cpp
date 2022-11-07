/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * Func: Draw the Sierpinski according to the 'order'
 * Return: total nums of the black Triangles.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: */
    //边界情况：order=1时，根据当前点填充1个三角形，返回数量1
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    //递归策略：不断降低order；相比order，order-1对应三角形的点坐标分别为 原来one/two/three相邻点的中点
    //每次order - 1时，从1个三角形，填充到3个三角形；直至order = 0，调用实际填充函数；
    else {
        double x12 = 0.5 * one.x + 0.5 * two.x;
        double y12 = 0.5 * one.y + 0.5 * two.y;
        double x23 = 0.5 * two.x + 0.5 * three.x;
        double y23 = 0.5 * two.y + 0.5 * three.y;
        double x13 = 0.5 * one.x + 0.5 * three.x;
        double y13 = 0.5 * one.y + 0.5 * three.y;

        return drawSierpinskiTriangle(window, one, GPoint(x12, y12), GPoint(x13, y13), order - 1)
             + drawSierpinskiTriangle(window, GPoint(x12, y12), two, GPoint(x23, y23), order - 1)
             + drawSierpinskiTriangle(window, GPoint(x13, y13), GPoint(x23, y23), three, order - 1);
        }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

