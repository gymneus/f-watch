#!/usr/bin/python

# Copyright (C) 2014 Julian Lewis
# @author Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, you may find one here:
# http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
# or you may search the http://www.gnu.org website for the version 2 license,
# or you may write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

import sys
import time
import numpy as np
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D

import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

DISP_FIELDS = ['x', 'y', 'z']
AXES_X = 0
AXES_Y = 1
AXES_Z = 2

def get_mag_data(filename):
	f = open(filename)
	vals = []
	for i in range(0, len(DISP_FIELDS)):
		vals.append([])
	for line in f:
		found = 0
		for field in line.split():
			if found == len(DISP_FIELDS):
				break
			val = field.split(':')
			if val[0] in DISP_FIELDS:
				found += 1
				try:
					vals[DISP_FIELDS.index(val[0])].append(int(val[1])) 
				except:
					pass
	return vals

def plot_combo_data(vals):
	fig = []
	ax = []

	# draw y(x)
	fig.append(plt.figure(0))
	ax.append(fig[0].add_subplot(111))
	ax[0].set_xlabel('x')
	ax[0].set_ylabel('y')
	ax[0].plot(vals[AXES_X], vals[AXES_Y], marker='.', linestyle='None', c='b',scaley=True,scalex=True)

	## draw z(x)
	fig.append(plt.figure(1))
	ax.append(fig[1].add_subplot(111))
	ax[1].set_xlabel('x')
	ax[1].set_ylabel('z')
	ax[1].plot(vals[AXES_X], vals[AXES_Z], marker='.', linestyle='None', c='b',scaley=True,scalex=True)

	## draw z(y)
	fig.append(plt.figure(2))
	ax.append(fig[2].add_subplot(111))
	ax[2].set_xlabel('y')
	ax[2].set_ylabel('z')
	ax[2].plot(vals[AXES_Y], vals[AXES_Z], marker='.', linestyle='None', c='b',scaley=True,scalex=True)

	#draw 3d if somebody prefers that
	#fig = plt.figure()
	#ax = fig.add_subplot(111, projection='3d')
	#ax.set_xlabel('x')
	#ax.set_ylabel('y')
	#ax.set_zlabel('z')
	#ax.plot(vals[AXES_X], vals[AXES_Y], vals[AXES_Z], marker='.', linestyle='None')

#not very sophisticated hard- and soft-iron calibration
def calibrate(vals):
	min_x = min(vals[AXES_X])
	max_x = max(vals[AXES_X])
	min_y = min(vals[AXES_Y])
	max_y = max(vals[AXES_Y])
	min_z = min(vals[AXES_Z])
	max_z = max(vals[AXES_Z])
	# hard-iron calibration
	print "hard-iron x=", (min_x + max_x)/2.0
	print "hard-iron y=", (min_y + max_y)/2.0
	print "hard-iron z=", (min_z + max_z)/2.0
	# soft-iron calibration
	vmax_x = max_x - (min_x + max_x)/2.0
	vmax_y = max_y - (min_x + max_x)/2.0
	vmax_z = max_z - (min_x + max_x)/2.0
	vmin_x = min_x - (min_x + max_x)/2.0
	vmin_y = min_y - (min_x + max_x)/2.0
	vmin_z = min_z - (min_x + max_x)/2.0
	avg_x = (vmax_x + abs(vmin_x))/2.0
	avg_y = (vmax_y + abs(vmin_y))/2.0
	avg_z = (vmax_z + abs(vmin_z))/2.0
	avg_tot = (avg_x + avg_y + avg_z)/3.0
	print "soft-iron x=", avg_tot/avg_x
	print "soft-iron y=", avg_tot/avg_y
	print "soft-iron z=", avg_tot/avg_z

if len(sys.argv) == 1:
	print "You did not specify the log file"
	sys.exit()

vals = get_mag_data(sys.argv[1])
calibrate(vals)
#correction
#for i in range(0, len(vals[0])):
#	#hard iron
#	vals[0][i] = vals[0][i] - 176
#	vals[1][i] = vals[1][i] - 368
#	vals[2][i] = vals[2][i] - 484
#	#soft iron
#	vals[0][i] = vals[0][i]*0.994949494949
#	vals[1][i] = vals[1][i]*1.00346373268
#	vals[2][i] = vals[2][i]*1.00162700834
plot_combo_data(vals)
plt.show()
