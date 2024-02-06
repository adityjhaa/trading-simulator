#include <iostream>

using namespace std;

int main(int argv, char *argc[])
{
    string strategy = argc[1];

    string str = "";

    if (strategy == "BASIC")
    {
        str = "basic";
    }
    else if (strategy == "DMA")
    {
        str = "dma";
    }
    else if (strategy == "DMA++")
    {
        str = "dmapp";
    }
    else if (strategy == "MACD")
    {
        str = "macd";
    }
    else if (strategy == "RSI")
    {
        str = "rsi";
    }
    else if (strategy == "ADX")
    {
        str = "adx";
    }
    else if (strategy == "LINEAR_REGRESSION")
    {
        str = "linregr";
    }
    else if (strategy == "BEST_OF_ALL")
    {
        str = "bestall";
    }
    else if (strategy == "PAIRS")
    {
        str = "pairs";
    }
    else
    {
        return 0;
    }

    string command1 = "g++ " + str + ".cpp -o " + str + ".out";
    string command2 = "./" + str + ".out ";
    system(command1.c_str());
    system(command2.c_str());
    return 0;
}