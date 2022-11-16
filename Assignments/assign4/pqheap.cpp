#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQHEAp class.
 * 初始分配到内存数量-10，动态数组-指针，已使用的内存-0；
 * 父节点索引、左/右子节点均初始为-NONE
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated];
    _numFilled = 0;

    parenentIndex = NONE;
    leftChildIndex = NONE;
    rightChildIndex = NONE;
}

/*
 * The array memory that was allocated for the PQArray is deleted here.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

// help Function: 扩容
void PQHeap::enlarge() {
    // 备份
    DataPoint* old_elements = _elements;
    int old_numAllocated = _numAllocated;
    // double the size
    _numAllocated *= 2;
    _elements = new DataPoint[_numAllocated];
    // 从备份中复制到扩容后的向量
    for (int i = 0; i < old_numAllocated; i++) {
        _elements[i] = old_elements[i];
    }
    // 释放旧内存
    delete[] old_elements;
}

/*
* Adds a new element into the queue. O(log n),
* 实现将一个新元素排入队列中，按照优先值顺序插入到二叉树中
* 插入规则：从最末位置开始，如果新元素的优先值其父节点的优先值，则与父节点交换；
*/

#define rec // ite为迭代版本， rec为递归版本
#ifdef ite
void PQHeap::enqueue(DataPoint elem) {
    // 1. 判定是否扩容
    if (_numFilled == _numAllocated) {
        enlarge();
    }
    // 2. tempTarget-当前加入位置，初始化为末尾位置；
    int tempTarget = _numFilled;

    // 3. 提取当前位置的父节点位置，如不为空NONE，则进入判定
    // 如父节点的优先值大于当前位置，则需要交换位置
    int upperindex = getParentIndex(_numFilled);
    while (upperindex != NONE && _elements[upperindex].priority > elem.priority) {
        *(_elements + tempTarget) = *(_elements + upperindex);
        // 交换位置后，将当前位置更新为父节点
        tempTarget = upperindex;
        upperindex = getParentIndex(tempTarget);
    }
    // 4. 最终加入的位置
    _elements[tempTarget] = elem;
    // 5. 实际数量 +1
    _numFilled++;
}
#endif

#ifdef rec
// 递归实现函数：根据所要入队的元素和当前索引，返回最终实现入队的索引值
int PQHeap::enqueueRec(DataPoint& elem, int index) {
    int parent = getParentIndex(index);
    // base case1: 没有父节点，说明已经到根节点啦，则返回根节点Index
    if (parent == NONE) {
        return index;
    }
    // base case2: 判定当前索引位置的父节点，如果小于入队元素的优先值，则不能继续向上了，返回当前索引值
    else if (_elements[parent].priority <= elem.priority) {
        return index;
    }
    // 基本递归：当前索引的父节点优先值比入队元素更大，说明还能往上一层
    else {
        // 先交换元素
        *(_elements + index) = *(_elements + parent);
        return enqueueRec(elem, parent);
    }
}
// 递归包装函数
void PQHeap::enqueue(DataPoint elem) {
    if (_numFilled == _numAllocated) {
        enlarge();
    }
    int targetIndex = enqueueRec(elem, _numFilled);
    _elements[targetIndex] = elem;
    _numFilled++;
}
#endif

/*
* Returns, but does not remove, the element that is frontmost.
* If the priority queue is empty, this function calls error().O(1).
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQHeap is empty!");
    }

    return _elements[0];
}

/*
* Removes and returns the element that is frontmost in this priority queue.
* The frontmost element is the one with the most urgent priority.  O(log n)
* 将顶层元素挤出，将最末端元素加入到顶端，再调整二叉树的排序
* 调整规则：从顶层元素开始，如果原末端元素的优先值大于下一层子节点（两个子节点优先值较小的节点），则交换元素；
*/
#define deq_rec // ite为迭代版本， rec为递归版本

#ifdef deq_ite
DataPoint PQHeap::dequeue() {
    // 1.复制一份第一个元素，如果为空，peek()函数会报错
    DataPoint front = peek();
    // 2.从第一层开始
    int tempTarget = 0;
    // 2.a 提取下一层子节点中的优先值较小节点
    int underSmaller = getSmallerChildIndex(tempTarget);
    // 2.b 如果下一层子节点存在，并且下一层子节点的优先值比小于原末端元素，则需要交换元素
    while (underSmaller != NONE && _elements[size() - 1].priority > _elements[underSmaller].priority) {
        *(_elements + tempTarget) = *(_elements + underSmaller);
        tempTarget = underSmaller;
        underSmaller = getSmallerChildIndex(tempTarget);
    }
    // 3.原末端元素最终插入的位置
    _elements[tempTarget] = _elements[size() - 1];
    // 4.实际元素数量-1
    _numFilled--;
    return front;
}
#endif

