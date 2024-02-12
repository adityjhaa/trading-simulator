#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

int main(int argv, char *argc[])
{
    string symbol{argc[1]};
    int x{stoi(argc[2])};
    int n{stoi(argc[3])};
    double oversold_threshold{stod(argc[4])};
    double overbought_threshold{stod(argc[5])};

    vector<pair<string, double>> data;
    ifstream f("help.txt");
    string text;
    getline(f, text);
    double prev{stod(text)};
    f.close();
    data.push_back({"-1", prev});

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
    double rsi{}, rs{}, gain_n{}, loss_n{}, sum_loss_n{}, sum_gain_n{};
    int stocks{};
    double cashflow{};
    int windowSize = 14;

    for (int i = 1; i < len; i++)
    {
        sum_gain_n += max((data[i].second - data[i - 1].second), 0.0);
        sum_loss_n += max((data[i - 1].second - data[i].second), 0.0);
        if (i < n)
        {
            cash_file << data[i].first << "," << cashflow << "\n";
            continue;
        }
        if (i > n)
        {
            sum_gain_n -= max((data[i - n].second - data[i - 1 - n].second), 0.0);
            sum_loss_n -= max((data[i - n - 1].second - data[i - n].second), 0.0);
        }
        gain_n = sum_gain_n / windowSize;
        loss_n = sum_loss_n / windowSize;

        rs = gain_n / loss_n;
        rsi = 100.0 - (100.0 / (1.0 + rs));
        if (rsi <= oversold_threshold and stocks < x)
        {
            stocks++;
            order_file << data[i].first << ",BUY,1," << data[i].second << "\n";
            cashflow -= data[i].second;
        }
        else if (rsi >= overbought_threshold and stocks > -x)
        {
            stocks--;
            order_file << data[i].first << ",SELL,1," << data[i].second << "\n";
            cashflow += data[i].second;
        }

        cash_file << data[i].first << "," << cashflow << "\n";
    }

    double final_pnl{cashflow + (stocks * data[len - 1].second)};
    final_file << final_pnl;

    cash_file.close();
    order_file.close();
    final_file.close();

    return 0;
}
