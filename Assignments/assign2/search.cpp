// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: 将输入字符串进行处理，去除首尾标点、转换为小写
string cleanToken(string s)
{
    //如果整个字符串没有一个字母，则返回一个空字符串
    int i = 0;
    for (char element : s) {
        if (isalpha(element)) {
            break;
        }
        i++;
        if (i == s.size() - 1)
        {
            return string{};
        }
    }
    //去除字符串首尾的标点符号
    while (ispunct(s.front())) {
        s.erase(0, 1);
    }
    while (ispunct(s.back())) {
        s.pop_back();
    }
    //将字符串转换为小写
    for (int i = 0; i < s.size(); i++) {
        if (isalpha(s[i])) {
            s[i] = toLowerCase(s[i]);
        }
    }
    return s;
}

// TODO: 从正文文本中提取出所有不重复的字符串，并以集合的形式返回
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    //调用stringSplit函数对text文本进行分割，返回一个vector
    Vector<string> splitText = stringSplit(text, " ");

    for (const string& str : splitText) {
        string token = cleanToken(str);
        if (token.size() != 0) {
            tokens.add(token);
        }
    }
    return tokens;
}

// TODO:  从数据库文件读取内容，并将其处理为反向索引的形式
//输入参数1--网络数据库文件名字的字符串，输入参数2--生成反向索引数据的Map;输出参数--处理过的网页的数量
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    //读取数据库文件内容，按行读取
    ifstream in;
    if (!openFile(in, dbfile)) {
        error("Cannot open file named" + dbfile);
    }
    Vector<string> lines;
    readEntireFile(in, lines);

    //将每一行的网页内容处理成独一无二的字符串-gatherTokens，然后添加对应的URL到映射中
    int nURL = 0;
    //先遍历每一行网页文本-奇数行，通过gatherTokens函数处理得到字符串集合
    for (int i = 1; i < lines.size(); i = i +2) {
        Set<string> tokensSet = gatherTokens(lines[i]);
        //遍历该字符串集合，生成映射index，每一个字符串作为key,该行对应网页URL作为value-set的其中一个元素；
        for (const string& element : tokensSet) {
            index[element].add(lines[i - 1]);
        }
        nURL++;
    }
    return nURL;
}

// TODO: 根据内容搜索网页
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    //分割字符串
    Vector<string> splitQuery = stringSplit(query, " ");

    //如果query只有一个关键词
    if (splitQuery.size() == 1) {
        string word = cleanToken(splitQuery[0]);
        result = index.get(word);
    }
    //如果query有多个关键词
    else {
        for (const string& element: splitQuery) {
            string word = cleanToken(element);
            //如果没有连接符号，则加入该关键词对应的set，并集
            if (isalpha(element[0])) {
                result += index.get(word);
            }
            //如果连接符为+，则取两个集合set的交集
            else if (element[0] == '+') {
                result.intersect(index.get(word));
            }
            //如果连接符为-，则取两个集合的差集
            else if (element[0] == '-') {
                result.difference(index.get(word));
            }
        }
    }
    return result;
}

// TODO: 构建小型搜索引擎，输入参数为数据库文件；
//最终实现根据用户提供的关键词，打印匹配页面的URLs
void searchEngine(string dbfile)
{
    // - 根据数据库文件内容，构建一个反向索引index
    Map<string, Set<string>> index;
    cout << "Stand by while building index..." << endl;
    int nPages = buildIndex(dbfile, index);
    // - 打印出构建反向索引用到的网页数量以及所有页面中包含了多少独一无二的单词
    cout << "Indexed " << nPages << "pages containing " << index.size() << "unique terms";

    // - 进入一个循环，提示用户输入查询关键词
    while (true) {
        Set<string> pages;
        string query = getLine("Enter query sentence (Return/Enter to quit: ");
        // - 当用户输入空字符串时，提示查询完成，程序结束。
        if (query.size() == 0) {
            break;
        }
        // - 对于每一个查询关键词，找出所有匹配页面并打印出 URLs
        pages = findQueryMatches(index, query);
        cout << "Found " << pages.size() << "matching pages" << endl;
        cout << pages << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


 //TODO: add your test cases here
STUDENT_TEST("~~~cleanToken: no punctuation at beginning or end, but one char and long strings") {
    EXPECT_EQUAL(cleanToken("Luo0*Fqqq"), "luo0*fqqq");
    EXPECT_EQUAL(cleanToken("A"), "a");
    EXPECT_EQUAL(cleanToken("helloWorldluofffqqqfffqqqfffqqqfffqqqfffqqq"), "helloworldluofffqqqfffqqqfffqqqfffqqqfffqqq");
}

STUDENT_TEST("~~~cleanToken on strings with some punctuations at beginning and end") {
    EXPECT_EQUAL(cleanToken("//hel0lo/*"), "hel0lo");
    EXPECT_EQUAL(cleanToken("~~~~~woRLD!!!!!!"), "world");
    EXPECT_EQUAL(cleanToken("~~~~~LUOFQ"), "luofq");
}

STUDENT_TEST("~~~cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("~~~111"), "");
    EXPECT_EQUAL(cleanToken("~!C!!!"), "c");
    EXPECT_EQUAL(cleanToken("~!CS106B!!!"), "cs106b");
}

STUDENT_TEST("~~~gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "learn", "lea2rn", "stickmind"};
    EXPECT_EQUAL(gatherTokens("~I~ _learn_ Stickmind!!!! ~~lea2rn********* i i~ learn lea2rn"), expected);
}

STUDENT_TEST("~~~gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("ValueType& operator[] (const KeyType& key) ValueType operator[] (const KeyType& key) const;");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("valuetype"));
    EXPECT(!tokens.contains("(const"));
    Set<string> expected = {"valuetype", "operator", "const", "keytype", "key"};
    EXPECT_EQUAL(tokens, expected);
}

STUDENT_TEST("buildIndex from website.txt, 32 pages") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT_EQUAL(index.size(), 3884);
    EXPECT(!index.containsKey("fish"));
}

STUDENT_TEST("~~~findQueryMatches from tiny_website.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny_website.txt", index);
    Set<string> matches1 = findQueryMatches(index, "programming");
    EXPECT_EQUAL(matches1.size(), 2);
    EXPECT(matches1.contains("http://cs106b.stanford.edu/class/cs106b/about_assignments"));
    Set<string> matches2 = findQueryMatches(index, "hippo");
    EXPECT(matches2.isEmpty());
}

STUDENT_TEST("~~~findQueryMatches from tiny_website.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny_website.txt", index);
    Set<string> matchesOr = findQueryMatches(index, "coding Fridays");
    EXPECT_EQUAL(matchesOr.size(), 2);

    Set<string> matchesAnd = findQueryMatches(index, "we +exams");
    EXPECT_EQUAL(matchesAnd.size(), 2);
    EXPECT(matchesAnd.contains("http://cs106b.stanford.edu/class/cs106b/about_assignments"));
    EXPECT(matchesAnd.contains("http://cs106b.stanford.edu/class/cs106b/about_assessments"));

    Set<string> matchesWithout = findQueryMatches(index, "we -scheduling");
    EXPECT_EQUAL(matchesWithout.size(), 2);
    EXPECT(matchesWithout.contains("http://cs106b.stanford.edu/class/cs106b/about_lectures"));
    EXPECT(!matchesWithout.contains("http://cs106b.stanford.edu/class/cs106b/about_assessments"));
}
