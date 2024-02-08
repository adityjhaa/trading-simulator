#include<iostream>
#include<sstream>
#include<fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argv, char *argc[]){
    string symbol = argc[1];
    int n0 = stoi(argc[2]);
    int x = stoi(argc[3]);

    cout<<symbol<<endl<<n0<<" "<<x<<endl;

    return 0;    
}