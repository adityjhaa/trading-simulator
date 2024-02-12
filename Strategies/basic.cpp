#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argv, char *argc[])
{

    string symbol{argc[1]};
    int n0{stoi(argc[2])};
    int x{stoi(argc[3])};

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

    int inc_n{};
    int dec_n{};
    double price{data[0].second};
    int stocks{};
    double cashflow{};

    for (int i = 0; i < len; i++)
    {
        if (data[i].second > price)
        {
            if (inc_n < n0)
            {
                inc_n++;
            }
            if (inc_n == n0 and stocks < x)
            {
                // buy
                stocks++;
                order_file << data[i].first << ",BUY,1," << data[i].second << "\n";
                cashflow -= data[i].second;
            }
            dec_n = 0;
        }
        else if (data[i].second < price)
        {
            if (dec_n < n0)
            {
                dec_n++;
            }
            if (dec_n == n0 and stocks > -x)
            {
                // sell
                stocks--;
                order_file << data[i].first << ",SELL,1," << data[i].second << "\n";
                cashflow += data[i].second;
            }
            inc_n = 0;
        }
        else
        {
            inc_n = dec_n = 0;
        }
        price = data[i].second;

        cash_file << data[i].first << "," << cashflow << "\n";
    }

    double final_pnl{cashflow + (stocks*price)};
    final_file << final_pnl;

    cash_file.close();
    order_file.close();
    final_file.close();

    return 0;
}
