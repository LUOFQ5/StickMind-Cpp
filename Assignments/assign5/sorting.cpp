#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * 将输入链表划分为小于、等于或大于所选基准的三个子链表;只是将节点分配到三个子链表中，不涉及新分配节点！
 * 输入目标节点front，smaller-小于基准的子链表，bigger-大于基准的子链表； equal-等于基准的子链表；
 */
void partition(ListNode*& front, ListNode*& smaller, ListNode*& bigger, ListNode*& equal) {
    // 三个子链表的末端指针-起到移动作用，
    ListNode* copybig = bigger;
    ListNode* copysmall = smaller;
    ListNode* copyequal = equal;

    // 1. 首先链表，第一个元素为基准，应当放入到equal里面
    equal = front;
    copyequal = equal;
    // 2. 开始遍历：
    front = front->next;
    while (front != nullptr) {
        // a. 如果当前节点大于基准值，则需要加入到bigger链表
        if (front->data > equal->data) {
            // if-如果bigger链表为空，则当前节点直接作为其第一个节点
            if (bigger == nullptr) {
                bigger = front;
                copybig = bigger;
            }
            // else-否则，借用移动指针加入到bigger链表的末尾
            else {
                copybig->next = front;
                copybig = copybig->next;
            }
        }
        // b.如果当前节点小于基准值，则加入到smaller链表
        else if (front->data < equal->data) {
            // if-如果samller链表为空，则当前节点直接作为其第一个节点
            if (smaller == nullptr) {
                smaller = front;
                copysmall = smaller;
            }
            // else-否则，借用移动指针加入到smaller链表的末尾
            else {
                copysmall->next = front;
                copysmall = copysmall->next;
            }
        }
        // c。如果当前节点等于基准值，则加入到equal链表的末尾
        else {
                copyequal->next = front;
                copyequal = copyequal->next;
            }
        front = front->next;
    }
    // 3. 将末端指针之后进行截断（原链表可能会存在后续的节点，虽然这些节点已加入到另外两个子链表中）
    if (copysmall != nullptr) {
        copysmall->next = nullptr;
    }
    if (copybig != nullptr) {
        copybig->next = nullptr;
    }
    if (copyequal != nullptr) {
        copyequal->next = nullptr;
    }
    return;
}

/*
 * 将三个子链表首尾相连，组合成排好顺序的链表
 * 输入：本质上都为指针，其中front-为最终排序的链表指针，三个子链表可能存在空链表的情况
 * smaller-存放小于基准值结点的链表，bigger-存放大于基准值结点的链表，equal-存放等于基准值的结点的链表
 */
void concatenate(ListNode*& front, ListNode*& smaller, ListNode*& bigger, ListNode*& equal) {
    // 声明尾部指针-移动指针
    ListNode* tail = front;
    // 1. 从小于基准值的链表开始
        // a. 如果front为空，说明当前为第一个结点，直接加入到链表
    if (front == nullptr) {
        front = smaller;
        tail = front;
    }
        // b. 如果front不为空，则确保tail为当前链表的末端，再移动到当前smaller链表的末端
    else {
        while (tail != nullptr) {
            tail = tail->next;
        }
    }
    // 2. 前进到等于基准值的链表
        // a. 如果front为空，说明当前smaller为空，则直接加入equal链表
    if(front == nullptr) {
        front = equal;
        tail = front;
    }
        // b. 否则，先确保tail为当前链表的末端，再将当前equal链表加入到末端后
    else {
        while (tail->next != nullptr) {
            tail = tail->next;
        }
        tail->next = equal;
    }
    // 3. 前进到bigger链表
    //    a.如果为空，则连接结束
    if (bigger == nullptr) {
        return;
    }
    //    b. 否则先确保tail为当前链表的末端，然后将当前bigger链表加入到末端
    else {
        while (tail->next != nullptr) {
            tail = tail->next;
        }
        tail->next = bigger;
    }
}

