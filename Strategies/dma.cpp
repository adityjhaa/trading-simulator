#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

int main(int argv, char *argc[])
{
    string symbol{argc[1]};
    int n0{stoi(argc[2])};
    int x{stoi(argc[3])};
    int p{stoi(argc[4])};

    vector<pair<string, double>> data;

    ifstream file(symbol + ".csv");
    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return 1;
    }

    string first_line;
    getline(file, first_line);

    string line;
    while (getline(file, line))
    {
        string date;
        string price;
        stringstream ss(line);
        getline(ss, date, ',');
        getline(ss, price, ',');
        data.push_back({date, stod(price)});
    }

    file.close();

    ofstream cash_file("daily_cashflow.csv");
    ofstream order_file("order_statistics.csv");
    ofstream final_file("final_pnl.txt");

    cash_file << "Date,Cashflow\n";
    order_file << "Date,Order_dir,Quantity,Price\n";

    long unsigned int len{data.size()};
    double sma{}, sd{}, variance{}, sumOfSquares{}, sum{};
    int stocks{};
    double cashflow{};

    for (int i = 0; i < len; i++)
    {
        // the first n0 days we have no DMA so we can continue
        int windowSize = min(i + 1, n0);
        sumOfSquares += (data[i].second * data[i].second);
        sum += (data[i].second);
        if (i >= n0)
        {
            sum -= data[i-n0].second;
            sumOfSquares -= data[i-n0].second*data[i-n0].second;
        }
        sma = sum / windowSize;
        variance = (sumOfSquares - (sum * sum) / windowSize) / (windowSize - 1);

        // getting the first value of sma and sd
        sd = sqrt(variance);

        // implementing buy and sell
        if ((data[i].second > sma + (p * sd)) and stocks < x)
        {
            // buy
            stocks++;
            order_file << data[i].first << ",BUY,1," << data[i].second << "\n";
            cashflow -= data[i].second;
        }
        else if ((data[i].second < sma - (p * sd)) and stocks > -x)
        {
            // sell
            stocks--;
            order_file << data[i].first << ",SELL,1," << data[i].second << "\n";
            cashflow += data[i].second;
        }

        // cash_file
        cash_file << data[i].first << "," << cashflow << "\n";
    }

    double final_pnl{cashflow + (stocks * data[len - 1].second)};
    final_file << final_pnl ;

    cash_file.close();
    order_file.close();
    final_file.close();

    return 0;
}