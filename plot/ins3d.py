# -*- coding: utf-8 -*-
"""
Created on Tue Dec 15 10:44:56 2020

@author: Ardi
"""

import matplotlib.pyplot as plt
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

fileIMU = "C:/Users/Ardi/Documents/dataIMU_1.csv"
fileGPS = "C:/Users/Ardi/Documents/dataGPS_1.csv"
dataIMU = pd.read_csv(fileIMU)
dataGPS = pd.read_csv(fileGPS)

mpl.rcParams['legend.fontsize'] = 10
fig = plt.figure()
fig.suptitle('Grafik 3D Lintasan Pengujian Ketiga', fontsize=12)
ax = fig.gca(projection='3d')
ax.plot(dataIMU['yy'], dataIMU['xx'], dataIMU['zz'], label='INS', color='blue')
ax.plot(dataGPS['yy'], dataGPS['xx'], dataGPS['zz'], label='GPS', color='red')
ax.legend()
ax.tick_params(labelsize=8)
ax.set_xlabel('Longitude', fontsize=10)
ax.set_ylabel('Latitude', fontsize=10)
ax.set_zlabel('Altitude', fontsize=10)
#plt.title("Simple Plot")
plt.tight_layout()
plt.show()