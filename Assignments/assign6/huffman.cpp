#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 */
// 解码操作，基于编码tree和 压缩后的编码序列，重建出原文信息-string
//#define ref_sol
#define luo_sol
#ifdef luo_sol
void decodeTextRec(EncodingTreeNode* tree, EncodingTreeNode* tempTree, Queue<Bit>& messageBits, string& result) {
    // edge case1: 退出条件
    if (messageBits.isEmpty()) {
        return;
    }
    // 当前不是树叶节点，取出当前节点值，选择进入左或者右的子节点
    if (messageBits.dequeue() == 0) {
        tempTree = tempTree->zero;
    } else {
        tempTree = tempTree->one;
    }
    // case1: 达到一个树叶结点，取结点上的char值，然后进入下一个路径--从顶部tree重新解码
    if (tempTree->isLeaf()) {
        result += tempTree->getChar();
        decodeTextRec(tree, tree, messageBits, result);
    }
    // case2: 该结点为中间结点，当前路径未结束，继续前进。
    else {
        decodeTextRec(tree, tempTree, messageBits, result);
    }
}
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string result;
    // 特殊情况：如果tree只有一个节点，并且messageBit多次重复该节点
    if (tree->isLeaf()) {
        while (!messageBits.isEmpty()) {
            messageBits.dequeue();
            result += tree->getChar();
        }
    } else {
        decodeTextRec(tree, tree, messageBits, result);
    }
    return result;
}
#endif

#ifdef ref_sol
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string message;
    if (tree->isLeaf()) { // 一般最少为两个字符，此处仅作测试
        message += tree->getChar();
        return message;
    }
    auto copy = tree;
    while (!messageBits.isEmpty()) {
        if (messageBits.dequeue() == 0) {
            copy = copy->zero;
        } else {
            copy = copy->one;
        }
        if (copy->isLeaf()) {
            message += copy->getChar();
            copy = tree;
        }
    }
    return message;
}
#endif
/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 */
// 解扁平：输入扁平化的位序列和字符序列，输出为原始编码树。
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    // edge case: 退出条件
    if (treeShape.isEmpty()) {
        return nullptr;
    }
    // 存储一个返回值-指针
    EncodingTreeNode* node= nullptr;
    // basic case1: 如果node为中间节点，则需要分别递归实现 left节点和right节点
    if (treeShape.dequeue() == 1) {
        EncodingTreeNode* left = unflattenTree(treeShape, treeLeaves);
        EncodingTreeNode* right = unflattenTree(treeShape, treeLeaves);
        node = new EncodingTreeNode(left, right);
//        node = new EncodingTreeNode(nullptr, nullptr);
//      node->zero = unflattenTree(treeShape, treeLeaves);
//      node->one = unflattenTree(treeShape, treeLeaves);

    }
    // basic case2: 如果node为叶节点，则需要创建一个有char值的指针
    else {
        node = new EncodingTreeNode(treeLeaves.dequeue());
    }
    return node;
}

/**
 * Decompress the given EncodedData and return the original text.
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 */
// 解压缩操作：
// 输入：压缩的数据结构体指针，包括treeShape-扁平化的位序列，treeLeaves-字符序列，messageBits-压缩编码序列
// 输出：解压后的字符信息-string
string decompress(EncodedData& data) {
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves);
    string texts = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return texts;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 */
// 构造最优哈夫曼树：输入字符窜，输出树节点
// 1.输入字符窜需要满足至少包含两个不同的字母，否则报错
// 2.连接相同权重节点的顺序可能不一样，只需保证最佳树是有效的

//helper func: 输入字符串，输出按字符词频统计的优先队列-数据类型是节点指针EncodingTreeNode*
PriorityQueue<EncodingTreeNode*> textToPq(string text) {
    Map<char, double> chars;
    for (auto item: text) {
        chars[item] += 1;
    }
    PriorityQueue<EncodingTreeNode*> orderedChars;
    for (char ch: chars) {
        EncodingTreeNode* node = new EncodingTreeNode(ch);
        orderedChars.enqueue(node, chars[ch]);
    }
    return orderedChars;
}
//递归实现函数：
EncodingTreeNode* buildHuffmanTreeRec(PriorityQueue<EncodingTreeNode*>& orderedChars) {
    //队列中只有一个节点，作为最终的指针结果tree
    if (orderedChars.size() < 2) {
        return orderedChars.dequeue();
    }
    //递归：此时队列中有超过两个节点
    //a. 取出优先级的前两个节点，存储优先级值
    int zeroPri = orderedChars.peekPriority();
    EncodingTreeNode* zero = orderedChars.dequeue();
    int onePri = orderedChars.peekPriority();
    EncodingTreeNode* one = orderedChars.dequeue();
    //b. 合并两个节点作为内部节点，新节点的优先值为叶子节点之和，并加入到优先队列
    EncodingTreeNode* tree = new EncodingTreeNode(zero, one);
    orderedChars.enqueue(tree, zeroPri + onePri);
    //c. 新的队列，递归调用函数
    return buildHuffmanTreeRec(orderedChars);
}

