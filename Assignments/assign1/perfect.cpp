/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO:
 * 一种优化的寻找完全数算法，只需穷举到N的平方根附近；
 */
long smarterSum(long n) {
    // TODO:
    //先走一个0和1
    long smarterTotal = 0;
    if (n == 0 || n == 1)
    {
        return 0;
    }
    //for-loop, 除数从1增加到小于或等于平方根的最接近整数
    for (long divisor = 1, max = sqrt(n); divisor <= max ; divisor++)
    {
        //如果n能被divisor整除，则divisor是因子，需要加和
        //并且如果最大的divisor不是N的平方根，那么必然存在另一个因子 n/divisor，一起加和
        if (n % divisor == 0)
        {
            smarterTotal += divisor;
            if (divisor != 1 && n != max * max)
            {
                smarterTotal += n/divisor;
            }
        }
    }
    return smarterTotal;
}

/* TODO:
 * 判断是否是完全数，返回bool值；
*/
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return (n != 0) && (n == smarterSum(n));
}

/* TODO:
 * 寻找指定值范围内的完全数，并打印出来
 */
void findPerfectsSmarter(long stop) {
     // TODO: for-loop，调用判断函数，如果是完全数则打印出来
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        //每隔若干数打印一个点，显示正在寻找中
        if (num % 20000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO:
 * 基于欧几里得优化的完全数搜索算法
 */
long findNthPerfectEuclid(long n) {
    //TODO: 通过公式计算m值，判断m是否为素数-即完全数，通过m计算得到一个新的完全数
    //k为迭代数，nn为第几个完全数的标识
    long nn = 0;
    long k = 1;
    while (true)
    {
        long m = pow(2, k) - 1;
        //如m为完全数，则得到第nn个完全数，如nn符合需求，则返回该完全数；
        if (smarterSum(m) == 1)
        {
            nn++;
            if (nn == n)
            {
                return m * pow(2, k - 1);
            }
        }
        k++;
    }
    return 0;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
STUDENT_TEST("~~~~Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10, findPerfects(10));
    TIME_OPERATION(1000, findPerfects(1000));
    TIME_OPERATION(2000, findPerfects(2000));
    TIME_OPERATION(30000, findPerfects(30000));
    TIME_OPERATION(60000, findPerfects(60000));
    TIME_OPERATION(120000, findPerfects(120000));
    TIME_OPERATION(240000, findPerfects(240000));
}

STUDENT_TEST("Test negative values: ") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-99));
    EXPECT(!isPerfect(-999));
}

****************************************************************************************//
//Test for smarter functions
STUDENT_TEST("Test smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), divisorSum(1));
    EXPECT_EQUAL(smarterSum(4), divisorSum(4));
    EXPECT_EQUAL(smarterSum(6), divisorSum(6));
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
}

STUDENT_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(28));
}

STUDENT_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfectSmarter(12));
    EXPECT(!isPerfectSmarter(98765));
}

STUDENT_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
}

STUDENT_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfectSmarter(33550336));
}

STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(30000, findPerfectsSmarter(30000));
    TIME_OPERATION(60000, findPerfectsSmarter(60000));
    TIME_OPERATION(120000, findPerfectsSmarter(120000));
    TIME_OPERATION(240000, findPerfectsSmarter(240000));
    TIME_OPERATION(2400000, findPerfectsSmarter(2400000));
    TIME_OPERATION(4800000, findPerfectsSmarter(4800000));
    TIME_OPERATION(9600000, findPerfectsSmarter(9600000));
    TIME_OPERATION(36000000, findPerfectsSmarter(36000000));
}

**************************************************************************************//
//Test for findNthPerfectEucli
STUDENT_TEST("Test findNthPerfectEucli") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(2)));
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}


STUDENT_TEST("Time trials of findNthPerfectEucli") {
    TIME_OPERATION(5, findNthPerfectEuclid(5));
    TIME_OPERATION(6, findNthPerfectEuclid(6));
}
