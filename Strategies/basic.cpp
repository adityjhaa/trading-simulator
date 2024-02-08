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

    ifstream file("Stocks/" + symbol + ".csv");
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

    long unsigned int len{data.size()};

    int inc_n{};
    int dec_n{};
    double price{data[0].second};
    int stocks{};

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
            }
            inc_n = 0;
        }
        else
        {
            inc_n = dec_n = 0;
        }
        price = data[i].second;
    }

    return 0;
}
