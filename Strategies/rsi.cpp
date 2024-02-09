#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argv, char *argc[])
{
    string symbol{argc[1]};
    int x{stoi(argc[2])};
    int n{stoi(argc[3])};
    double oversold_threshold{stod(argc[4])};
    double overbought_threshold{stod(argc[5])};

    return 0;
}
