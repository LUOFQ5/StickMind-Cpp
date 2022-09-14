#include "console.h"
#include "simpio.h"
using namespace std;
const int STOREY = 6; //default 6 layers
void printPyramid0(int storey);
void printDiamond(int storey);
void printPyramid(int storey);
void printTriangle(int storey);
void printTriangle2(int storey);

int main()
{

    int program = getInteger("Select your program(0, 1, 2, 3, 4): ");
    int layer = 0;
    switch (program) {
    case 0:
        layer = getInteger("Enter your own storey: ");
        printPyramid0(layer ? layer : STOREY);
        break;
    case 1:
        layer = getInteger("Enter your own storey: ");
        printDiamond(layer ? layer : STOREY);
        break;
    case 2:
        layer = getInteger("Enter your own storey: ");
        printPyramid(layer ? layer : STOREY);
        break;
    case 3:
        layer = getInteger("Enter your own storey: ");
        printTriangle(layer ? layer : STOREY);
        break;
    case 4:
        layer = getInteger("Enter your own storey: ");
        printTriangle2(layer ? layer : STOREY);
        break;
    default:
        cout << "Sorry, you have to select the given program." << endl;
        break;
    }
    return 0;
}

void printPyramid0(int storey)
{
    for(int i = 1-storey; i<= 0; i++)
    {
        for(int j = 1-storey; j< storey; j++ )
        {
            int sum = abs(i) + abs(j);
            if((sum < storey) && (sum%2 == 1))
            {
                cout << "* ";
            }
            else
                cout << "  ";
        }
        cout << endl;
    }
}

void printDiamond(int storey)
{
    for(int i = 1-storey; i < storey; i++){
        for(int j = 1-storey; j < storey; j++){
            int sum = abs(i) + abs(j);
            if (sum < storey){
                cout << "*";
            }
            else
                cout << " ";
        }
        cout << endl;
    }
}

void printPyramid(int storey)
{
    for(int i = 1-storey; i<= 0; i++)
    {
        for(int j = 1-storey; j< storey; j++ )
        {
            if(abs(i)+abs(j) < storey)
            {
                cout << "*";
            }
            else
                cout << " ";
        }
        cout << endl;
    }
}

void printTriangle(int storey)
{
    for(int i = 1-storey; i <= 0; i++){
        for(int j = 1-storey; j < storey; j++){
            int sum = abs(i) + abs(j);
            if ((sum == storey-1 ) || ((i == 0) && (abs(j)%2 == 1))){
                cout << "*";
            }
            else
                cout << " ";
        }
        cout << endl;
    }
}

void printTriangle2(int storey)
{
    for(int i = 1-storey; i <= 0; i++){
        for(int j = 1-storey; j < storey; j++){
            int sum = abs(i) + abs(j);
            if ((sum == storey-1 ) || ((i == 0) && (abs(j)%2 == 1))){
                cout << storey- abs(i);
            }
            else
                cout << " ";
        }
        cout << endl;
    }
}
