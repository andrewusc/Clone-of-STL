
#include <iostream>
#include "Utility.h"
using namespace std;

int main(int argc, const char * argv[]) {
    pair<int, int> p1(2,5);
    cout << p1.first << " " << p1.second << endl;
    pair<int, int> p2 = p1;
    cout << p2.first << " " << p2.second << endl;
    pair<int, int> p3(p1);
    cout << p3.first << " " << p3.second << endl;
    cout << (p1 == p3) <<endl;
    return 0;
}
