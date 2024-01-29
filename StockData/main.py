from jugaad_data.nse import stock_df
from datetime import date
from dateutil.relativedelta import relativedelta
import os
import time
import sys
import pandas
from matplotlib import pyplot as plt
import matplotlib.cm as cm

symbol = sys.argv[1]
x=int(sys.argv[2])

today = date.today()
last = today - relativedelta(years=x) #+ relativedelta(days=1)
t = str(symbol)

df = stock_df(symbol=symbol, from_date=last,to_date=today, series="EQ")
columns = ['DATE', 'OPEN', 'CLOSE', 'HIGH','LOW', 'LTP', 'VOLUME', 'VALUE', 'NO OF TRADES']
df = df.loc[:,columns]
sizes=[]
times=[]


file=t+".csv"
stime = time.time()
df.to_csv(file, index=False)
etime = time.time() 
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)

file=t+".txt"
stime = time.time()
df.to_csv(file, sep='\t', index=False)
etime = time.time() 
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)

file=t+".json"
stime = time.time()
df.to_json(file)
etime = time.time() 
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)

file=t+".parquet"
stime = time.time()
df.to_parquet(file, engine='fastparquet')
etime = time.time() 
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)

file=t+".orc"
stime = time.time()
df.to_orc(file)
etime = time.time()
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)

file=t+".feather"
stime = time.time()
df.to_feather(file)
etime = time.time() 
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)

file=t+".pkl"
stime = time.time()
df.to_pickle(file)
etime = time.time() 
sizes.append(os.path.getsize(file)/1000)
times.append((etime-stime)*10000)



files = ["csv", "txt", "json", "parquet", "orc", "feather", "pickle"]
colors = ['red', 'purple', 'brown', 'green', 'blue', 'orange', 'black']

for i in range(len(files)):
    plt.scatter(times[i],sizes[i],c=colors[i], label=files[i])


plt.title('Differrent file types')
plt.xlabel('time to create files(10-4 s)')
plt.ylabel('size of file(kilobytes)')
plt.legend()
plt.savefig(t+'.png')
