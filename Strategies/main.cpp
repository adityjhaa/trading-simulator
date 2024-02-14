#include <iostream>

using namespace std;

int main(int argv, char *argc[])
{

    string strategy = argc[1];
    string symbol = argc[2];
    string n = argc[3];
    string x = argc[4];
    string start_date = argc[5];
    string end_date = argc[6];
    string p = argc[7];
    string max_hold_days = argc[8];
    string c1 = argc[9];
    string c2 = argc[10];
    string oversold_threshold = argc[11];
    string overbought_threshold = argc[12];
    string adx_threshold = argc[13];
    string train_start_date = argc[14];
    string train_end_date = argc[15];
    string symbol1 = argc[16];
    string symbol2 = argc[17];
    string threshold = argc[18];
    string stop_loss_threshold = argc[19];

    string command = "";

    // --------------- generate data -------------------------- //
    if (strategy == "PAIRS")
    {
        command = "python3 main.py " + symbol1 + " " + start_date + " " + end_date + " 0";
        system(command.c_str());
        command = "python3 main.py " + symbol2 + " " + start_date + " " + end_date + " 0";
        system(command.c_str());
    }
    else if (strategy == "LINEAR_REGRESSION")
    {
        command = "python3 main.py " + symbol + " " + train_start_date + " " + train_end_date + " 1";
        system(command.c_str());
        command = "python3 main.py " + symbol + " " + start_date + " " + end_date + " 0";
        system(command.c_str());
    }
    else
    {
        command = "python3 main.py " + symbol + " " + start_date + " " + end_date + " 0";
        system(command.c_str());
    }

    // ---------------- case based commands ------------------ //
    if (strategy == "BASIC")
    {
        system("g++ basic.cpp -o basic.out");
        command = "./basic.out " + symbol + " " + n + " " + x;
        system(command.c_str());
    }
    else if (strategy == "DMA")
    {
        system("g++ dma.cpp -o dma.out");
        command = "./dma.out " + symbol + " " + n + " " + x + " " + p;
        system(command.c_str());
    }
    else if (strategy == "DMA++")
    {
        system("g++ dmapp.cpp -o dmapp.out");
        command = "./dmapp.out " + symbol + " " + n + " " + x + " " + p + " " + max_hold_days + " " + c1 + " " + c2;
        system(command.c_str());
    }
    else if (strategy == "MACD")
    {
        system("g++ macd.cpp -o macd.out");
        command = "./macd.out " + symbol + " " + x;
        system(command.c_str());
    }
    else if (strategy == "RSI")
    {
        system(("python3 help.py " + symbol + " " + start_date).c_str());
        system("g++ rsi.cpp -o rsi.out");
        command = "./rsi.out " + symbol + " " + x + " " + n + " " + oversold_threshold + " " + overbought_threshold;
        system(command.c_str());
    }
    else if (strategy == "ADX")
    {
        system("g++ adx.cpp -o adx.out");
        command = "./adx.out " + symbol + " " + x + " " + n + " " + adx_threshold;
        system(command.c_str());
    }
    else if (strategy == "LINEAR_REGRESSION")
    {
        system("g++ linregr.cpp -o linregr.out");
        command = "./linregr.out " + symbol + " " + x + " " + p + " " + train_start_date + " " + train_end_date;
        system(command.c_str());
    }
    else if (strategy == "BEST_OF_ALL")
    {
        system("g++ bestall.cpp -o bestall.out");
        command = "./bestall.out " + symbol + " " + start_date + " " + end_date;
        system(command.c_str());
    }
    else if (strategy == "PAIRS")
    {
        system(("python3 help2.py " + symbol1 + " " + symbol2 + " " + start_date + " " + n).c_str());
        system("g++ pairs.cpp -o pairs.out");
        command = "./pairs.out " + symbol1 + " " + symbol2 + " " + x + " " + n + " " + threshold + " " + stop_loss_threshold;
        system(command.c_str());
    }

    return 0;
}