#ifdef deq_rec
// 递归版本：根据输入的节点（实际为原末端节点元素），从上到小根据优先值判定最终它的位置；
// 中间过程涉及到当前位置与下一层子节点的位置交换
// 返回值为原末端节点元素最终插入的位置
int PQHeap::dequeueRec(DataPoint& elem, int target) {
    int leftchild = getLeftChildIndex(target);
    // base case1: 没有子节点，只有顶层一个元素，则返回Index-0
    if (leftchild == NONE) {
        return target;
    }
    // *提取子节点中优先值较小的Index
    int smallchild = NONE;
    int rightchild = getRightChildIndex(target);
    if (rightchild == NONE) {
        smallchild = leftchild;
    } else {
        smallchild = _elements[leftchild].priority > _elements[rightchild].priority ? rightchild: leftchild;
    }
    // base case2: 当前位置的优先值小于下一层子节点，则满足规则，返回当前Index
    if (_elements[smallchild].priority > elem.priority) {
        return target;
    }
    // 递归情况： 当前位置的优先值大于下一层子节点，则交换位置，并更新当前位置为该层的子节点
    else {
        *(_elements + target) = *(_elements + smallchild);
        return dequeueRec(elem, smallchild);
    }
}

DataPoint PQHeap::dequeue() {
    //1.存储一份当前顶层节点元素，如为空则peeek()函数报错
    DataPoint front = peek();
    // 2.从顶层开始，对原末端节点元素判定最终位置，并确定好顺序
    int target = dequeueRec(_elements[size() - 1], 0);
    _elements[target] = _elements[size() - 1];
    //3. 元素-1，并返回顶层元素
    _numFilled--;
    return front;
}
#endif

/*
* Returns whether this priority queue is empty. O(1).
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
* Returns the count of elements in this priority queue. O(1)
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
* Removes all elements from the priority queue. O(1)
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * Print out the array representing the heap for debugging purposes
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "]" << _elements[i] << endl;
    }
}

/*
 * Confirm the internal state of member variables appears valid.
 * 确认父节点比子节点优先，即父节点的优先值比子节点的小
 */
void PQHeap::validateInternalState() const {
    // If there are more elements than spots in the array, we have a problem.
    if (_numFilled > _numAllocated) {
        error("Too many elements in not enough space");
    }
    // Loop over the elements in the array and compare priority of parents element and child elemets;
    // If current element has smaller prioritythan the parent element, use error to report this problem.
    if (size() > 1) {
        for (int i = 1; i < size(); i++) {
            if (_elements[i].priority < _elements[getParentIndex(i)].priority) {
                printDebugInfo("ValidateInternalState");
                error("PQHeap has elements out of order at index " + integerToString(i));
            }
        }
    }
}

/*
 * 返回父节点的index值，如果没有父节点 则返回NONE；
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0) {
        return NONE;
    } else {
        return (child - 1) /2;
    }
}

/*
 * 返回左边子节点的index值，如果没有则返回NONE
 */
int PQHeap::getLeftChildIndex(int parent) const {
    if (parent * 2 + 2 > size()) {
        return NONE;
    }
    return parent * 2 + 1;
}

/*
 * 返回右边子节点的index值，如果没有则返回NONE
 */
int PQHeap::getRightChildIndex(int parent) const {
    if (parent * 2 + 3 > size()) {
        return NONE;
    }
    return parent * 2 + 2;
}

// 返回子节点中优先级较低的index值，如只要一个左节点则返回该元素的index值，如没有子节点则返回NONE；
int PQHeap::getSmallerChildIndex(int parent) const {
    if (getRightChildIndex(parent) == NONE) {
        return getLeftChildIndex(parent);
    } else {
        return _elements[getLeftChildIndex(parent)].priority > _elements[getRightChildIndex(parent)].priority?
                getRightChildIndex(parent): getLeftChildIndex(parent);
    }
//    int leftIndex = getLeftChildIndex(parent);
//    int rightIndex = getRightChildIndex(parent);
//    if (rightIndex == NONE) {
//        return leftIndex;
//    } else {
//        return _elements[leftIndex].priority > _elements[rightIndex].priority? rightIndex : leftIndex;
//    }
}
/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */

STUDENT_TEST("PQHeap example from writeup") {
    PQHeap pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

STUDENT_TEST("PQHeap, enqueue only, validate at every step") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

STUDENT_TEST("PQHeap: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("PQHeap: dequeue or peek on empty queue raises error") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

STUDENT_TEST("PQHeap, dequeue, validate at every step") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

STUDENT_TEST("PQHeap, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

STUDENT_TEST("PQHeap, sequence of mixed operations") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

STUDENT_TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

void fillQueueheap(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueueheap(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueueheap(pq, n));
    TIME_OPERATION(n, emptyQueueheap(pq, n));
}

STUDENT_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;
    for (int n = 100000; n < 2000000; n= n *2) {
        TIME_OPERATION(n, fillQueueheap(pq, n));
        TIME_OPERATION(n, emptyQueueheap(pq, n));
    }
}
STUDENT_TEST("PQHeap timing test, fillQueue....enqueue operation") {
    PQHeap pq;
    for (int n = 50000; n < 10000000; n= n *2) {
        TIME_OPERATION(n, fillQueueheap(pq, n));
        emptyQueueheap(pq, n);
    }
}

STUDENT_TEST("PQHeap timing test, emptyQueue....dequeue operation") {
    PQHeap pq;
    for (int n = 50000; n < 10000000; n= n *2) {
        fillQueueheap(pq, n);
        TIME_OPERATION(n, emptyQueueheap(pq, n));
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
