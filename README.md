# Earnings-surprise-on-stock-price
Impact of Earnings Surprise on Stock Price (C++) using libcurl and Excel Driver

•	Selected and sorted stocks from S&P 500 into 3 groups based on their earnings and EPS Estimate from Bloomberg terminal
Group: fiscal 2016 Q3 Data of S&P 500 Stocks from Bloomberg. Beat criteria: Surprise above 10%; Meet Criteria: Surprise between -10%-10%; Miss Criteria: Surprise below -10%

•	Retrieved historical prices around the date of earning release using external library (libcurl) from Yahoo Finance

•	Computed CCAR (cumulative average abnormal returns) for each group, generated a CAAR Excel chart via C++

•	Reached the conclusion that stock behavior does respond gradually to the earnings announcement

Input file: data_bloomberg_0420.csv under ExcelDriver directory. To run the code, please set up libcurl environment in your visual studio project firstly.
