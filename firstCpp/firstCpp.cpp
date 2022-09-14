#include <iostream>
#include "console.h"
#include "simpio.h"
#include <string>
using namespace std;

int main()
{
    string userName = getLine("Please input your name: ");
    //string hiThere = string("hi") + "there";
    cout << "Hello, " + userName;
    return 0;
}
