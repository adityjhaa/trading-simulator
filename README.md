# Trading Simulator & Analyzer

## 1\. Introduction

The primary goal is to provide users with a user-friendly interface to view stock data, compare multiple stocks on the same graph, and apply various filters for better analysis.

## 2\. Technologies Used

-   Flask: Chosen for its simplicity and flexibility, Flask allows us to build a web application quickly and efficiently.

-   Pandas: Utilized for data manipulation and analysis, especially for handling stock data.

-   Plotly: Employed for creating interactive and visually appealing graphs to display stock information.

-   Flask\_login: Used for management of user login and logout states.

-   WerkZeug: This module is commonly used in web applications, especially with Flask, to manage user passwords securely.

-   Jugaad data: For fetching the stock data.

-   HTML, CSS: For the basic structure and design elements for the web application.

## 3\. Key Features

### 3.1 Stock Display:

-   The backend fetches stock data from Jugaad Data and presents it.

-   Historical data is displayed, including opening, closing, and high and low prices.

-   Users can choose specific time intervals (daily, weekly, monthly) for stock data visualization and the beginning and end dates.

### 3.2 Multiple Stock Comparison:

-   The website allows users to compare stocks on the same graph for a comprehensive analysis.

-   Users can input stock symbols or select from a predefined list to visualize the performance of multiple stocks simultaneously.

### 3.3 Filtering Options:

To enhance analysis, the backend supports various filters:

-   MACD: Moving Average Convergence Divergence is also implemented for the user to check the validity of the stock.

-   SMA: Incorporating Simple Moving Averages (SMA) with customizable window sizes (we used SBA-50 and SBA-100).

-   RSI: Users can apply Relative Strength Index(RSI) filters to identify overbought or oversold conditions.

### 3.4 User Authentication and Authorization:

-   The backend implements user authentication and authorization mechanisms to ensure data privacy and security.

-   Authenticated users have access to personalized features, and administrators have additional privileges.

## 4\. Architecture

The backend follows a modular and scalable architecture:

-   Routes: Defined for handling different URLs and HTTP methods.

-   Controllers: Responsible for handling business logic, such as fetching stock data, applying filters, and preparing data for visualization.

-   Models: Represent the data structures used in the application.

-   Templates: Developed using Jinja2 to create dynamic HTML pages.

-   API Integration: Utilizes YFinance API for fetching real-time stock data.

## 5\. Website Layout

The first-time user is directed to the registration page.

![](.//media/image3.png)
<br><br>
Here, an account can be created by the user and he/she will be sent to
the home page. If the user already has an account, he/she can login
directly by going to the login page.

![](.//media/image5.png)
<br><br>
After log in or after creating an account, the user is greeted with the
home page.

![](.//media/image7.png)

<br><br>
Here, the user can search a stock by its symbol, the start date and end
date (if the date bounds are not given, the program uses 10 years before
the present day and the present day as the default bounds).

The program loads the stocks as a candle-stick chart. The user has some
useful options here:

-   He/she can choose whether to view the daily, weekly or monthly stock price changes.

-   He/she can choose to view in some pre-defined time ranges (e.g. 1 month, 6 month, 1 year, etc.).

-   He/she can choose between a number of technical filters for enhanced analysis (e.g. MACD, RSI, SMA).

-   Adjustment of range with the help of the range-slider facilitates the experience even more.

![](.//media/image1.png)
<br><br>

An example of filters is shown below.

![](.//media/image6.png)
<br><br>

These features make it extremely user-friendly and informative for the
user. Now, we shall see the next big thing - Comparison.

![](.//media/image4.png)
<br><br>

On this page, the user can add multiple stocks to view them
simultaneously. This gives a better comparison sense between the
multiple stocks and hence makes this web application even more useful.

Once the user has all his information, he may want to logout (for
privacy reasons). The logout button comes into picture here and makes
logging out one click away from the user.

![](.//media/image2.png)
<br><br>

This takes him/her back to the login page where he/she can easily login
for any information he/she may want to gain.

We had a complete walkthrough of this amazing web application. Hope you
are excited to use it for your stock related queries.

## Designed by-
## _Aditya Jha & Jakharia Rishit_
