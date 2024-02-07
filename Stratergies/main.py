import sys
from datetime import date
import pandas as pd
from datetime import date
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
    df = df[[ "DATE", "OPEN", "CLOSE", "HIGH","LOW", "LTP", "VOLUME", "VALUE","NO OF TRADES"]]
    return df


# ----------------------------------------------- pickle --------------------------------------------------------
def write_pickle(DATA, symbol):
    pd.to_pickle(DATA, symbol + ".pkl")

# -------------------------------------------------------- MAIN -----------------------------------------------------------------------
def main():
    argument = sys.argv[1]
    today = sys.argv[2]
    lastday = sys.argv[3]

    DATA = generate_dataframe(argument, today, lastday)
    write_pickle(DATA, "Stocks/" + argument)


if __name__ == "__main__":
    main()