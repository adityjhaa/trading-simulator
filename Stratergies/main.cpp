#include <iostream>

using namespace std;

int main(int argv, char *argc[])
{
    string stratergy = argc[1];

    string str = "";

    if (stratergy == "BASIC")
    {
        str = "basic";
    }
    else if (stratergy == "DMA")
    {
        str = "dma";
    }
    else if (stratergy == "DMA++")
    {
        str = "dmapp";
    }
    else if (stratergy == "MACD")
    {
        str = "macd";
    }
    else if (stratergy == "RSI")
    {
        str = "rsi";
    }
    else if (stratergy == "ADX")
    {
        str = "adx";
    }
    else if (stratergy == "LINEAR_REGRESSION")
    {
        str = "linregr";
    }
    else if (stratergy == "BEST_OF_ALL")
    {
        str = "bestall";
    }
    else if (stratergy == "PAIRS")
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