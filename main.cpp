//#include <cctype>
#include <iostream>
#include <string>
#include "StanfordCPPLib/map.h"
using namespace std;

int main(){
    Map<char, char> myMap;
    char ch = 'A';
    for (int i = 0; i < 20; ++i) {
        myMap.put(i,ch + i);
    }
    for (auto it = myMap.begin(); it != myMap.end(); ++it) {
        cout << it.operator->() << ' ';
    }
}
