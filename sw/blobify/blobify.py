#!/usr/bin/python

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

