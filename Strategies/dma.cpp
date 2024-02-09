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
    int p{stoi(argc[4])};

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
        getline(ss, price, '\n');
        data.push_back({date, stod(price)});
    }

    file.close();

    return 0;
}