#!/usr/bin/python
#
# Copyright (C) 2014 Julian Lewis
# author Theodor Stana <theodor.stana@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, you may find one here:
# http://www.gnu.org/licenses/gpl-3.0-standalone.html
# or you may search the http://www.gnu.org website for the version 3 license,
# or you may write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

if __name__ == "__main__":
	fn1 = raw_input("bootloader  (.bin): ")
	fn2 = raw_input("application (.bin): ")
	fn3 = raw_input("blob output (.bin): ")

	f1 = open(fn1,'rb')
	f2 = open(fn2,'rb')
	f3 = open(fn3,'wb')
	a1 = []
	a2 = []

	# read contents
	a1 = f1.read()
	a2 = f2.read()

	# Pad with zeroes up to bootloader start
	lt = 0x8000 - (len(a1) % 0x8000)
	for i in range(0, lt):
		a1 += '\0'

	# write output files
	f3.write(a1+a2)

	print("SUCCESS -- Output written to " + fn3)
	f1.close()
	f2.close()
	f3.close()

