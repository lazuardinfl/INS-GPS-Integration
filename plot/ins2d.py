# -*- coding: utf-8 -*-
"""
Created on Tue Dec 29 08:57:41 2020

@author: Ardi
"""

import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd

fileIMU = "C:/Users/Ardi/Documents/dataIMU_3.csv"
fileGPS = "C:/Users/Ardi/Documents/dataGPS_3.csv"
dataIMU = pd.read_csv(fileIMU)
dataGPS = pd.read_csv(fileGPS)

mpl.rcParams['legend.fontsize'] = 10
plt.plot(dataIMU['yy'], dataIMU['xx'], label='INS')
plt.plot(dataGPS['yy'], dataGPS['xx'], label='GPS')
plt.legend()
plt.tick_params(labelsize=10)
plt.xlabel('Longitude', fontsize=10)
plt.ylabel('Latitude', fontsize=10)
plt.title("Simple Plot")
plt.grid(linestyle=':')
plt.tight_layout()
plt.show()