EncodingTreeNode* buildHuffmanTree(string text) {
    PriorityQueue<EncodingTreeNode*> orderedChars = textToPq(text);
    if (orderedChars.size() < 2) {
        error("The input text does not contain at least two distinct characters~~");
    }
    EncodingTreeNode* tree = buildHuffmanTreeRec(orderedChars);
    return tree;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 */
// helper function: 遍历节点树，对每一个叶子值-char生成对应的路径序列treeBits。
void BitsToChars(EncodingTreeNode* tree, Queue<Bit>& treeBits, Map<char, Queue<Bit>>& resultMap) {
    if (tree->isLeaf()) {
        resultMap[tree->getChar()] = treeBits;
    } else {
        Queue<Bit> copy0 = treeBits;
        copy0.enqueue(0);
        BitsToChars(tree->zero, copy0, resultMap);
        Queue<Bit> copy1 = treeBits;
        copy1.enqueue(1);
        BitsToChars(tree->one, copy1, resultMap);
    }
}

// 压缩-编码：根据字符串信息和编码树，输出编码序列；
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Queue<Bit> treeBits;
    Map<char, Queue<Bit>> BitChars;
    BitsToChars(tree, treeBits, BitChars);

    Queue<Bit> result;
    for (auto item: text) {
        Queue<Bit> temps = BitChars[item];
        while (!temps.isEmpty()) {
            result.enqueue(temps.dequeue());
        }
    }
    return result;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 */
// 展平树，生成扁平化的树型位序列和叶子-字符序列；
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (tree == nullptr) {
        return;
    }
    if (tree->isLeaf()) {
       treeShape.enqueue(0);
       treeLeaves.enqueue(tree->getChar());
   } else {
       treeShape.enqueue(1);
       flattenTree(tree->zero, treeShape, treeLeaves);
       flattenTree(tree->one, treeShape, treeLeaves);
   }

}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 */
// 最终的压缩函数，对输入的字符窜信息执行压缩操作，返回一个包含压缩信息的结构体。
EncodedData compress(string messageText) {
    if (messageText.length() < 2) {
        error("The input text doesn't contain at least tow characters.");
    }
    EncodingTreeNode* tree = buildHuffmanTree(messageText);
    Queue<Bit> messageBits = encodeText(tree, messageText);
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(tree, treeShape, treeLeaves);
    deallocateTree(tree);
    return EncodedData{treeShape, treeLeaves, messageBits};
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode* nodeT = new EncodingTreeNode('T');
    EncodingTreeNode* nodeR = new EncodingTreeNode('R');
    EncodingTreeNode* nodeS = new EncodingTreeNode('S');
    EncodingTreeNode* nodeE = new EncodingTreeNode('E');
    EncodingTreeNode* layer2 = new EncodingTreeNode(nodeR, nodeS);
    EncodingTreeNode* layer1 = new EncodingTreeNode(layer2, nodeE);
    EncodingTreeNode* layer0 = new EncodingTreeNode(nodeT, layer1);

    return layer0;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
//    if (t->isLeaf()) {
//        delete t;
//    } else {
//        deallocateTree(t->zero);
//        deallocateTree(t->one);
//    }

    if (!t->isLeaf()) {
        deallocateTree(t->zero);
        deallocateTree(t->one);
    }
    delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* TODO: Implement this utility function needed for testing. */
    if ((a == nullptr && b != nullptr) || (a !=nullptr && b == nullptr)){
        return false;
    }
    if (a == nullptr && b == nullptr) {
        return true;
    }

    if ((a->isLeaf() && !b->isLeaf()) || (!a->isLeaf() && b->isLeaf())) {
        return false;
    } else if (a->isLeaf() && b->isLeaf()) {
        if (a->getChar() != b->getChar()) {
            return false;
        }
    } else {
        return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
    }
    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */
STUDENT_TEST("Tests for createExampleTree, deallocateTree, and areEqual") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    EncodingTreeNode* tree1 = new EncodingTreeNode('a');
    EXPECT_EQUAL(areEqual(tree1, nullptr), false);
    EXPECT_EQUAL(areEqual(tree1, tree), false);

    EncodingTreeNode* tree2 = new EncodingTreeNode('b');
    EXPECT_EQUAL(areEqual(tree2, tree1), false);
    EncodingTreeNode* tree3 = new EncodingTreeNode('b');
    EXPECT_EQUAL(areEqual(tree2, tree3), true);
    EXPECT_EQUAL(areEqual(tree3, tree2), true);

    EncodingTreeNode* cpytree = createExampleTree(); // see diagram above
    EXPECT_EQUAL(areEqual(tree, cpytree), true);

    EXPECT_EQUAL(areEqual(tree->one, cpytree), false);
    EXPECT_EQUAL(areEqual(tree, cpytree->zero), false);

    deallocateTree(tree);
    deallocateTree(cpytree);
    deallocateTree(tree1);
    deallocateTree(tree2);
    deallocateTree(tree3);
}
STUDENT_TEST("decodeText, test for very small trees") {
    EncodingTreeNode* node1 = new EncodingTreeNode('R');
    EXPECT(node1 != nullptr);

    Queue<Bit> messageBits = {};
    EXPECT_EQUAL(decodeText(node1, messageBits), "");

    EncodingTreeNode* node11 = new EncodingTreeNode('L');
    EncodingTreeNode* node2 = new EncodingTreeNode(node1, node11);
    messageBits = {0,1};
    EXPECT_EQUAL(decodeText(node2, messageBits), "RL");
    messageBits = {1,0};
    EXPECT_EQUAL(decodeText(node2, messageBits), "LR");
    messageBits = {0,0, 1, 0, 1};
    EXPECT_EQUAL(decodeText(node2, messageBits), "RRLRL");

    deallocateTree(node2);
}