//递归实现快速排序算法，将输出链表的节点重新排列为顺序链表
//将输入链表划分为子链表，递归地对子链表进行排序，然后将排序后的子链表连接成一个整体
void quickSort(ListNode*& front) {
    //  base case 1:
    if (front == nullptr) {
        return;
    }
    // 递归情况：
    else {
        // a.  先开始第一轮切分，得到三个子链表
        ListNode* smaller = nullptr;
        ListNode* bigger = nullptr;
        ListNode* equal = nullptr;
        partition(front, smaller, bigger, equal);
        // b.  第二论切分smaller，直至最小单位链表只有一个结点，划分到equal子链表
        quickSort(smaller);
        // c.  第三轮切分bigger，直至最小单位链表只有一个结点，划分到equal子链表
        quickSort(bigger);
        // d.  从两个最小单位链表开始，不断聚合链表，直至完整的顺序链表
        concatenate(front, smaller, bigger, equal);
    }
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * 工具函数：遍历给定的链表，并打印每个节点的数据值；
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 * 工具函数：为给定链表中的所有节点释放内存，壁面内存泄漏
 */
void deallocateList(ListNode* front) {
    while (front != nullptr) {
        ListNode* temp = front;
        front = front->next;
        delete temp;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 * 工具函数：输入一个向量，返回一个包含相同顺序和相应值的链表
 */
ListNode* createList(Vector<int> values){
    // 如果输入向量为空，则返回空指针
    if (values.isEmpty()) {
        return nullptr;
    }
    // 先创建一个head指针，对应值为最后一个元素
    ListNode* head = new ListNode(values[values.size() - 1], nullptr);
    // 剩余元素，逆序创建nodes
    for (int i = values.size() - 1; i > 0; i--) {
        head = new ListNode(values[i-1], head);
    }
    return head;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 * 工具函数：比较链表和向量是否相等，如果两者包含相同顺序和值，则返回true;
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    // 1. 如果输入front为空
    if (front == nullptr) {
        if (v.isEmpty()) {
            return true;
        } else {
            return false;
        }
    }
    ListNode* temp = front;
    // 2. 迭代：按向量元素为基准，依次比较每一个node，直至倒数第二个
    if (v.isEmpty()) {
        return false;
    }
    for (int i = 0; i < v.size(); i++) {
        // 如果此时temp为空，说明链表比向量的数量少;
        // 或者链表的值跟向量的值不想等
        if (temp == nullptr || temp->data != v[i]) {
            return false;
        }
        // 如果temp到达倒数第一个元素，但此时向量不是最后一个元素，说明链表数量比向量多
        // 或者链表的值跟向量的值不相等
        if (i == v.size() - 1 && temp->next != nullptr) {
            return false;
        } else
        {
            temp = temp->next;
        }
    }
    return true;
}



STUDENT_TEST("Tests for creatLists: zero elements, one elements and multi elements"){
    /* Test create list function. */
    ListNode* studentList1 = createList({});
    printList(studentList1);
    EXPECT_EQUAL(studentList1, nullptr);

    ListNode* studentList2 = createList({0});
    ListNode* pt0 = new ListNode(0, nullptr);
    EXPECT_EQUAL(studentList2->data, pt0->data);
    deallocateList(pt0);

    /* Test create list function. */
    ListNode* studentList3 = createList({1, 1, 1, 1});
    printList(studentList3);
    ListNode* cur = studentList3;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, 1);
        cur = cur->next;
    }
    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList1);
    deallocateList(studentList2);
    deallocateList(studentList3);
}

