/**
 * File: warmup.cpp
 * ----------------
 * This code is provided as part of an exercise to practice using a
 * debugger. This program has buggy use of recursion.
 */

#include <iostream>    // for cout, endl
#include "error.h"
#include "random.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This is a recursive implementation of a function to calculate
 * factorials. This function works correctly for positive values
 * of n whose computed factorial value does not overflow the int
 * data type (n <= 12).
 *
 * As currently written, this function does not correctly handle
 * negative inputs.
 */
int factorial(int n) {
    if (n <= 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

//STUDENT_TEST("See wrong result of factorial(-3)") {
//    EXPECT_ERROR(factorial(-3));
//}

/* The myPower function computes exponentiation.
 * The call myPower(base, exp) returns base raised to the power exp.
 * Note that either/both of base and exp can be negative.
 * The calculation is done using type double to allow for the result
 * to have a fractional component, such as would be needed for
 * a negative exponent.
 *
 * WARNING: The given implementation is BUGGY! It works mostly correctly
 * but it has some bugs that you should find and fix. Refer to the
 * writeup for further instructions on how to do so.
 */
double myPower(int base, int exp) {
    if (exp == 0) {         // handle zero exp
        return 1;
    }
//    else if (base == 0) { // handle zero base
//        return 0;
//    }
    else if (exp < 0) {   // handle negative exp
        return 1.0 / myPower(base, -exp);
    }
//    else if (base < 0) {  // handle negative base
//        return -myPower(-base, exp);
//    }
    else {                // both base and exp are positive
        return base * myPower(base, exp - 1);
    }
}

// add tests for range-generated inputs
STUDENT_TEST("myPower(), generated inputs") {
    for (int base = 1; base < 25; base++) {
        for (int exp = 1; exp < 10; exp++) {
            EXPECT_EQUAL(myPower(base, exp), pow(base, exp));
        }
    }
}

// add tests for range-generated negative inputs
STUDENT_TEST("myPower(), generated negative inputs") {
    EXPECT_EQUAL(myPower(1, -1), pow(1, -1));
    EXPECT_EQUAL(myPower(-2, -1), pow(-2, -1));
    EXPECT_EQUAL(myPower(-3, -3), pow(-3, -3));
    EXPECT_EQUAL(myPower(-5, 2), pow(-5, 2));

    for (int base = -5; base < 1; base++) {
        for (int exp = 0; exp < 5; exp++) {
            EXPECT_EQUAL(myPower(base, exp), pow(base, exp));
        }
    }
    for (int base = 1; base < 5; base++) {
        for (int exp = -5; exp < 0; exp++) {
            EXPECT_EQUAL(myPower(base, exp), pow(base, exp));
        }
    }

}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Confirm result of factorial(7)") {
    EXPECT_EQUAL(factorial(7), 7*6*5*4*3*2);
}

PROVIDED_TEST("myPower(), compare to library pow(), fixed inputs") {
    EXPECT_EQUAL(myPower(7, 0), pow(7, 0));
    EXPECT_EQUAL(myPower(10, 2), pow(10, 2));
    EXPECT_EQUAL(myPower(5, -1), pow(5, -1));
    EXPECT_EQUAL(myPower(-3, 3), pow(-3, 3));
}

PROVIDED_TEST("myPower(), generated inputs") {
    for (int base = 1; base < 25; base++) {
        for (int exp = 1; exp < 10; exp++) {
            EXPECT_EQUAL(myPower(base, exp), pow(base, exp));
        }
    }
}

