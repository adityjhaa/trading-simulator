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

def generate_candlestick_chart(PATH, symbol):
    df = pd.read_pickle(PATH)
    df['DATE'] = pd.to_datetime(df['DATE'])
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
    df = request.args.get('arg1')
    symbol = request.args.get('arg2')
    symbols = []
    symbols.append(symbol)
    if df:
        candlestick_chart = generate_candlestick_chart(df, symbol)

        return render_template("graph.html", user = current_user, candlestick_chart = candlestick_chart, symbols = symbols)

    return redirect(url_for('views.home'))