#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <tuple>
#include <string>
using namespace std;

map<tuple<int, int>, double> memo_ewm;

double get_ewm(vector<pair<string, double>> data, int i, int n, double alpha);

int main(int argv, char *argc[])
{
    string symbol{argc[1]};
    int x{stoi(argc[2])};

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
        getline(ss, price, '\n');
        data.push_back({date, stod(price)});
    }

    file.close();

    ofstream cash_file("daily_cashflow.csv");
    ofstream order_file("order_statistics.csv");
    ofstream final_file("final_pnl.txt");

    cash_file << "Date,Cashflow\n";
    order_file << "Date,Order_dir,Quantity,Price\n";

    vector<pair<string, double>> macd_line;
    long unsigned int len{data.size()};
    double long_ewm{}, short_ewm{}, signal{}, macd{};
    int long_term{26}, short_term{12}, signal_term{9};
    int stocks{}, prevStocks{};
    double cashflow{};

    for (int i = 0; i < len; i++)
    {
        // macd implementations
        long_ewm = get_ewm(data, i, long_term, (double)2.0/(1+long_term));
        short_ewm = get_ewm(data, i, short_term, (double)2.0/(1+short_term));
        macd = short_ewm - long_ewm;
        macd_line.push_back({data[i].first, macd});
        signal = get_ewm(macd_line, i, signal_term, (double)2.0/(1+signal_term));

        // implementing buy and sell
        if ((macd > signal) and stocks < x)
        {
            // buy
            stocks++;
            order_file << data[i].first << ",BUY,1," << data[i].second << "\n";
            cashflow -= data[i].second;
        }
        else if ((macd < signal) and stocks > -x)
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
    final_file << final_pnl << "\n";

    cash_file.close();
    order_file.close();
    final_file.close();

    return 0;
}

double get_ewm(vector<pair<string, double>> data, int i, int n, double alpha)
{
    if (i == 0)
    {
        return data[0].second;
    }

    auto key{make_tuple(i, n)};
    if (memo_ewm.find(key) != memo_ewm.end())
    {
        return memo_ewm[key];
    }

    double prev_ewm{get_ewm(data, i - 1, n, alpha)};
    double ewm{(alpha * (data[i].second - prev_ewm)) + prev_ewm};

    memo_ewm[key] = ewm;
    return ewm;
}
