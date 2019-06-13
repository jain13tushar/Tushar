# -*- coding: utf-8 -*-
"""
Created on Thu Jun  6 16:02:54 2019

@author: TJ069482
"""

import pandas as pd

df1 = pd.read_csv("Humidity.csv", usecols = ['date_time','Humidity_value'])
df2 = pd.read_csv("Temperature.csv", usecols = ['Temperature_value'])
df3 = pd.read_csv("li.csv", usecols = ['li_value'])
df4 = pd.read_csv("Fs.csv", usecols = ['Fan_value'])

df5 = pd.read_csv("result.csv", usecols = ['date_time','Humidity_value','Temperature_value','li_value','Fan_value'])



df = pd.DataFrame(df5,columns=['date_time','Humidity_value','Temperature_value','li_value','Fan_value'])

finaldf = pd.concat([df1, df2, df3, df4], axis=1, join='inner').sort_index()

#out = df1.append(df2)
with open('C:\\Users\\TJ069482\\.spyder-py3\\result.csv', 'w', encoding='utf-8',newline='') as f:
    finaldf.to_csv(f, index=False)

#writer1 = pd.DataFrame(df1)
#writer2 = pd.DataFrame(df2)
#writer1.to_csv('out.csv',index=False)
#writer2.to_csv('outs.csv',index=False)
