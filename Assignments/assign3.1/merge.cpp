/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
//#include <math.h>
using namespace std;

/*
 * Func: 将两个独立的有序序列合并成一个新的序列；
 * 输入：a和b都是按从小到大排列的队列；
 * 输出：返回的新队列也是按从小到大的排列顺序；
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    //定义中间变量：存储上一次入队的元素值
    int lastitem = INT_MIN;
    /* TODO: */
    // while循环：每次循环往新队列中加入一个元素item；当a和b两个队列都为空时，结束循环
    while (!a.isEmpty() || !b.isEmpty()) {
        //定义中间变量：item下一个入队的值；
        int item = 0;
        //如果a为空，则直接将b队列的前部元素推出，并赋值到item;
        //如果b为空，则直接将a队列的前部元素推出，并赋值到item;
        if (a.isEmpty()) {
            item = b.dequeue();
        } else if (b.isEmpty()) {
            item = a.dequeue();
        }
        //如果a和b都不为空，则对比两个队列的前部元素，将值较小的元素从队列推出；
        else {
            if (a.peek() <= b.peek()) {
                item = a.dequeue();
            } else {
                item = b.dequeue();
            }
        }
        //如果item小于上一轮入队的值，则说明a或b队列中有元素没有按从小到大的顺序；
        if (lastitem > item) {
            error("Queue a or b could have some problems~~~~");
        }
        //入队 
        result.enqueue(item);
        //存储本轮的入队值
        lastitem = item;
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}


// Func: 递归多路合并的实现函数，函数参数中加入两个索引参数start和end
//二分法、分而治之，不断将Vector划分为两部分，直至只剩单个或两个元素
Queue<int> recMultiMergeRef(const Vector<Queue<int>>& all, int start, int end) {
    Queue<int> result;
    // base case
    // base case 1:当划分到只剩一个元素，此时起止索引值相等，调用binaryMerge或者直接创建一个单元素的队列
    if (start >= end) {
        //return binaryMerge(all[start], {});
        return Queue<int> {all[start]};
    }
    // base case 2:当划分到只剩两个元素，此时起止索引值相差1，调用binaryMerge返回一个合并后的队列；
    if (start == end - 1) {
        return binaryMerge(all[start], all[end]);
    }
    // 递归实现：不断对起止索引进行切片、取中值，降低问题规模
    // 前半部分first的起止索引为 start ~ mid
    // 后半部分last的起止索引为 mid+1 ~ end
    // 最后合并first和last两部分；
    else {
        int mid = (start + end)/2;
        Queue<int> first = recMultiMergeRef(all, start, mid);
        Queue<int> last = recMultiMergeRef(all, mid + 1, end);
        result = binaryMerge(first, last);
    }
    return result;
}

// Func： 递归多路合并函数，使用包装函数
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    // 调用实际递归函数，起止索引分别为：0和最大索引
    int last = all.size() - 1;
    result = recMultiMergeRef(all, 0, last);
    return result;
}

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("binaryMerge--edge tests: while a queue is empty") {
    Queue<int> a = {};
    Queue<int> b = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(binaryMerge(a, b), expected);

    a = {};
    b = {1};
    expected = {1};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {};
    b = {-1, 1, 2, 3, 5, 6};
    expected = {-1, 1, 2, 3, 5, 6};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge--tests for complicated queues") {
    Queue<int> a = {0, 0, 1, 1};
    Queue<int> b = {3};
    Queue<int> expected = {0, 0, 1, 1, 3};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {-1, 5};
    b = {-1, 5, 5};
    expected = {-1, -1, 5, 5, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {2};
    b = {2, 2, 2, 2, 2};
    expected = {2, 2, 2, 2, 2, 2};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {1, 3, 5, 7, 9};
    b = {2, 2, 4, 6, 8, 10};
    expected = {1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge--tests for problemd queues") {
    Queue<int> a = {0, 0, 1, 0};
    Queue<int> b = {3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));

    a = {2, -1};
    b = {-1, 5, 5};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));

    a = {};
    b = {1, 2, 4, 3, 8, 7};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));

    a = {1, 3, 5, 7, 6};
    b = {2, 2, 4, 6, 8, 10};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));

    a = {2};
    b = {2, 2, 2, 2, 2};
    EXPECT_NO_ERROR(binaryMerge(a, b));
    EXPECT_NO_ERROR(binaryMerge(b, a));

    a = {1, 3, 5, 7, 9};
    b = {2, 2, 4, 6, 8, 10};
    EXPECT_NO_ERROR(binaryMerge(a, b));
    EXPECT_NO_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("More tests for recMultiMerge, compare to naiveMultiMerge") {
    int n = 2000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("More tests for Time recMultiMerge operation, input sizes-9 million") {
    int n = 9000000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("big-O tests: Time binaryMerge operation for different input sizes") {
    for (int n = 5000000; n < 200000000; n *= 2) {
        Queue<int> a = createSequence(n);
        Queue<int> b = createSequence(n);
        TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
    }
}

STUDENT_TEST("big-O tests: Time naiveMultiMerge operation") {
    for (int n = 8000; n < 130000; n *= 2) {
        int k = n/10;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), naiveMultiMerge(all));
    }

    for (double i = 0.05; i < 0.5; i += 0.1) {
        int n = 32000;
        int k = n * i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), naiveMultiMerge(all));
    }
}


STUDENT_TEST("big-O tests: Time naiveMultiMerge operation") {
    for (int n = 8000; n < 130000; n *= 2) {
        int k = n/10;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }

    for (double i = 0.05; i < 0.5; i += 0.1) {
        int n = 32000;
        int k = n * i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
