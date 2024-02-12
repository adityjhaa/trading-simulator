#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <tuple>
#include <string>
using namespace std;

map<tuple<int, int, int, int>, double> memo_sf;
map<tuple<int, int, int, int>, double> memo_ama;

double get_er(vector<pair<string, double>> data, int i, int n0);

double get_sf(vector<pair<string, double>> data, int i, int n0, int c1, int c2);

double get_ama(vector<pair<string, double>> data, int i, int n0, int c1, int c2);

int main(int argv, char *argc[])
{
    string symbol{argc[1]};
    int n0{stoi(argc[2])};
    int x{stoi(argc[3])};
    int p{stoi(argc[4])};
    int max_hold_days{stoi(argc[5])};
    int c1{stoi(argc[6])};
    int c2{stoi(argc[7])};

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
    double ama{};
    int stocks{}, prevStocks{};
    double cashflow{};
    int hold_days{};

    for (int i = 0; i < len; i++)
    {
        // setting up prevStocks
        prevStocks = stocks;

        // data
        ama = get_ama(data, i, n0, c1, c2);


        // implementing buy and sell
        if ((data[i].second > ama + (p/100)) and stocks < x)
        {
            // buy
            stocks++;
            order_file << data[i].first << ",BUY,1," << data[i].second << "\n";
            cashflow -= data[i].second;
        }
        else if ((data[i].second < ama - (p/100)) and stocks > -x)
        {
            // sell
            stocks--;
            order_file << data[i].first << ",SELL,1," << data[i].second << "\n";
            cashflow += data[i].second;
        }

        // checking if stock has changed
        if (stocks == prevStocks)
        {
            hold_days += 1;
        }

        // checking if hold_days == max_hold_days
        if (hold_days == max_hold_days and stocks > 0)
        {
            // sell
            stocks--;
            order_file << data[i].first << ",SELL,1," << data[i].second << "\n";
            cashflow += data[i].second;

            // reset hold days
            hold_days = 0;
        }
        else if (hold_days == max_hold_days and stocks < 0)
        {
            // buy
            stocks++;
            order_file << data[i].first << ",BUY,1," << data[i].second << "\n";
            cashflow -= data[i].second;

            // reset hold days
            hold_days = 0;
        }

        // cash_file
        cash_file << data[i].first << "," << cashflow << "\n";
    }

    double final_pnl{cashflow + (stocks * data[len - 1].second)};
    final_file << final_pnl;

    cash_file.close();
    order_file.close();
    final_file.close();

    return 0;
}


double get_er(vector<pair<string, double>> data, int i, int n0)
{
    double curr_price{}, n_prev_price{}, abs_sum{};
    if (i < n0)
    {
        n_prev_price = data[0].second;
        curr_price = data[i].second;
        for (int j = 1; j <= i; j++)
        {
            abs_sum += abs(data[j].second - data[j - 1].second);
        }
    }
    else
    {
        n_prev_price = data[i - n0].second;
        curr_price = data[i].second;
        for (int j = i - n0; j <= i; j++)
        {
            abs_sum += abs(data[j].second - data[j - 1].second);
        }
    }

    double er{abs(curr_price - n_prev_price) / abs_sum};

    return er;
}

double get_sf(vector<pair<string, double>> data, int i, int n0, int c1, int c2)
{
    if (i == 0)
    {
        return 0.5;
    }

    auto key{make_tuple(i, n0, c1, c2)};
    if (memo_sf.find(key) != memo_sf.end())
    {
        return memo_sf[key];
    }

    double temp_val{((2 * get_er(data, i, n0)) / (1 + c2))};
    double numerator{temp_val - 1};
    double denominator{temp_val + 1};
    double factor{numerator / denominator};

    double prev_sf{get_sf(data, i - 1, n0, c1, c2)};

    double sf{prev_sf + (c1 * (factor - prev_sf))};

    memo_sf[key] = sf;

    return sf;
}

double get_ama(vector<pair<string, double>> data, int i, int n0, int c1, int c2)
{
    if (i == 0)
    {
        return data[0].second;
    }

    auto key{make_tuple(i, n0, c1, c2)};

    if (memo_ama.find(key) != memo_ama.end())
    {
        return memo_ama[key];
    }

    double sf_val{get_sf(data, i, n0, c1, c2)};

    double prev_ama{get_ama(data, i - 1, n0, c1, c2)};

    double ama{prev_ama + sf_val * (data[i].second - prev_ama)};

    memo_ama[key] = ama;

    return ama;
}

