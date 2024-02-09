import sys
from datetime import date
import pandas as pd
from jugaad_data.nse import stock_df

# ------------------------------------ generation of data frame --------------------------------------------
def generate_dataframe(symbol, today, lastday):
    # formatting the date
    to_day = int(today[:2])
    to_month = int(today[3:5])
    to_year = int(today[6:10])
    
    la_day = int(lastday[:2])
    la_month = int(lastday[3:5])
    la_year = int(lastday[6:10])
    
    df = pd.DataFrame(stock_df(symbol=symbol, from_date=date(to_year, to_month, to_day), to_date=date(la_year, la_month, la_day), series="EQ"))
    df = df[[ "DATE", "CLOSE"]]
    df = df.iloc[::-1]
    df['DATE'] = pd.to_datetime(df['DATE'], format='%d-%m-%Y')
    df['DATE'] = df['DATE'].dt.strftime('%d/%m/%Y')
    df.to_csv("Stocks/"+symbol+".csv", index=False)


# ----------------------------------------------- pickle --------------------------------------------------------
def write_pickle(DATA, symbol):
    pd.to_pickle(DATA, symbol + ".pkl")

# -------------------------------------------------------- MAIN -----------------------------------------------------------------------
def main():
    argument = sys.argv[1]
    today = sys.argv[2]
    lastday = sys.argv[3]

    generate_dataframe(argument, today, lastday)
    #write_pickle(DATA, "Stocks/" + argument)


if __name__ == "__main__":
    main()