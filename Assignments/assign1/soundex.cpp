/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 * 默认提供的代码无法避免这个清空：首字符非字母；
 */
string removeNonLetters(string s) {
    //由于string是一种基于类的数据结构，自带默认构造函数，因此可以只声明不初始化；
    //string result = charToString(s[0]);
    string result;
    //从第一个字符开始，只将字母录入到result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}


/* TODO:
 * 根据规定的编码系统，将字母转换成数字0-6
 * 这种写法太冗长了。。。。。
 */
int charToDigit(char cc)
{
    int digit = -1;

    if (toupper(cc) == 'A' || toupper(cc) == 'E' || toupper(cc) == 'I' || toupper(cc) == 'O' || toupper(cc) == 'U' || toupper(cc) == 'H' || toupper(cc) == 'W' || toupper(cc) == 'Y')
    {
        digit = 0;
    }
    else if (toupper(cc) == 'B' || toupper(cc) == 'F' || toupper(cc) == 'P' || toupper(cc) == 'V')
    {
        digit = 1;
    }
    else if (toupper(cc) == 'C' || toupper(cc) == 'G' || toupper(cc) == 'J' || toupper(cc) == 'K' || toupper(cc) == 'Q' || toupper(cc) == 'S' || toupper(cc) == 'X' || toupper(cc) == 'Z')
    {
        digit = 2;
    }
    else if (toupper(cc) == 'D' || toupper(cc) == 'T')
    {
        digit = 3;
    }
    else if (toupper(cc) == 'L')
    {
        digit = 4;
    }
    else if (toupper(cc) == 'M' || toupper(cc) == 'N')
    {
        digit = 5;
    }
    else if (toupper(cc) == 'R')
    {
        digit = 6;
    }

    return digit;
}

//通过soundex转化算法，将输入的字符串转化为特定的数据编码；
string soundex(string s) {
    // TODO:
    //去除字符串中的非字母元素
    s = removeNonLetters(s);

    //将首字母转换成大写，并记录下来
    char S0 = toUpperCase(s)[0];

    //根据编码规则、将字母转换成数字，数字依然作为string进行存储
    string soundexName;
    for (int i = 0; i < s.length(); ++i)
    {
        soundexName += integerToString(charToDigit(s[i]));
    }
    //去除重复数字中的第二个数字；
    //定义一个迭代器来实现erase功能
    string::iterator it;
    for (it = soundexName.begin(); it != soundexName.end(); it++)
    {
        while (it + 1 != soundexName.end())
        {
            if (*it == *(it + 1))
            {
                soundexName.erase(it + 1);
            }
            else
            {
                break;
            }
        }
    }
    //将首个数字转换回大写字母
    soundexName[0] = S0;
    //去除所有0数字，采用erase函数实现去除指定值的字符
    soundexName.erase(remove(soundexName.begin(),soundexName.end(),'0'),soundexName.end());

    //如果字符串的长度小于4，则在末尾加0
    while (soundexName.length() < 4)
    {
        soundexName.push_back('0');
    }
    //如果字符串的长度大于4，则删除末尾的字符
    while (soundexName.length() > 4)
    {
        soundexName.erase(soundexName.end() - 1);
    }

    return soundexName;
}


/* TODO: 实现soundex检索程序：
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    //TODO: 将用户输入的字符串转换为编码，同时遍历一个数据库，将数据库中编码相同的都记录下来并打印
    //提示用户持续输入，直到得到空白输入则停止
    while (true)
    {
        string inputNames = getLine("Enter a surname (RETURN to quit): ");
        if (inputNames == "")
        {
            cout << "All done!" << endl;
            break;
        }
        //将输入的姓名转换为soundex编码
        string filterNames = removeNonLetters(inputNames);
        string soundexName = soundex(filterNames);
        cout << "Soundex code is" << soundexName << endl;
        //遍历数据库，将与前述遍历相等的姓名添加到一个vector中
        Vector<string> foundInDatabase;
        for (int i = 0; i < databaseNames.size(); i++)
        {
            if (soundex(databaseNames[i]) == soundexName)
            {
                foundInDatabase.add(databaseNames[i]);
            }
        }
        foundInDatabase.sort();
        cout << "Matches from database: {";
        for (auto v: foundInDatabase)
        {
            cout << "\"" << v << "\", ";
        }
        cout << "\"" << endl;
    }

}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("~~~~Test for removing puntuation, digits, and spaces") {
    string s = "  tl dr";
    string result = removeNonLetters("  tl dr");
    EXPECT_EQUAL(result, "tldr");
    s = "";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "1abc";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "abc");
}


STUDENT_TEST("~~~~Test for charToDigit") {
    char cc = 'a';
    int result = charToDigit(cc);
    EXPECT_EQUAL(result, 0);
    cc = 's';
    result = charToDigit(cc);
    EXPECT_EQUAL(result, 2);
}

STUDENT_TEST("~~~~Test for soundex function") {
//    string ss = "Ashcraft";
//    string result = soundex(ss);
//    EXPECT_EQUAL(result, "A226");
    //EXPECT_EQUAL(soundex("Jackson"), "J250");
    EXPECT_EQUAL(soundex("Schwarz"), "S620");

    string result = soundex("Tessier-Lavigne");
    EXPECT_EQUAL(result, "T264");
    string ss = "Curie";
    result = soundex(ss);
    EXPECT_EQUAL(result, "C600");
    ss = "O'Conner";
    result = soundex(ss);
    EXPECT_EQUAL(result, "O256");
    ss = "Angelou";
    result = soundex(ss);
    EXPECT_EQUAL(result, "A524");
}
