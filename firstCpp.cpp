#include <iostream>
#include <string>
using namespace std;

int main()
{
    //string name = getline("Enter your name: ");
    string name;
    getline(cin, name);
    cout << "Hello, " + name << endl;
    return 0;
}
