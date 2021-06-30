# -*- coding: utf-8 -*-
"""
Created on Tue Dec 29 10:09:50 2020

@author: Ardi
"""

import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd

file = "C:/Users/Ardi/Documents/PIM.csv"
data = pd.read_csv(file)
#data.drop_duplicates()
#data1 = data.drop_duplicates(subset=['latIMUstd'])

mpl.rcParams['legend.fontsize'] = 10
#plt.plot(data['time'], data['latIMUstd'], label='INS', color='blue')
#plt.plot(data['time'], data['latGPSstd'], label='GPS', color='red')
#plt.plot(data['time'], data['longIMUstd'], label='INS', color='blue')
#plt.plot(data['time'], data['longGPSstd'], label='GPS', color='red')
plt.plot(data['time'], data['altIMUstd'], label='INS', color='blue')
plt.plot(data['time'], data['altGPSstd'], label='GPS', color='red')

#plt.plot(data1['time'], data2['latGPSstd'], label='GPS2')
plt.legend()
plt.tick_params(labelsize=10)
plt.xlabel('Waktu (s)', fontsize=10)
plt.ylabel('Error (m)', fontsize=10)
plt.title("Grafik Error dari Altitude Pengujian Ketiga Zona 1")
# Latitude Longitude Altitude
#plt.margins(0)
plt.grid(linestyle=':')
plt.tight_layout()
plt.show()