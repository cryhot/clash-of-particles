#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import sys

data = np.genfromtxt('data_complexity_heap.csv' if len(sys.argv) == 1 else sys.argv[1],
                     delimiter=',',
                     names=['nb', 'insert', 'extract'])

fig = plt.figure()

# time to insert
ax1 = fig.add_subplot(111)

ax1.set_title("Time to insert all nodes to heap")
ax1.set_xlabel('# of nodes')
ax1.set_ylabel('time')
ax1.plot(data['nb'], data['insert'], c='r', label='insert')

# time to extract
ax2 = fig.add_subplot(111)

ax2.set_title("Time to insert/extract all (min) nodes in/from heap")
ax2.set_xlabel('# of nodes')
ax2.set_ylabel('time')
ax2.plot(data['nb'], data['extract'], c='b', label='extract_min')

leg = ax2.legend()

plt.show()
