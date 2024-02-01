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

def generate_candlestick_chart(PATH, symbol, button):
    df = pd.read_pickle(PATH)
    if button == "m":
        df['idx'] = df['DATE']
        df.set_index('idx', inplace = True)
        df = df.resample('M').last()
    if button == "w":
        df['idx'] = df['DATE']
        df.set_index('idx', inplace = True)
        df = df.resample('W').last()
    candlestick_trace = go.Candlestick(x=df['DATE'],
                                    open=df['OPEN'],
                                    high=df['HIGH'],
                                    low=df['LOW'],
                                    close=df['CLOSE'])
    layout = go.Layout(title=symbol, xaxis=dict(title='Date'), yaxis=dict(title='Price'),height=800,dragmode='pan')
    figure = go.Figure(data=[candlestick_trace], layout=layout)
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
    if request.method == "POST":
        button_daily = request.form.get('Daily')
        button_weekly = request.form.get('Weekly')
        button_monthly = request.form.get('Monthly')
        if button_daily:
            button = "d"
        elif button_weekly:
            button = "w"
        elif button_monthly:
            button = "m"
    if PATH:
        candlestick_chart = generate_candlestick_chart(PATH, symbol, button)
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