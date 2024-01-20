from jugaad_data.nse import stock_df
from datetime import date
from dateutil.relativedelta import relativedelta
import os
import time
import sys
import pandas

symbol = sys.argv[1]
x=int(sys.argv[2])

today = date.today()
last = today - relativedelta(years=x) + relativedelta(days=1)
print(last)
print(today)
t = str(symbol)


file=t+".csv"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_csv(file)
etime = time.time() 
size = (os.path.getsize(file))
print(file)
print(size)
print((etime-stime)*1000)


file=t+".json"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_json(file)
etime = time.time() 
size = (os.path.getsize(file))
print(file)
print(size)
print((etime-stime)*1000)

file=t+".html"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_html(file)
etime = time.time() 
size = (os.path.getsize(file))
print(file)
print(size)
print((etime-stime)*1000)

file=t+".tex"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_latex(file)
etime = time.time() 
size = (os.path.getsize(file))
print(file)
print(size)
print((etime-stime)*1000)

file=t+".txt"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_csv(file, sep='\t', index=False)
etime = time.time() 
size = (os.path.getsize(file))
print(file)
print(size)
print((etime-stime)*1000)

file=t+".parquet"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_parquet(file, compression=None)
etime = time.time() 
size = (os.path.getsize(file))
print(size)
print((etime-stime)*1000)

file=t+".orc"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_orc(file)
etime = time.time() 
size = (os.path.getsize(file))
print(size)
print((etime-stime)*1000)

file=t+".feather"
stime = time.time()
df = stock_df(symbol="SBIN", from_date=last,to_date=today, series="EQ")
df.to_feather(file)
etime = time.time() 
size = (os.path.getsize(file))
print(size)
print((etime-stime)*1000)

