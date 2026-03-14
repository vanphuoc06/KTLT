import pandas as pd
df=pd.read_csv(r"C:\Users\ADMIN\Downloads\Đồ án\Input\all_stocks_5yr.csv\all_stocks_5yr.csv")
df=df.drop(columns=['open','high','low'])
df.to_csv(r"C:\Users\ADMIN\Downloads\Đồ án\Input\portfolio.csv",index=False)