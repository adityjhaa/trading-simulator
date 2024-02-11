#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

int main(int argv, char *argc[])
{
    string symbol1{argc[1]};
    string symbol2{argc[2]};
    int x{stoi(argc[3])};
    int n0{stoi(argc[4])};
    int threshold{stoi(argc[5])};
    int stop_loss_threshold{stoi(argc[6])};
    bool is_stop_loss{};

    if (stop_loss_threshold != 0)
    {
        is_stop_loss = true;
    }

    vector<pair<string, double>> data1;
    vector<pair<string, double>> data2;

    ifstream file1("Stocks/" + symbol1 + ".csv");
    ifstream file2("Stocks/" + symbol2 + ".csv");

    if (!file1.is_open() or !file2.is_open())
    {
        cerr << "Error opening file." << endl;
        return 1;
    }
    string first_line1, first_line2;
    getline(file1, first_line1);
    getline(file2, first_line2);

    string line1, line2;
    while (getline(file1, line1))
    {
        string date;
        string price;
        stringstream ss(line1);
        getline(ss, date, ',');
        getline(ss, price, ',');
        data1.push_back({date, stod(price)});
    }
    while (getline(file2, line2))
    {
        string date;
        string price;
        stringstream ss(line2);
        getline(ss, date, ',');
        getline(ss, price, ',');
        data1.push_back({date, stod(price)});
    }

    file1.close();
    file2.close();

    ofstream cash_file("results/daily_cashflow.csv");
    ofstream order_file1("results/order_statistics_1.csv");
    ofstream order_file2("results/order_statistics_2.csv");
    ofstream final_file("results/final_pnl.txt");
    cash_file << "Date,Cashflow\n";
    order_file1 << "Date,Order_dir,Quantity,Price\n";
    order_file2 << "Date,Order_dir,Quantity,Price\n";

    long unsigned int len{data1.size()};
    int stocks{};
    double cashflow{};
    double roll_mean{};
    vector<double> spread;

    for (int i = 0; i < len; i++)
    {
        spread.push_back(data1[i].second - data2[i].second);
        if (i < n0)
        {
            roll_mean += spread[i];
        }
    }

    return 0;
}