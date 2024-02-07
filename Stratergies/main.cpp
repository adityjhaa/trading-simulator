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

    // --------------- generate data --------------------------
    string symbol = argc[2];
    string start_date = argc[5];
    string end_date = argc[6];
    string command0 = "python3 main.py " + symbol + " " + start_date + " " + end_date;
    system(command0.c_str());

    string command1 = "g++ " + str + ".cpp -o " + str + ".out";
    string command2 = "./" + str + ".out ";
    system(command1.c_str());
    system(command2.c_str());
    return 0;
}