STUDENT_TEST("Tests for equivalent: zero elements, one elements and multi elements."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    EXPECT(areEquivalent(testList, {}));

    testList = new ListNode(3, testList);
    EXPECT(!areEquivalent(testList, {}));
    EXPECT(areEquivalent(testList, {3}));

    testList = new ListNode(3, testList);
    EXPECT(!areEquivalent(testList, {3}));
    EXPECT(!areEquivalent(testList, {3, 3, 3}));

    testList = new ListNode(1, testList);
    EXPECT(!areEquivalent(testList, {1, 3}));
    EXPECT(areEquivalent(testList, {1, 3, 3}));
    EXPECT(!areEquivalent(testList, {1, 3, 3, 3}));

    testList = new ListNode(1, testList);
    EXPECT(areEquivalent(testList, {1, 1, 3, 3}));

    ListNode* testList2 = nullptr;
    for (int i = 0; i <100; i++) {
        testList2 = new ListNode(i, testList2);
    }
    Vector<int> num;
    for (int i = 99; i >= 0; i--) {
        num.add(i);
    }
    EXPECT(areEquivalent(testList2, num));

    deallocateList(testList);
    deallocateList(testList2);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Tests for sorting: empty listnode, one-node list."){
    /* Initialize vector to sequence of values */
    Vector<int> values0 = {};
    /* Create equivalent list to use as test input */
    ListNode* list0 = createList(values0);
    /* Sort the list */
    quickSort(list0);
    values0.sort();
    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list0, values0));

    Vector<int> values1 = {0};
    ListNode* list1 = createList(values1);
    quickSort(list1);
    values1.sort();
    EXPECT(areEquivalent(list1, values1));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list0);
    deallocateList(list1);
}

STUDENT_TEST("Tests for sorting: two or three nodes in the list."){
    Vector<int> values0 = {1, 2};
    ListNode* list0 = createList(values0);
    quickSort(list0);
    values0.sort();
    EXPECT(areEquivalent(list0, values0));
    deallocateList(list0);

    Vector<int> values1 = {2, 1};
    ListNode* list1 = createList(values1);
    quickSort(list1);
    values1.sort();
    EXPECT(areEquivalent(list1, values1));
    deallocateList(list1);

    Vector<int> values22 = {0, 0};
    ListNode* list22 = createList(values22);
    quickSort(list22);
    values22.sort();
    EXPECT(areEquivalent(list22, values22));
    deallocateList(list22);

    Vector<int> values2 = {1, 2, 0};
    ListNode* list2 = createList(values2);
    quickSort(list2);
    values2.sort();
    EXPECT(areEquivalent(list2, values2));
    deallocateList(list2);

    Vector<int> values3 = {0, -1, 1};
    ListNode* list3 = createList(values3);
    quickSort(list3);
    values3.sort();
    EXPECT(areEquivalent(list3, values3));
    deallocateList(list3);

    Vector<int> values33 = {3, 3, 3};
    ListNode* list33 = createList(values33);
    quickSort(list33);
    values33.sort();
    EXPECT(areEquivalent(list33, values33));
    deallocateList(list33);
}


STUDENT_TEST("Tests for sorting: more nodes."){
    /* Initialize vector to sequence of values */
    Vector<Vector<int>> values = {{1,2,3,4},{-99,-10,0,8,9},{1,1,1,1,1,1},{-5,-3,-1,-1,0,0,2,5,6,7,7},{9,-1,2,-4},{1,-1,2,-2,-3,4,-5,6}};
    for (int i=0; i < values.size(); i++) {
        /* Create equivalent list to use as test input */
        ListNode* list = createList(values[i]);
        /* Sort the list */
        quickSort(list);
        values[i].sort();
        /* Confirm sorted list matches sorted vector */
        EXPECT(areEquivalent(list, values[i]));
        deallocateList(list);
    }
}


PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}


STUDENT_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 80000;

    for(int n = startSize; n < 100*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

STUDENT_TEST("端到端测试：逆序") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 100; i > 0; i--) {
        values.add(i);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：正序") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(i);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：重复值") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(3);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：正负值") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 10; i++) {
        values.add(randomInteger(-100, 100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);
    EXPECT(areEquivalent(list, values));

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：随机值") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(randomInteger(0, 100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    EXPECT(areEquivalent(list, values));
    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}
