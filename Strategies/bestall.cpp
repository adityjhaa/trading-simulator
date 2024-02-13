#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>

using namespace std;

void generate_data(string strategy, string symbol, string start_date, string end_date, string x, string train_start_date, string train_end_date)
{
    string command = "";
    // --------------- generate data -------------------------- //
    if (strategy == "LINEAR_REGRESSION")
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
        command = "./basic.out " + symbol + " " + "7" + " " + x;
        system(command.c_str());
    }
    else if (strategy == "DMA")
    {
        system("g++ dma.cpp -o dma.out");
        command = "./dma.out " + symbol + " " + "50" + " " + x + " " + "2";
        system(command.c_str());
    }
    else if (strategy == "DMA++")
    {
        system("g++ dmapp.cpp -o dmapp.out");
        command = "./dmapp.out " + symbol + " " + "14" + " " + x + " " + "5" + " " + "28" + " " + "2" + " " + "0.2";
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
        system("g++ rsi.cpp -o rsi.out");
        command = "./rsi.out " + symbol + " " + x + " " + "14" + " " + "30" + " " + "70";
        system(command.c_str());
    }
    else if (strategy == "ADX")
    {
        system("g++ adx.cpp -o adx.out");
        command = "./adx.out " + symbol + " " + x + " " + "14" + " " + "25";
        system(command.c_str());
    }
    else if (strategy == "LINEAR_REGRESSION")
    {
        system("g++ linregr.cpp -o linregr.out");
        command = "./linregr.out " + symbol + " " + x + " " + "2" + " " + train_start_date + " " + train_end_date;
        system(command.c_str());
    }
}

int main(int argv, char *argc[])
{

    vector<pair<string, double>> strategies = {{"BASIC", 0}, {"DMA", 0}, {"DMA++", 0}, {"MACD", 0}, {"RSI", 0}, {"ADX", 0}, {"LINEAR_REGRESSION", 0}};
    string symbol = argc[1];
    string start_date = argc[2];
    string end_date = argc[3];
    string x = "5";

    string year = start_date.substr(6, 4);
    string train_year = to_string(stoi(year) - 1);
    string train_start_date = start_date.substr(0, 6) + train_year;
    string train_end_date = end_date.substr(0, 6) + train_year;

#pragma omp parallel for
    for (int i = 0; i < strategies.size(); i++)
    {
        generate_data(strategies[i].first, symbol, start_date, end_date, x, train_start_date, train_end_date);

        ifstream file("results/final_pnl.txt");
        if (!file.is_open())
        {
            cerr << "Error opening file." << endl;
            return 1;
        }

        string outcome;
        getline(file, outcome);
        double symbol_pnl = stod(outcome.substr(11));
        strategies[i].second = symbol_pnl;
    }
    
    int index{};
    for (int i = 1; i < strategies.size(); i++)
    {
        if (strategies[i].second > strategies[i - 1].second)
            index = i;
    }

    generate_data(strategies[index].first, symbol, start_date, end_date, x, train_start_date, train_end_date);

    return 0;
}