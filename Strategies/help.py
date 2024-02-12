from datetime import date
import pandas as pd
from jugaad_data.nse import stock_df
from dateutil.relativedelta import relativedelta
import sys

def main():
    symbol = sys.argv[1]
    today = sys.argv[2]
    to_day = int(today[:2])
    to_month = int(today[3:5])
    to_year = int(today[6:10])
    today = date(to_year, to_month, to_day)
    t = today - relativedelta(days=1)
    laday = t - relativedelta(days=3)
    
    df = pd.DataFrame(stock_df(symbol=symbol, from_date=laday, to_date=t, series="EQ"))
    df = df[['DATE','CLOSE']]
    df = df.iloc[0]['CLOSE']
    with open('help.txt', 'w') as f:
        f.write(str(df))

    
if __name__ == "__main__":
    main()