STUDENT_TEST("decodeText, test for more small examples") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = {}; // empty
    EXPECT_EQUAL(decodeText(tree, messageBits), "");

    messageBits = { 0 }; // T
    EXPECT_EQUAL(decodeText(tree, messageBits), "T");
    messageBits = { 1, 0, 0 }; // R
    EXPECT_EQUAL(decodeText(tree, messageBits), "R");
    messageBits = { 1, 0, 1}; // S
    EXPECT_EQUAL(decodeText(tree, messageBits), "S");

    messageBits = { 1, 0, 1, 1, 0, 1, 1, 0, 1}; // SSS
    EXPECT_EQUAL(decodeText(tree, messageBits), "SSS");

    messageBits = { 1, 0, 1, 1, 1}; // SE
    EXPECT_EQUAL(decodeText(tree, messageBits), "SE");

    messageBits = { 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0}; // RSTESTRERTT
    EXPECT_EQUAL(decodeText(tree, messageBits), "RSTESTRERTT");

    deallocateTree(tree);

}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

STUDENT_TEST("unflattenTree, test for empty case, one-node case") {
    Queue<Bit>  treeShape  = {};
    Queue<char> treeLeaves = {};
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree, nullptr));

    treeShape  = { 0 };
    treeLeaves = {'T'};
    EncodingTreeNode* reference = new EncodingTreeNode('T');
    tree = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree, reference));
    deallocateTree(reference);
    deallocateTree(tree);

    EncodingTreeNode* nodeT = new EncodingTreeNode('T');
    EncodingTreeNode* nodeR = new EncodingTreeNode('R');
    EncodingTreeNode* layer = new EncodingTreeNode(nodeT, nodeR);
    treeShape  = { 1, 0, 0 };
    treeLeaves = { 'T', 'R'};
    tree = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree, layer));

    deallocateTree(layer);
    deallocateTree(tree);
}


PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

STUDENT_TEST("decompress, more tests") {
    EncodedData data = {
        { 0 }, // treeShape
        { 'F' },  // treeLeaves
        { 0,0,0 } // messageBits
    };
    EXPECT_EQUAL(decompress(data), "FFF");

    data = {
        { 1, 1, 0, 1, 0, 0, 1, 0, 0 }, // treeShape
        { 'F', 'L', 'E', 'R', 'A' },  // treeLeaves
        { 0,0, 0,1,0, 0,1,1, 1,0, 1,1} // messageBits
    };
    EXPECT_EQUAL(decompress(data), "FLERA");

    data = {
        { 1, 1, 0, 1, 0, 0, 1, 0, 0 }, // treeShape
        { 'F', 'L', 'E', 'R', 'A' },  // treeLeaves
        { 1,1, 1,1, 0,1,1, 0,1,0, 1,0, 0,0, 0,0, 1,0, 0,1,0, 0,0} // messageBits
    };
    EXPECT_EQUAL(decompress(data), "AAELRFFRLF");
}


PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
