from datetime import date
import pandas as pd
from jugaad_data.nse import stock_df
from dateutil.relativedelta import relativedelta
import sys

def main():
    symbol1 = sys.argv[1]
    symbol2 = sys.argv[2]
    today = sys.argv[3]
    n = int(sys.argv[4])
    to_day = int(today[:2])
    to_month = int(today[3:5])
    to_year = int(today[6:10])
    today = date(to_year, to_month, to_day)
    t = today - relativedelta(days=1)
    laday = t - relativedelta(days=3*n)
    
    df1 = pd.DataFrame(stock_df(symbol=symbol1, from_date=laday, to_date=t, series="EQ"))
    df1 = df1[['DATE','CLOSE']]
    df1 = df1.iloc[:n]['CLOSE']
    df2 = pd.DataFrame(stock_df(symbol=symbol2, from_date=laday, to_date=t, series="EQ"))
    df2 = df2[['DATE','CLOSE']]
    df2 = df2.iloc[:n]['CLOSE']
    df = df1-df2
    df.to_csv('help.txt', index=False)
    

if __name__ == "__main__":
    main()