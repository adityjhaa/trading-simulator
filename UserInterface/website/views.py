# this file handels the frontend that is the home page
from flask import Blueprint, render_template, request, redirect, url_for
from flask_login import login_required, current_user
from datetime import *
from dateutil.relativedelta import relativedelta
from jugaad_data.nse import stock_df
import plotly.graph_objs as go
import pandas as pd

views = Blueprint('views', __name__)
PATH = "./website/data/DATA.pkl"

stock_lists=[]
indicators = []

def generate_macd_trace(df):
    short_window = 12
    long_window = 26
    
    short_ema = df['CLOSE'].ewm(span=short_window, adjust=False).mean()
    long_ema = df['CLOSE'].ewm(span=long_window, adjust=False).mean()
    
    macd_line = short_ema - long_ema
    
    signal_window = 9
    signal_line = macd_line.ewm(span=signal_window, adjust=False).mean()
    
    df['MACD'] = macd_line
    df['Signal_line'] = signal_line
    
    macd_trace = go.Scatter(x = df['DATE'], y = df['MACD'], mode='lines', name='MACD line')
    signal_trace = go.Scatter(x = df['DATE'], y = df['Signal_line'], mode='lines', name='Signal line')
    return macd_trace, signal_trace

def generate_rsi_trace(df):
    df['Price Change'] = df['CLOSE'].diff()

    rsi_period = 14

    df['Gain'] = df['Price Change'].apply(lambda x: x if x > 0 else 0)
    df['Loss'] = df['Price Change'].apply(lambda x: abs(x) if x < 0 else 0)

    df['Average Gain'] = df['Gain'].rolling(window=rsi_period, min_periods=1).mean()
    df['Average Loss'] = df['Loss'].rolling(window=rsi_period, min_periods=1).mean()

    df['RS'] = df['Average Gain'] / df['Average Loss']

    df['RSI'] = 100 - (100 / (1 + df['RS']))
    
    rsi_trace = go.Scatter(x = df['DATE'], y = df['RSI'], mode = 'lines', name='RSI line')
    return rsi_trace

def generate_s50_trace(df):
    sma_window = 50
    df['SMA50'] = df['Close'].rolling(window=sma_window, min_periods=1).mean()

def generate_s100_trace(df):
    sma_window = 100
    df['SMA100'] = df['Close'].rolling(window=sma_window, min_periods=1).mean()

def generate_candlestick_chart(PATH, symbol, button, indicators = ""):
    df = pd.read_pickle(PATH)
    if button == "d":
        df['idx'] = df['DATE']
        df.set_index('idx', inplace = True)
        df = df.resample('D').last()        
    if button == "m":
        df['idx'] = df['DATE']
        df.set_index('idx', inplace = True)
        df = df.resample('M').last()
    if button == "w":
        df['idx'] = df['DATE']
        df.set_index('idx', inplace = True)
        df = df.resample('W').last()

    trace_list = []
    candlestick_trace = go.Candlestick(x=df['DATE'],
                                    open=df['OPEN'],
                                    high=df['HIGH'],
                                    low=df['LOW'],
                                    close=df['CLOSE'], name='Stock Data')
    trace_list.append(candlestick_trace)

    if 'm' in indicators:
        macd_trace, signal_trace = generate_macd_trace(df)
        trace_list.append(macd_trace)
        trace_list.append(signal_trace)
    if 'r' in indicators:
        rsi_trace = generate_rsi_trace(df)
        trace_list.append(rsi_trace)
    if 's50' in indicators:
        s50_trace = generate_s50_trace(df)
    if 's100' in indicators:
        s100_trace = generate_s100_trace(df)

    layout = go.Layout(title=symbol, xaxis=dict(title='Date'), yaxis=dict(title='Price'),height=800,dragmode='pan')
    figure = go.Figure(data=trace_list, layout=layout)
    figure.update_layout(
    xaxis=dict(
        rangeselector=dict(
            buttons=list([
                dict(count=1,
                     label="1m",
                     step="month",
                     stepmode="backward"),
                dict(count=6,
                     label="6m",
                     step="month",
                     stepmode="backward"),
                dict(count=1,
                     label="1y",
                     step="year",
                     stepmode="backward"),
                dict(step="all")
                ])
            ),
            rangeslider=dict(
                visible=True
            ),
            type="date"
        )
    )
    return figure.to_html(full_html=False)

@views.route('/', methods = ['GET', 'POST'])
@login_required
def home():
    stock_lists.clear()
    today = date.today()
    last = today - relativedelta(years = 5)
    df = stock_df(symbol = "SBIN", from_date=last, to_date=today, series="EQ")

    if request.method=='POST':
      
        if request.form.get('start'):
            today = request.form.get('start')
            today = datetime.strptime(today, '%Y-%m-%d').date()

        if request.form.get('end'):
            last = request.form.get('end', default=last)
            last = datetime.strptime(last, '%Y-%m-%d').date()

        if request.form.get('symbol'):
            symbol = request.form.get('symbol', default=df)

            df = stock_df(symbol = symbol, from_date=last, to_date=today, series="EQ")
            df = pd.to_pickle(df, PATH)
            return redirect(url_for('views.graph', arg1 = PATH, arg2 =  symbol))

    return render_template("home.html", user = current_user)

@views.route('/graph', methods = ['GET', 'POST'])
@login_required
def graph():
    
    stock_lists.clear()
    PATH = request.args.get('arg1')
    symbol = request.args.get('arg2')
    button = ""
    print(indicators)
    
    if request.method == "POST":
        
        # daily_weekly_monthly control
        button_daily = request.form.get('Daily')
        button_weekly = request.form.get('Weekly')
        button_monthly = request.form.get('Monthly')
        if button_daily:
            button = "d"
        elif button_weekly:
            button = "w"
        elif button_monthly:
            button = "m"
            
        # Indicators control
        button_MACD = request.form.get('MACD')
        button_RSI = request.form.get('RSI')
        button_SMA_50 = request.form.get('SMA-50')
        button_SMA_100 = request.form.get('SMA-100')
        if button_MACD:
            indicators.append('m')
        if button_RSI:
            indicators.append('r')
        if button_SMA_50:
            indicators.append('s50')
        if button_SMA_100:
            indicators.append('s100')
        if request.form.get('clear'):
            indicators.clear()
        
    if PATH:
        candlestick_chart = generate_candlestick_chart(PATH, symbol, button, indicators)
        return render_template("graph.html", user = current_user, candlestick_chart = candlestick_chart)

    return redirect(url_for('views.home'))

@views.route('/compare', methods = ['GET', 'POST'])
@login_required
def compare():
    if request.method == "POST":
        if request.form.get('add-symbol'):
            symbol = request.form.get('add-symbol')
            today = date.today()
            last = today - relativedelta(years = 5)
            df = stock_df(symbol = symbol, from_date=last, to_date=today, series="EQ")
            stock_lists.append(df)
        if request.form.get('clear'):
            stock_lists.clear()

    trace_list=[]
    for stock in stock_lists:
        trace_list.append(go.Scatter(x=stock['DATE'],
                                     y=stock['CLOSE'],
                                     mode='lines',
                                     name=stock['SYMBOL'][0]
                                     ))
    layout = go.Layout(title='COMPARISON CHART', xaxis=dict(title='Date'),yaxis=dict(title='Price'),xaxis_rangeslider=dict(visible=True),height=700)
    figure = go.Figure(data=trace_list, layout=layout)
    chart = figure.to_html(full_html=False)

    return render_template("compare.html", user = current_user, compare_graph=chart)