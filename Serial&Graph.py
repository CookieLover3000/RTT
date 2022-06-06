import time

import matplotlib
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
# matplotlib.use('module://matplotlib-backend-notcurses')
# matplotlib.use("TkAgg")


from serial.tools import list_ports
# port = list(list_ports.comports())
# for p in port:
#     print(p.device)

ser = serial.Serial('/dev/cu.usbmodem21402')  # open serial port
print(ser.name)  # check which port was really used

data = []
data_time = []

style.use('fivethirtyeight')
fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)


def animate(i):
    try:
        temp = int(ser.readline())
        if temp > 200 & temp < 500:
            print(temp)
            data.append(temp)
            # data.append(int(ser.readline()))
            data_time.append(round(time.time()*1000))

    except:
        print(':) help')

    ax1.clear()
    ax1.plot(data_time, data)


ani = animation.FuncAnimation(fig, animate, interval=50, repeat=True, frames=200)
plt.show()


# ser.close()  # close port
