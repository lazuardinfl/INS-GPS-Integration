# -*- coding: utf-8 -*-
"""
Created on Tue Dec 15 10:06:15 2020

@author: Ardi
"""

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

fileIMU = "C:/Users/Ardi/Documents/dataIMU_1.csv"
fileGPS = "C:/Users/Ardi/Documents/dataGPS_1.csv"

fig = plt.figure()
fig.suptitle('INS vs GPS', fontsize=12)
ax = fig.gca(projection='3d')
mpl.rcParams['legend.fontsize'] = 10

def animate(i):
    dataIMU = pd.read_csv(fileIMU)
    dataGPS = pd.read_csv(fileGPS)
    ax.cla()
    ax.plot(dataIMU['yy'], dataIMU['xx'], dataIMU['zz'], label='INS')
    ax.plot(dataGPS['yy'], dataGPS['xx'], dataGPS['zz'], label='GPS')
    ax.legend()
    ax.tick_params(labelsize=10)
    ax.set_xlabel('Longitude', fontsize=10)
    ax.set_ylabel('Latitude', fontsize=10)
    ax.set_zlabel('Altitude', fontsize=10)
    
ani = FuncAnimation(fig, animate, interval=0.1)
#ani = FuncAnimation(fig, animate, interval=10, blit=True)
plt.tight_layout()
plt.show()