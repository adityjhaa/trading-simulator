# this file handels the frontend that is the home page
from flask import Blueprint, render_template, request
from flask_login import login_required, current_user
from datetime import *
from dateutil.relativedelta import relativedelta
from jugaad_data.nse import stock_df
import plotly.graph_objs as go

views = Blueprint('views', __name__)

@views.route('/', methods = ['GET', 'POST'])
@login_required
def home():

    today = date.today()
    last = today - relativedelta(years = 2)
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
        
        def generate_candlestick_chart():
            candlestick_trace = go.Candlestick(x=df['DATE'],
                                            open=df['OPEN'],
                                            high=df['HIGH'],
                                            low=df['LOW'],
                                            close=df['CLOSE'])

            layout = go.Layout(title='Candlestick Chart', xaxis=dict(title='Date'), yaxis=dict(title='Price'),height=600, xaxis_rangeslider_visible=False,dragmode='pan')

            figure = go.Figure(data=[candlestick_trace], layout=layout)
            return figure.to_html(full_html=False)
    
        if not df.empty:
            candlestick_chart = generate_candlestick_chart()

            print("yes")

            return render_template("graph.html", user = current_user, candlestick_chart = candlestick_chart)

    return render_template("home.html", user = current_user)
