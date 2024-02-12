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
        data2.push_back({date, stod(price)});
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
    double sum{};
    double sumOfSquares{};
    double roll_mean{};
    double sd{};
    double z_score{};
    double variance{};
    double spread;

    for (int i = 0; i < len; i++)
    {
        spread = (data1[i].second - data2[i].second);
        if (i < n0)
        {
            sum += spread;
            roll_mean = sum / (i + 1);
            sumOfSquares += spread * spread;
            variance = sumOfSquares / (i + 1) - (roll_mean * roll_mean);
        }
        sd = sqrt(variance);

        z_score = (spread - roll_mean) / sd;

        if (z_score > threshold and stocks > -x)
        {
            // sell
            stocks--;
            order_file1 << data1[i].first << ",SELL,1," << data1[i].second << "\n";
            order_file2 << data2[i].first << ",BUY,1," << data2[i].second << "\n";
            cashflow += data1[i].second;
            cashflow -= data2[i].second;
        }
        else if (z_score < -threshold and stocks < x)
        {
            // buy
            stocks++;
            order_file1 << data1[i].first << ",BUY,1," << data1[i].second << "\n";
            order_file2 << data2[i].first << ",SELL,1," << data2[i].second << "\n";
            cashflow -= data1[i].second;
            cashflow += data2[i].second;
        }

        // stop loss impllementation
        if (is_stop_loss == true)
        {
            if (z_score > stop_loss_threshold and stocks > -x)
            {
                // sell
                stocks--;
                order_file1 << data1[i].first << ",SELL,1," << data1[i].second << "\n";
                order_file2 << data2[i].first << ",BUY,1," << data2[i].second << "\n";
                cashflow += data1[i].second;
                cashflow -= data2[i].second;
            }
            else if (z_score < -stop_loss_threshold and stocks < x)
            {
                // buy
                stocks++;
                order_file1 << data1[i].first << ",BUY,1," << data1[i].second << "\n";
                order_file2 << data2[i].first << ",SELL,1," << data2[i].second << "\n";
                cashflow -= data1[i].second;
                cashflow += data2[i].second;
            }
        }

        if (i >= n0)
        {
            double past_spread = data1[i - n0].second - data2[i - n0].second;
            sum += (spread) - (past_spread);
            roll_mean = sum / n0;
            sumOfSquares += (spread * spread) - (past_spread * past_spread);
            variance = sumOfSquares / n0 - (roll_mean * roll_mean);
        }
        // cash_file
        cash_file << data1[i].first << "," << cashflow << "\n";
    }

    double final_pnl{cashflow + (stocks * data1[len - 1].second) - (stocks * data2[len - 1].second)};
    final_file << "Final pnl : " << final_pnl << "\n";

    cash_file.close();
    order_file1.close();
    order_file2.close();
    final_file.close();
    return 0;
}