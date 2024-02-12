import sys
from datetime import date
import pandas as pd
from jugaad_data.nse import stock_df

# ------------------------------------ generation of data frame --------------------------------------------
def generate_dataframe(symbol, today, lastday, train_data):
    # formatting the date
    to_day = int(today[:2])
    to_month = int(today[3:5])
    to_year = int(today[6:10])
    
    la_day = int(lastday[:2])
    la_month = int(lastday[3:5])
    la_year = int(lastday[6:10])
    
    df = pd.DataFrame(stock_df(symbol=symbol, from_date=date(to_year, to_month, to_day), to_date=date(la_year, la_month, la_day), series="EQ"))
    df = df[[ "DATE", "CLOSE", "HIGH", "LOW", "PREV. CLOSE", "VWAP", "NO OF TRADES", "OPEN"]]
    df = df.iloc[::-1]
    df['DATE'] = pd.to_datetime(df['DATE'], format='%d-%m-%Y')
    df['DATE'] = df['DATE'].dt.strftime('%d/%m/%Y')
    if (train_data == "0"):
        df.to_csv(symbol+".csv", index=False)
    else:
        df.to_csv(symbol+"_train.csv", index = False)

# -------
# ------------------------------------------------- MAIN -----------------------------------------------------------------------
def main():
    argument = sys.argv[1]
    today = sys.argv[2]
    lastday = sys.argv[3]
    train_data = sys.argv[4]

    generate_dataframe(argument, today, lastday, train_data)


if __name__ == "__main__":
    main()
