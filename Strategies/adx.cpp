#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>

using namespace std;

double get_ewm(vector<pair<string, double>> data, int i, int n, double alpha, string indicator, map<pair<int, string>, double> memo_ewm)
{
    if (i == 0)
    {
        return data[i].second;
    }
    pair<int, string> key = {i, indicator};
    if (memo_ewm.find(key) != memo_ewm.end())
    {
        return memo_ewm[key];
    }

    double prev_ewm{get_ewm(data, i - 1, n, alpha, indicator, memo_ewm)};
    double ewm{(alpha * (data[i].second - prev_ewm)) + prev_ewm};

    memo_ewm[key] = ewm;
    return ewm;
}

int main(int argv, char *argc[])
{
    string symbol{argc[1]};
    int x{stoi(argc[2])};
    int n{stoi(argc[3])};
    double adx_threshold{stod(argc[4])};

    vector<pair<string, vector<double>>> data;

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
        string close;
        string high;
        string low;
        string prevClose;
        stringstream ss(line);
        getline(ss, date, ',');
        getline(ss, close, ',');
        getline(ss, high, ',');
        getline(ss, low, ',');
        getline(ss, prevClose, ',');
        data.push_back({date, {stod(close), stod(high), stod(low), stod(prevClose)}});
    }
    file.close();

    ofstream cash_file("daily_cashflow.csv");
    ofstream order_file("order_statistics.csv");
    ofstream final_file("final_pnl.txt");

    cash_file << "Date,Cashflow\n"
              << data[0].first << ",0\n";
    order_file << "Date,Order_dir,Quantity,Price\n";

    long unsigned int len{data.size()};
    double close{}, high{}, prev_high{}, low{}, prev_low{}, prevClose{};
    double tr{}, dm_p{}, dm_m{}, atr{}, di_p{}, di_m{}, dx{}, adx{};
    vector<pair<string, double>> tr_line, dm_p_line, dm_m_line, dx_line;
    map<pair<int, string>, double> memo{};
    int stocks{};
    double cashflow{};

    for (int i = 1; i < len; i++)
    {
        // defining names for current stock value
        close = data[i].second[0];
        high = data[i].second[1];
        prev_high = data[i - 1].second[1];
        low = data[i].second[2];
        prev_low = data[i - 1].second[2];
        prevClose = data[i].second[3];

        // calculating indicator variables
        // phase 1
        tr = max(abs(high - low), max(abs(high - prevClose), abs(low - prevClose)));
        dm_p = max(0.0, high - prev_high);
        dm_m = max(0.0, prev_low - low);
        tr_line.push_back({data[i].first, tr});

        atr = get_ewm(tr_line, i, n, (double)2.0 / (1 + n), "atr", memo);
        // phase 2
        dm_p_line.push_back({data[i].first, (dm_p / atr)});
        dm_m_line.push_back({data[i].first, (dm_m / atr)});

        di_p = get_ewm(dm_p_line, i, n, (2.0 / (1 + n)), "di+", memo);
        di_m = get_ewm(dm_m_line, i, n, (2.0 / (1 + n)), "di-", memo);

        // phase 3
        dx = ((di_p - di_m) / (di_p + di_m)) * 100;
        dx_line.push_back({data[i].first, dx});

        adx = get_ewm(dx_line, i, n, (2.0 / (1 + n)), "adx", memo);
        if (adx > adx_threshold and stocks < x)
        {
            stocks++;
            order_file << data[i].first << ",BUY,1," << data[i].second[0] << "\n";
            cashflow -= data[i].second[0];
        }
        else if (adx < adx_threshold and stocks > -x)
        {
            stocks--;
            order_file << data[i].first << ",SELL,1," << data[i].second[0] << "\n";
            cashflow += data[i].second[0];
        }

        cash_file << data[i].first << "," << cashflow << "\n";
    }

    double final_pnl{cashflow + (stocks * data[len - 1].second[0])};
    final_file << final_pnl;

    cash_file.close();
    order_file.close();
    final_file.close();

    return 0;
}
