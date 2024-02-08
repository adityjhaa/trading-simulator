#include <iostream>

using namespace std;

int main(int argv, char *argc[])
{
    string cc = "g++ ";
    string strategy = argc[1];
    string symbol = argc[2];
    string n = argc[3];
    string x = argc[4];
    string start_date = argc[5];
    string end_date = argc[6];
    
    // --------------- generate data --------------------------
    string command0 = "python3 main.py " + symbol + " " + start_date + " " + end_date;
    system(command0.c_str());

    if (strategy == "BASIC")
    {
        system("g++ basic.cpp -o basic.out");
        string command = "./basic.out " + symbol +" " + n + " " + x;
        system(command.c_str());
    }
    else if (strategy == "DMA")
    {
        system("g++ dma.cpp -o dma.out");
        string command = "./dma.out ";
        system(command.c_str());
    }
    else if (strategy == "DMA++")
    {
        system("g++ dmapp.cpp -o dmapp.out");
        string command = "./dmapp.out ";
        system(command.c_str());
    }
    else if (strategy == "MACD")
    {
        system("g++ macd.cpp -o macd.out");
        string command = "./macd.out ";
        system(command.c_str());
    }
    else if (strategy == "RSI")
    {
        system("g++ rsi.cpp -o rsi.out");
        string command = "./rsi.out ";
        system(command.c_str());
    }
    else if (strategy == "ADX")
    {
        system("g++ adx.cpp -o adx.out");
        string command = "./adx.out ";
        system(command.c_str());
    }
    else if (strategy == "LINEAR_REGRESSION")
    {
        system("g++ linregr.cpp -o linregr.out");
        string command = "./linregr.out ";
        system(command.c_str());
    }
    else if (strategy == "BEST_OF_ALL")
    {
        system("g++ bestall.cpp -o bestall.out");
        string command = "./bestall.out ";
        system(command.c_str());
    }
    else if (strategy == "PAIRS")
    {
        system("g++ pairs.cpp -o pairs.out");
        string command = "./pairs.out ";
        system(command.c_str());        
    }
    
    return 0;
}