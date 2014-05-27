EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:freewatch
LIBS:freewatch_v1-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "Freewatch Peripherals & Sensors"
Date ""
Rev "v1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L L70 U6
U 1 1 537A7ED7
P 5100 1850
F 0 "U6" H 5800 2000 60  0000 C CNN
F 1 "L70" H 5800 700 60  0000 C CNN
F 2 "" H 5100 1850 60  0000 C CNN
F 3 "" H 5100 1850 60  0000 C CNN
	1    5100 1850
	1    0    0    -1  
$EndComp
$Comp
L LSM303C U11
U 1 1 537A7F34
P 2750 4750
F 0 "U11" H 3550 4900 60  0000 C CNN
F 1 "LSM303C" H 3550 4000 60  0000 C CNN
F 2 "" H 4350 4750 60  0000 C CNN
F 3 "" H 4350 4750 60  0000 C CNN
	1    2750 4750
	1    0    0    -1  
$EndComp
$Comp
L MAX44009 U10
U 1 1 537A7F7B
P 7800 4050
F 0 "U10" H 8350 4200 60  0000 C CNN
F 1 "MAX44009" H 8350 3600 60  0000 C CNN
F 2 "" H 7800 4050 60  0000 C CNN
F 3 "" H 7800 4050 60  0000 C CNN
	1    7800 4050
	1    0    0    -1  
$EndComp
$Comp
L MPL3115A2 U12
U 1 1 537A8023
P 7700 5300
F 0 "U12" H 8300 5450 60  0000 C CNN
F 1 "MPL3115A2" H 8250 4850 60  0000 C CNN
F 2 "" H 7700 5300 60  0000 C CNN
F 3 "" H 7700 5300 60  0000 C CNN
	1    7700 5300
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR019
U 1 1 537C3EC0
P 2800 2650
F 0 "#PWR019" H 2800 2750 30  0001 C CNN
F 1 "VCC" H 2800 2750 30  0000 C CNN
F 2 "" H 2800 2650 60  0000 C CNN
F 3 "" H 2800 2650 60  0000 C CNN
	1    2800 2650
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR020
U 1 1 537C3EDE
P 7550 4000
F 0 "#PWR020" H 7550 4100 30  0001 C CNN
F 1 "VCC" H 7550 4100 30  0000 C CNN
F 2 "" H 7550 4000 60  0000 C CNN
F 3 "" H 7550 4000 60  0000 C CNN
	1    7550 4000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR021
U 1 1 537C3F18
P 6700 5250
F 0 "#PWR021" H 6700 5350 30  0001 C CNN
F 1 "VCC" H 6700 5350 30  0000 C CNN
F 2 "" H 6700 5250 60  0000 C CNN
F 3 "" H 6700 5250 60  0000 C CNN
	1    6700 5250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR022
U 1 1 537C3F29
P 1550 4700
F 0 "#PWR022" H 1550 4800 30  0001 C CNN
F 1 "VCC" H 1550 4800 30  0000 C CNN
F 2 "" H 1550 4700 60  0000 C CNN
F 3 "" H 1550 4700 60  0000 C CNN
	1    1550 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 537C3F60
P 4550 3350
F 0 "#PWR023" H 4550 3350 30  0001 C CNN
F 1 "GND" H 4550 3280 30  0001 C CNN
F 2 "" H 4550 3350 60  0000 C CNN
F 3 "" H 4550 3350 60  0000 C CNN
	1    4550 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 537C3F78
P 7550 4400
F 0 "#PWR024" H 7550 4400 30  0001 C CNN
F 1 "GND" H 7550 4330 30  0001 C CNN
F 2 "" H 7550 4400 60  0000 C CNN
F 3 "" H 7550 4400 60  0000 C CNN
	1    7550 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 537C3F89
P 1550 5050
F 0 "#PWR025" H 1550 5050 30  0001 C CNN
F 1 "GND" H 1550 4980 30  0001 C CNN
F 2 "" H 1550 5050 60  0000 C CNN
F 3 "" H 1550 5050 60  0000 C CNN
	1    1550 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 537C3F9A
P 6700 5600
F 0 "#PWR026" H 6700 5600 30  0001 C CNN
F 1 "GND" H 6700 5530 30  0001 C CNN
F 2 "" H 6700 5600 60  0000 C CNN
F 3 "" H 6700 5600 60  0000 C CNN
	1    6700 5600
	1    0    0    -1  
$EndComp
Text Notes 1900 1700 0    40   ~ 0
From "L70 hardware design".\n\n"L70&L76 reference design" shows\nadditional matching circuit.
$Comp
L GND #PWR027
U 1 1 537C41D6
P 1650 2150
F 0 "#PWR027" H 1650 2150 30  0001 C CNN
F 1 "GND" H 1650 2080 30  0001 C CNN
F 2 "" H 1650 2150 60  0000 C CNN
F 3 "" H 1650 2150 60  0000 C CNN
	1    1650 2150
	1    0    0    -1  
$EndComp
$Comp
L MAX2659 U5
U 1 1 537C45EB
P 2900 1850
F 0 "U5" H 3500 2000 60  0000 C CNN
F 1 "MAX2659" H 3500 1300 60  0000 C CNN
F 2 "" H 2900 1850 60  0000 C CNN
F 3 "" H 2900 1850 60  0000 C CNN
	1    2900 1850
	1    0    0    -1  
$EndComp
$Comp
L RES R10
U 1 1 537C46E1
P 2250 1850
F 0 "R10" H 2220 1910 39  0000 C CNN
F 1 "0" H 2250 1850 30  0000 C CNN
F 2 "" H 2250 1850 60  0000 C CNN
F 3 "" H 2250 1850 60  0000 C CNN
	1    2250 1850
	1    0    0    -1  
$EndComp
$Comp
L CAP C10
U 1 1 537C472A
P 2050 2000
F 0 "C10" V 2000 2050 39  0000 C CNN
F 1 "NM" V 2100 2070 39  0000 C CNN
F 2 "" H 2050 2000 60  0000 C CNN
F 3 "" H 2050 2000 60  0000 C CNN
	1    2050 2000
	0    1    1    0   
$EndComp
$Comp
L CAP C11
U 1 1 537C4796
P 2450 2000
F 0 "C11" V 2400 2050 39  0000 C CNN
F 1 "NM" V 2500 2070 39  0000 C CNN
F 2 "" H 2450 2000 60  0000 C CNN
F 3 "" H 2450 2000 60  0000 C CNN
	1    2450 2000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR028
U 1 1 537C4830
P 2050 2150
F 0 "#PWR028" H 2050 2150 30  0001 C CNN
F 1 "GND" H 2050 2080 30  0001 C CNN
F 2 "" H 2050 2150 60  0000 C CNN
F 3 "" H 2050 2150 60  0000 C CNN
	1    2050 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 537C4845
P 2450 2150
F 0 "#PWR029" H 2450 2150 30  0001 C CNN
F 1 "GND" H 2450 2080 30  0001 C CNN
F 2 "" H 2450 2150 60  0000 C CNN
F 3 "" H 2450 2150 60  0000 C CNN
	1    2450 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 537C49B0
P 2800 2300
F 0 "#PWR030" H 2800 2300 30  0001 C CNN
F 1 "GND" H 2800 2230 30  0001 C CNN
F 2 "" H 2800 2300 60  0000 C CNN
F 3 "" H 2800 2300 60  0000 C CNN
	1    2800 2300
	1    0    0    -1  
$EndComp
$Comp
L CAP C9
U 1 1 537C4A70
P 4800 1850
F 0 "C9" V 4750 1900 39  0000 C CNN
F 1 "56pF" V 4850 1950 39  0000 C CNN
F 2 "" H 4800 1850 60  0000 C CNN
F 3 "" H 4800 1850 60  0000 C CNN
	1    4800 1850
	1    0    0    -1  
$EndComp
$Comp
L RES R11
U 1 1 537C4AEB
P 4800 2050
F 0 "R11" H 4770 2110 39  0000 C CNN
F 1 "100" H 4800 2050 30  0000 C CNN
F 2 "" H 4800 2050 60  0000 C CNN
F 3 "" H 4800 2050 60  0000 C CNN
	1    4800 2050
	1    0    0    -1  
$EndComp
$Comp
L RES R12
U 1 1 537C4B8B
P 4800 2250
F 0 "R12" H 4770 2310 39  0000 C CNN
F 1 "100" H 4800 2250 30  0000 C CNN
F 2 "" H 4800 2250 60  0000 C CNN
F 3 "" H 4800 2250 60  0000 C CNN
	1    4800 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 537C51C6
P 5000 2900
F 0 "#PWR031" H 5000 2900 30  0001 C CNN
F 1 "GND" H 5000 2830 30  0001 C CNN
F 2 "" H 5000 2900 60  0000 C CNN
F 3 "" H 5000 2900 60  0000 C CNN
	1    5000 2900
	1    0    0    -1  
$EndComp
$Comp
L CAP C16
U 1 1 537C5493
P 4550 2800
F 0 "C16" V 4500 2850 39  0000 C CNN
F 1 "100nF" V 4600 2900 39  0000 C CNN
F 2 "" H 4550 2800 60  0000 C CNN
F 3 "" H 4550 2800 60  0000 C CNN
	1    4550 2800
	0    1    1    0   
$EndComp
$Comp
L CAP C15
U 1 1 537C5505
P 4300 2800
F 0 "C15" V 4250 2850 39  0000 C CNN
F 1 "10uF" V 4350 2870 39  0000 C CNN
F 2 "" H 4300 2800 60  0000 C CNN
F 3 "" H 4300 2800 60  0000 C CNN
	1    4300 2800
	0    1    1    0   
$EndComp
$Comp
L CAP C17
U 1 1 537C56B6
P 4300 3200
F 0 "C17" V 4250 3250 39  0000 C CNN
F 1 "4.7uF" V 4350 3300 39  0000 C CNN
F 2 "" H 4300 3200 60  0000 C CNN
F 3 "" H 4300 3200 60  0000 C CNN
	1    4300 3200
	0    1    1    0   
$EndComp
$Comp
L CAP C18
U 1 1 537C56DE
P 4550 3200
F 0 "C18" V 4500 3250 39  0000 C CNN
F 1 "100nF" V 4600 3300 39  0000 C CNN
F 2 "" H 4550 3200 60  0000 C CNN
F 3 "" H 4550 3200 60  0000 C CNN
	1    4550 3200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR032
U 1 1 537C5AE0
P 4300 2950
F 0 "#PWR032" H 4300 2950 30  0001 C CNN
F 1 "GND" H 4300 2880 30  0001 C CNN
F 2 "" H 4300 2950 60  0000 C CNN
F 3 "" H 4300 2950 60  0000 C CNN
	1    4300 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 537C5B1E
P 4550 2950
F 0 "#PWR033" H 4550 2950 30  0001 C CNN
F 1 "GND" H 4550 2880 30  0001 C CNN
F 2 "" H 4550 2950 60  0000 C CNN
F 3 "" H 4550 2950 60  0000 C CNN
	1    4550 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR034
U 1 1 537C5B3A
P 4300 3350
F 0 "#PWR034" H 4300 3350 30  0001 C CNN
F 1 "GND" H 4300 3280 30  0001 C CNN
F 2 "" H 4300 3350 60  0000 C CNN
F 3 "" H 4300 3350 60  0000 C CNN
	1    4300 3350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR035
U 1 1 537C5D39
P 4200 3100
F 0 "#PWR035" H 4200 3200 30  0001 C CNN
F 1 "VCC" H 4200 3200 30  0000 C CNN
F 2 "" H 4200 3100 60  0000 C CNN
F 3 "" H 4200 3100 60  0000 C CNN
	1    4200 3100
	0    -1   -1   0   
$EndComp
Text Notes 3000 3450 0    40   ~ 0
Startup time = 200us\nSEL0=1, SEL1=0
Text GLabel 7000 1850 2    40   Output ~ 0
GPS_TXD
Text GLabel 7000 1950 2    40   Input ~ 0
GPS_RXD
Text GLabel 5250 3850 2    40   Input ~ 0
GPS_EN
Text GLabel 7000 2150 2    40   Input ~ 0
GPS_STDBY
Text GLabel 7000 2250 2    40   Input ~ 0
GPS_RST
Text GLabel 7000 2050 2    40   Output ~ 0
GPS_PPS
Text Notes 8100 4650 0    40   ~ 0
I2C address = b'1001010
$Comp
L CAP C19
U 1 1 537CF6B1
P 7550 4150
F 0 "C19" V 7500 4200 39  0000 C CNN
F 1 "1uF" V 7600 4250 39  0000 C CNN
F 2 "" H 7550 4150 60  0000 C CNN
F 3 "" H 7550 4150 60  0000 C CNN
	1    7550 4150
	0    1    1    0   
$EndComp
$Comp
L CAP C25
U 1 1 537CF915
P 7000 5450
F 0 "C25" V 6950 5500 39  0000 C CNN
F 1 "100nF" V 7050 5550 39  0000 C CNN
F 2 "" H 7000 5450 60  0000 C CNN
F 3 "" H 7000 5450 60  0000 C CNN
	1    7000 5450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR036
U 1 1 537D003D
P 9000 4400
F 0 "#PWR036" H 9000 4400 30  0001 C CNN
F 1 "GND" H 9000 4330 30  0001 C CNN
F 2 "" H 9000 4400 60  0000 C CNN
F 3 "" H 9000 4400 60  0000 C CNN
	1    9000 4400
	1    0    0    -1  
$EndComp
Text GLabel 9250 4250 2    40   Output ~ 0
LS_INT_N
$Comp
L CAP C26
U 1 1 537D047D
P 7300 5650
F 0 "C26" V 7250 5700 39  0000 C CNN
F 1 "100nF" V 7350 5750 39  0000 C CNN
F 2 "" H 7300 5650 60  0000 C CNN
F 3 "" H 7300 5650 60  0000 C CNN
	1    7300 5650
	0    1    1    0   
$EndComp
$Comp
L CAP C24
U 1 1 537D04F1
P 6700 5450
F 0 "C24" V 6650 5500 39  0000 C CNN
F 1 "10uF" V 6750 5550 39  0000 C CNN
F 2 "" H 6700 5450 60  0000 C CNN
F 3 "" H 6700 5450 60  0000 C CNN
	1    6700 5450
	0    1    1    0   
$EndComp
Text GLabel 9000 5500 2    40   Output ~ 0
PS_INT1
Text GLabel 9000 5600 2    40   Output ~ 0
PS_INT2
$Comp
L CAP C22
U 1 1 537D1905
P 2100 5000
F 0 "C22" V 2050 5050 39  0000 C CNN
F 1 "100nF" V 2150 5100 39  0000 C CNN
F 2 "" H 2100 5000 60  0000 C CNN
F 3 "" H 2100 5000 60  0000 C CNN
	1    2100 5000
	0    1    1    0   
$EndComp
$Comp
L CAP C21
U 1 1 537D193B
P 1800 4900
F 0 "C21" V 1750 4950 39  0000 C CNN
F 1 "100nF" V 1850 5000 39  0000 C CNN
F 2 "" H 1800 4900 60  0000 C CNN
F 3 "" H 1800 4900 60  0000 C CNN
	1    1800 4900
	0    1    1    0   
$EndComp
$Comp
L CAP C20
U 1 1 537D1970
P 1550 4900
F 0 "C20" V 1500 4950 39  0000 C CNN
F 1 "10uF" V 1600 5000 39  0000 C CNN
F 2 "" H 1550 4900 60  0000 C CNN
F 3 "" H 1550 4900 60  0000 C CNN
	1    1550 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	1600 1850 2150 1850
Wire Wire Line
	2050 1850 2050 1900
Connection ~ 2050 1850
Wire Wire Line
	2050 2100 2050 2150
Wire Wire Line
	2350 1850 2900 1850
Wire Wire Line
	2450 1850 2450 1900
Connection ~ 2450 1850
Wire Wire Line
	2450 2100 2450 2150
Wire Wire Line
	2900 2050 2800 2050
Wire Wire Line
	2800 2050 2800 2300
Wire Wire Line
	2900 2250 2800 2250
Connection ~ 2800 2250
Wire Wire Line
	4100 1850 4700 1850
Wire Wire Line
	4100 2050 4700 2050
Wire Wire Line
	4100 2250 4700 2250
Wire Wire Line
	4900 2250 5100 2250
Wire Wire Line
	4900 2050 5100 2050
Wire Wire Line
	4900 1850 5100 1850
Wire Wire Line
	5000 2650 5000 2900
Wire Wire Line
	5000 2650 5100 2650
Wire Wire Line
	5100 2750 5000 2750
Connection ~ 5000 2750
Wire Wire Line
	5100 2850 5000 2850
Connection ~ 5000 2850
Wire Wire Line
	4300 2900 4300 2950
Wire Wire Line
	4550 2900 4550 2950
Wire Wire Line
	4300 3300 4300 3350
Wire Wire Line
	4550 3300 4550 3350
Wire Wire Line
	4900 3100 4900 2550
Wire Wire Line
	4900 2550 5100 2550
Connection ~ 4550 3100
Connection ~ 4300 2700
Connection ~ 4300 3100
Connection ~ 4550 2700
Wire Wire Line
	6550 1950 7000 1950
Wire Wire Line
	6550 1850 7000 1850
Wire Wire Line
	6550 2350 6650 2350
Wire Wire Line
	6650 2350 6650 3600
Wire Wire Line
	6550 2250 7000 2250
Wire Wire Line
	6550 2150 7000 2150
Wire Wire Line
	6550 2050 7000 2050
Wire Wire Line
	7550 4000 7550 4050
Wire Wire Line
	7550 4050 7800 4050
Wire Wire Line
	7550 4250 7550 4400
Wire Wire Line
	7550 4250 7800 4250
Wire Wire Line
	7800 4350 7550 4350
Connection ~ 7550 4350
Wire Wire Line
	8900 4050 9850 4050
Wire Wire Line
	8900 4150 9850 4150
Wire Wire Line
	8900 4250 9250 4250
Wire Wire Line
	6700 5250 6700 5350
Wire Wire Line
	6700 5300 7700 5300
Connection ~ 6700 5300
Wire Wire Line
	7000 5350 7000 5300
Connection ~ 7000 5300
Wire Wire Line
	7700 5400 7600 5400
Wire Wire Line
	7600 5400 7600 5300
Connection ~ 7600 5300
Wire Wire Line
	7300 5550 7300 5500
Wire Wire Line
	7300 5500 7700 5500
Wire Wire Line
	7600 5600 7700 5600
Wire Wire Line
	7300 5750 7300 5800
Wire Wire Line
	7000 5550 7000 5600
Wire Wire Line
	6700 5550 6700 5600
Wire Wire Line
	8850 5300 9650 5300
Wire Wire Line
	8850 5400 9750 5400
Wire Wire Line
	8850 5500 9000 5500
Wire Wire Line
	8850 5600 9000 5600
Wire Wire Line
	7600 5650 7600 5600
$Comp
L GND #PWR037
U 1 1 537D094C
P 7600 5650
F 0 "#PWR037" H 7600 5650 30  0001 C CNN
F 1 "GND" H 7600 5580 30  0001 C CNN
F 2 "" H 7600 5650 60  0000 C CNN
F 3 "" H 7600 5650 60  0000 C CNN
	1    7600 5650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR038
U 1 1 537D08FC
P 7300 5800
F 0 "#PWR038" H 7300 5800 30  0001 C CNN
F 1 "GND" H 7300 5730 30  0001 C CNN
F 2 "" H 7300 5800 60  0000 C CNN
F 3 "" H 7300 5800 60  0000 C CNN
	1    7300 5800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR039
U 1 1 537D224A
P 7000 5600
F 0 "#PWR039" H 7000 5600 30  0001 C CNN
F 1 "GND" H 7000 5530 30  0001 C CNN
F 2 "" H 7000 5600 60  0000 C CNN
F 3 "" H 7000 5600 60  0000 C CNN
	1    7000 5600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR040
U 1 1 537D2691
P 2650 5400
F 0 "#PWR040" H 2650 5400 30  0001 C CNN
F 1 "GND" H 2650 5330 30  0001 C CNN
F 2 "" H 2650 5400 60  0000 C CNN
F 3 "" H 2650 5400 60  0000 C CNN
	1    2650 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5250 2650 5400
Wire Wire Line
	2650 5250 2750 5250
Wire Wire Line
	2750 5350 2650 5350
Connection ~ 2650 5350
$Comp
L CAP C23
U 1 1 537D27F3
P 2450 5100
F 0 "C23" V 2400 5150 39  0000 C CNN
F 1 "100nF" V 2500 5200 39  0000 C CNN
F 2 "" H 2450 5100 60  0000 C CNN
F 3 "" H 2450 5100 60  0000 C CNN
	1    2450 5100
	0    1    1    0   
$EndComp
$Comp
L GND #PWR041
U 1 1 537D2826
P 2450 5250
F 0 "#PWR041" H 2450 5250 30  0001 C CNN
F 1 "GND" H 2450 5180 30  0001 C CNN
F 2 "" H 2450 5250 60  0000 C CNN
F 3 "" H 2450 5250 60  0000 C CNN
	1    2450 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5200 2450 5250
Wire Wire Line
	2450 5000 2450 4950
Wire Wire Line
	2450 4950 2750 4950
$Comp
L GND #PWR042
U 1 1 537D2A44
P 2100 5150
F 0 "#PWR042" H 2100 5150 30  0001 C CNN
F 1 "GND" H 2100 5080 30  0001 C CNN
F 2 "" H 2100 5150 60  0000 C CNN
F 3 "" H 2100 5150 60  0000 C CNN
	1    2100 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 5100 2100 5150
Wire Wire Line
	2100 4700 2100 4900
Wire Wire Line
	2100 4850 2750 4850
Wire Wire Line
	1800 4800 1800 4750
Wire Wire Line
	1550 4750 2750 4750
$Comp
L VCC #PWR043
U 1 1 537D2E88
P 2100 4700
F 0 "#PWR043" H 2100 4800 30  0001 C CNN
F 1 "VCC" H 2100 4800 30  0000 C CNN
F 2 "" H 2100 4700 60  0000 C CNN
F 3 "" H 2100 4700 60  0000 C CNN
	1    2100 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 4700 1550 4800
Connection ~ 1800 4750
Connection ~ 1550 4750
$Comp
L GND #PWR044
U 1 1 537D3021
P 1800 5050
F 0 "#PWR044" H 1800 5050 30  0001 C CNN
F 1 "GND" H 1800 4980 30  0001 C CNN
F 2 "" H 1800 5050 60  0000 C CNN
F 3 "" H 1800 5050 60  0000 C CNN
	1    1800 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 5000 1800 5050
Wire Wire Line
	1550 5000 1550 5050
Connection ~ 2100 4850
Text GLabel 4450 4750 2    40   Input ~ 0
AC_CLK
Text Notes 8000 5900 0    40   ~ 0
I2C address = b'110000
Text GLabel 9850 4050 2    40   Input ~ 0
SCL
Text GLabel 9850 4150 2    40   BiDi ~ 0
SDA
Wire Wire Line
	9650 5300 9650 4050
Connection ~ 9650 4050
Wire Wire Line
	9750 5400 9750 4150
Connection ~ 9750 4150
Text GLabel 4450 4850 2    40   BiDi ~ 0
AC_SDIO
Text GLabel 4450 4950 2    40   Input ~ 0
AC_CS_XL
Text GLabel 4450 5050 2    40   Input ~ 0
AC_CS_MAG
Text GLabel 4450 5150 2    40   Output ~ 0
AC_INT_XL
Text GLabel 4450 5250 2    40   Output ~ 0
AC_INT_MAG
Text GLabel 4450 5350 2    40   Output ~ 0
AC_DRDY_MAG
Wire Wire Line
	4350 4750 4450 4750
Wire Wire Line
	4350 4850 4450 4850
Wire Wire Line
	4450 4950 4350 4950
Wire Wire Line
	4350 5050 4450 5050
Wire Wire Line
	4450 5150 4350 5150
Wire Wire Line
	4350 5250 4450 5250
Wire Wire Line
	4450 5350 4350 5350
Text Notes 6800 2550 0    40   ~ 0
GPS_RST and GPS_STDBY \nare pulled up internally,\npins must be configured as\nopen-drain in the MCU
Text Notes 600  1200 0    50   ~ 0
Copyright Julian Lewis 2014.\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\nYou may redistribute and modify this documentation under the terms of the\nCERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is distributed\nWITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF MERCHANTABILITY,\nSATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.\nPlease see the CERN OHL v.1.2 for applicable conditions
Wire Notes Line
	550  550  550  1250
Wire Notes Line
	550  1250 3700 1250
Wire Notes Line
	3700 1250 3700 550 
Wire Notes Line
	3700 550  550  550 
$Comp
L ADP198 U9
U 1 1 537E7255
P 2900 2700
F 0 "U9" H 3300 2850 60  0000 C CNN
F 1 "ADP198" H 3300 2150 60  0000 C CNN
F 2 "" H 3700 2700 60  0000 C CNN
F 3 "" H 3700 2700 60  0000 C CNN
	1    2900 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2700 4800 2700
Wire Wire Line
	4200 3100 4900 3100
Wire Wire Line
	4800 2700 4800 2450
Wire Wire Line
	4800 2450 5100 2450
$Comp
L GND #PWR045
U 1 1 537E89FC
P 2800 3150
F 0 "#PWR045" H 2800 3150 30  0001 C CNN
F 1 "GND" H 2800 3080 30  0001 C CNN
F 2 "" H 2800 3150 60  0000 C CNN
F 3 "" H 2800 3150 60  0000 C CNN
	1    2800 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3000 2800 3000
Wire Wire Line
	2800 3000 2800 3150
Wire Wire Line
	2900 3100 2800 3100
Connection ~ 2800 3100
Wire Wire Line
	2800 2650 2800 2800
Wire Wire Line
	2800 2800 2900 2800
Wire Wire Line
	2900 2700 2800 2700
Connection ~ 2800 2700
Wire Wire Line
	4750 3600 4750 3850
Connection ~ 4750 3600
Wire Wire Line
	5250 3850 5050 3850
Wire Wire Line
	6650 3600 5050 3600
$Comp
L GND #PWR046
U 1 1 537E939F
P 3900 3050
F 0 "#PWR046" H 3900 3050 30  0001 C CNN
F 1 "GND" H 3900 2980 30  0001 C CNN
F 2 "" H 3900 3050 60  0000 C CNN
F 3 "" H 3900 3050 60  0000 C CNN
	1    3900 3050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR047
U 1 1 537E942B
P 3900 2850
F 0 "#PWR047" H 3900 2950 30  0001 C CNN
F 1 "VCC" H 3900 2950 30  0000 C CNN
F 2 "" H 3900 2850 60  0000 C CNN
F 3 "" H 3900 2850 60  0000 C CNN
	1    3900 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2800 3800 2800
Wire Wire Line
	3800 2800 3800 2700
Connection ~ 3800 2700
Wire Wire Line
	3800 3600 4850 3600
Wire Wire Line
	3800 3600 3800 3100
Wire Wire Line
	3800 3100 3700 3100
Wire Wire Line
	3700 3000 3900 3000
Wire Wire Line
	3900 3000 3900 3050
Wire Wire Line
	3700 2900 3900 2900
Wire Wire Line
	3900 2900 3900 2850
$Comp
L RES R13
U 1 1 537EA484
P 5250 3450
F 0 "R13" H 5220 3510 39  0000 C CNN
F 1 "470k" H 5250 3450 30  0000 C CNN
F 2 "" H 5250 3450 60  0000 C CNN
F 3 "" H 5250 3450 60  0000 C CNN
	1    5250 3450
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR048
U 1 1 537EA55A
P 5250 3300
F 0 "#PWR048" H 5250 3400 30  0001 C CNN
F 1 "VCC" H 5250 3400 30  0000 C CNN
F 2 "" H 5250 3300 60  0000 C CNN
F 3 "" H 5250 3300 60  0000 C CNN
	1    5250 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3300 5250 3350
Wire Wire Line
	5250 3550 5250 3600
Connection ~ 5250 3600
Connection ~ 7950 2700
Wire Wire Line
	8050 2700 7950 2700
Wire Wire Line
	7950 2600 8050 2600
Wire Wire Line
	7950 2600 7950 2750
$Comp
L GND #PWR049
U 1 1 537EEDBD
P 7950 2750
F 0 "#PWR049" H 7950 2750 30  0001 C CNN
F 1 "GND" H 7950 2680 30  0001 C CNN
F 2 "" H 7950 2750 60  0000 C CNN
F 3 "" H 7950 2750 60  0000 C CNN
	1    7950 2750
	1    0    0    -1  
$EndComp
Connection ~ 7950 2300
Wire Wire Line
	7950 2400 8050 2400
Wire Wire Line
	7950 2300 8050 2300
Wire Wire Line
	7950 2250 7950 2400
Wire Wire Line
	8950 2850 9500 2850
Wire Wire Line
	8950 2700 8950 2850
Wire Wire Line
	8850 2700 8950 2700
Wire Wire Line
	9250 2550 9250 2600
Connection ~ 9250 2300
Wire Wire Line
	9250 2300 9250 2350
Connection ~ 8950 2300
Wire Wire Line
	8950 2400 8950 2300
Wire Wire Line
	8850 2400 8950 2400
Wire Wire Line
	9050 2500 9050 2450
Wire Wire Line
	8850 2500 9050 2500
Wire Wire Line
	9050 2600 9050 2650
Wire Wire Line
	8850 2600 9050 2600
$Comp
L VCC #PWR050
U 1 1 537EE3EE
P 9050 2450
F 0 "#PWR050" H 9050 2550 30  0001 C CNN
F 1 "VCC" H 9050 2550 30  0000 C CNN
F 2 "" H 9050 2450 60  0000 C CNN
F 3 "" H 9050 2450 60  0000 C CNN
	1    9050 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR051
U 1 1 537EE3E8
P 9050 2650
F 0 "#PWR051" H 9050 2650 30  0001 C CNN
F 1 "GND" H 9050 2580 30  0001 C CNN
F 2 "" H 9050 2650 60  0000 C CNN
F 3 "" H 9050 2650 60  0000 C CNN
	1    9050 2650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR052
U 1 1 537ED9B9
P 9250 2600
F 0 "#PWR052" H 9250 2600 30  0001 C CNN
F 1 "GND" H 9250 2530 30  0001 C CNN
F 2 "" H 9250 2600 60  0000 C CNN
F 3 "" H 9250 2600 60  0000 C CNN
	1    9250 2600
	-1   0    0    -1  
$EndComp
$Comp
L CAP C14
U 1 1 537ED7ED
P 9250 2450
F 0 "C14" V 9200 2500 39  0000 C CNN
F 1 "100nF" V 9300 2550 39  0000 C CNN
F 2 "" H 9250 2450 60  0000 C CNN
F 3 "" H 9250 2450 60  0000 C CNN
	1    9250 2450
	0    1    1    0   
$EndComp
$Comp
L ADP198 U8
U 1 1 537E740A
P 8050 2300
F 0 "U8" H 8450 2450 60  0000 C CNN
F 1 "ADP198" H 8450 1750 60  0000 C CNN
F 2 "" H 8850 2300 60  0000 C CNN
F 3 "" H 8850 2300 60  0000 C CNN
	1    8050 2300
	1    0    0    -1  
$EndComp
Text GLabel 9500 2850 2    40   Input ~ 0
SD_EN
Wire Wire Line
	10150 2600 9800 2600
Wire Wire Line
	10050 2500 10050 2750
Wire Wire Line
	10150 2500 10050 2500
Wire Wire Line
	10150 2400 9800 2400
Wire Wire Line
	8850 2300 10150 2300
Wire Wire Line
	10150 2200 9800 2200
Wire Wire Line
	10150 2100 9800 2100
Text GLabel 9800 2100 0    40   Input ~ 0
SD_CS_N
Text GLabel 9800 2400 0    40   Input ~ 0
SD_CLK
Text GLabel 9800 2200 0    40   Input ~ 0
SD_MOSI
Text GLabel 9800 2600 0    40   Output ~ 0
SD_MISO
$Comp
L GND #PWR053
U 1 1 537C3FAB
P 10050 2750
F 0 "#PWR053" H 10050 2750 30  0001 C CNN
F 1 "GND" H 10050 2680 30  0001 C CNN
F 2 "" H 10050 2750 60  0000 C CNN
F 3 "" H 10050 2750 60  0000 C CNN
	1    10050 2750
	-1   0    0    -1  
$EndComp
$Comp
L VCC #PWR054
U 1 1 537C3F07
P 7950 2250
F 0 "#PWR054" H 7950 2350 30  0001 C CNN
F 1 "VCC" H 7950 2350 30  0000 C CNN
F 2 "" H 7950 2250 60  0000 C CNN
F 3 "" H 7950 2250 60  0000 C CNN
	1    7950 2250
	-1   0    0    -1  
$EndComp
$Comp
L MICRO_SDCARD_SPI U7
U 1 1 537A7FBA
P 10150 2000
F 0 "U7" H 10700 2150 60  0000 C CNN
F 1 "MICRO_SDCARD_SPI" H 10500 1150 60  0000 C CNN
F 2 "" H 10150 2000 60  0000 C CNN
F 3 "" H 10150 2000 60  0000 C CNN
	1    10150 2000
	1    0    0    -1  
$EndComp
$Comp
L DIODE_SKY D2
U 1 1 537F01F9
P 4950 3850
F 0 "D2" H 4950 3925 39  0000 C CNN
F 1 "CDBU0130R" H 4950 3775 30  0000 C CNN
F 2 "" H 4950 3850 60  0000 C CNN
F 3 "" H 4950 3850 60  0000 C CNN
	1    4950 3850
	-1   0    0    1   
$EndComp
$Comp
L DIODE_SKY D1
U 1 1 537F02DF
P 4950 3600
F 0 "D1" H 4950 3675 39  0000 C CNN
F 1 "CDBU0130R" H 4950 3525 30  0000 C CNN
F 2 "" H 4950 3600 60  0000 C CNN
F 3 "" H 4950 3600 60  0000 C CNN
	1    4950 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	4750 3850 4850 3850
$Comp
L CAP C13
U 1 1 53820B46
P 4550 2400
F 0 "C13" V 4500 2450 39  0000 C CNN
F 1 "33nF" V 4600 2500 39  0000 C CNN
F 2 "" H 4550 2400 60  0000 C CNN
F 3 "" H 4550 2400 60  0000 C CNN
	1    4550 2400
	0    1    1    0   
$EndComp
$Comp
L CAP C12
U 1 1 53820BAE
P 4300 2400
F 0 "C12" V 4250 2450 39  0000 C CNN
F 1 "100pF" V 4350 2500 39  0000 C CNN
F 2 "" H 4300 2400 60  0000 C CNN
F 3 "" H 4300 2400 60  0000 C CNN
	1    4300 2400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR055
U 1 1 53820F79
P 4300 2550
F 0 "#PWR055" H 4300 2550 30  0001 C CNN
F 1 "GND" H 4300 2480 30  0001 C CNN
F 2 "" H 4300 2550 60  0000 C CNN
F 3 "" H 4300 2550 60  0000 C CNN
	1    4300 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR056
U 1 1 53820FB1
P 4550 2550
F 0 "#PWR056" H 4550 2550 30  0001 C CNN
F 1 "GND" H 4550 2480 30  0001 C CNN
F 2 "" H 4550 2550 60  0000 C CNN
F 3 "" H 4550 2550 60  0000 C CNN
	1    4550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2500 4300 2550
Wire Wire Line
	4550 2500 4550 2550
Wire Wire Line
	4550 2250 4550 2300
Connection ~ 4550 2250
Wire Wire Line
	4300 2250 4300 2300
Connection ~ 4300 2250
$Comp
L W3011A ANT1
U 1 1 53821E01
P 1600 1850
F 0 "ANT1" H 2050 2000 60  0000 C CNN
F 1 "W3011A" H 2000 1500 60  0000 C CNN
F 2 "" H 1600 1850 60  0000 C CNN
F 3 "" H 1600 1850 60  0000 C CNN
	1    1600 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1600 2050 1650 2050
Wire Wire Line
	1650 1950 1650 2150
Wire Wire Line
	1600 1950 1650 1950
Connection ~ 1650 2050
$Comp
L CAP C27
U 1 1 53831B91
P 4100 6850
F 0 "C27" V 4050 6900 39  0000 C CNN
F 1 "100nF" V 4150 6950 39  0000 C CNN
F 2 "" H 4100 6850 60  0000 C CNN
F 3 "" H 4100 6850 60  0000 C CNN
	1    4100 6850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR057
U 1 1 53831B98
P 3800 7050
F 0 "#PWR057" H 3800 7050 30  0001 C CNN
F 1 "GND" H 3800 6980 30  0001 C CNN
F 2 "" H 3800 7050 60  0000 C CNN
F 3 "" H 3800 7050 60  0000 C CNN
	1    3800 7050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR058
U 1 1 53831B9E
P 3450 6800
F 0 "#PWR058" H 3450 6900 30  0001 C CNN
F 1 "VCC" H 3450 6900 30  0000 C CNN
F 2 "" H 3450 6800 60  0000 C CNN
F 3 "" H 3450 6800 60  0000 C CNN
	1    3450 6800
	1    0    0    -1  
$EndComp
Text Notes 3300 6400 0    60   ~ 12
Optional vibrating motor
$Comp
L CONN_2 P6
U 1 1 53831BA5
P 4800 6850
F 0 "P6" V 4750 6850 40  0000 C CNN
F 1 "CONN_2" V 4850 6850 40  0000 C CNN
F 2 "" H 4800 6850 60  0000 C CNN
F 3 "" H 4800 6850 60  0000 C CNN
	1    4800 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 7050 3800 6950
Connection ~ 4100 6950
Connection ~ 3850 6950
Wire Wire Line
	3800 6950 4450 6950
Wire Wire Line
	3800 6750 4450 6750
Connection ~ 4100 6750
Wire Wire Line
	3800 6650 3800 6750
Connection ~ 3850 6750
$Comp
L ADP198 U13
U 1 1 53831BBB
P 2450 6650
F 0 "U13" H 2850 6800 60  0000 C CNN
F 1 "ADP198" H 2850 6100 60  0000 C CNN
F 2 "" H 3250 6650 60  0000 C CNN
F 3 "" H 3250 6650 60  0000 C CNN
	1    2450 6650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR059
U 1 1 53831BC2
P 3450 7000
F 0 "#PWR059" H 3450 7000 30  0001 C CNN
F 1 "GND" H 3450 6930 30  0001 C CNN
F 2 "" H 3450 7000 60  0000 C CNN
F 3 "" H 3450 7000 60  0000 C CNN
	1    3450 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 6600 2350 6750
Wire Wire Line
	2350 6750 2450 6750
Wire Wire Line
	2450 6650 2350 6650
Connection ~ 2350 6650
Wire Wire Line
	2450 6950 2350 6950
Wire Wire Line
	2350 6950 2350 7100
Wire Wire Line
	2450 7050 2350 7050
Connection ~ 2350 7050
Wire Wire Line
	3250 6850 3450 6850
Wire Wire Line
	3450 6850 3450 6800
Wire Wire Line
	3250 6950 3450 6950
Wire Wire Line
	3450 6950 3450 7000
Wire Wire Line
	3250 6750 3350 6750
Wire Wire Line
	3350 6750 3350 6650
Wire Wire Line
	3250 6650 3800 6650
Connection ~ 3350 6650
$Comp
L VCC #PWR060
U 1 1 53831BD8
P 2350 6600
F 0 "#PWR060" H 2350 6700 30  0001 C CNN
F 1 "VCC" H 2350 6700 30  0000 C CNN
F 2 "" H 2350 6600 60  0000 C CNN
F 3 "" H 2350 6600 60  0000 C CNN
	1    2350 6600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR061
U 1 1 53831BDE
P 2350 7100
F 0 "#PWR061" H 2350 7100 30  0001 C CNN
F 1 "GND" H 2350 7030 30  0001 C CNN
F 2 "" H 2350 7100 60  0000 C CNN
F 3 "" H 2350 7100 60  0000 C CNN
	1    2350 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 7050 3350 7050
Wire Wire Line
	3350 7050 3350 7300
Wire Wire Line
	3350 7300 3650 7300
Text GLabel 3650 7300 2    40   Input ~ 0
VM_EN
$Comp
L DIODE_SKY D3
U 1 1 538345B8
P 3850 6850
F 0 "D3" V 3900 6775 39  0000 C CNN
F 1 "CDBU0130R" V 4000 6750 30  0000 C CNN
F 2 "" H 3850 6850 60  0000 C CNN
F 3 "" H 3850 6850 60  0000 C CNN
	1    3850 6850
	0    -1   -1   0   
$EndComp
Text Notes 3150 4450 0    60   ~ 12
3D accelerometer and compass
Text Notes 4450 1450 0    60   ~ 12
GPS module
Text Notes 8950 1800 0    60   ~ 12
Micro SD card slot
Text Notes 7900 3750 0    60   ~ 12
Ambient light sensor
Text Notes 7100 5050 0    60   ~ 12
Barometric pressure sensor and thermometer
Text Notes 8150 3050 0    40   ~ 0
Startup time = 200us\nSEL0=1, SEL1=0
Text Notes 2550 7400 0    40   ~ 0
Startup time = 200us\nSEL0=1, SEL1=0
Wire Wire Line
	8900 4350 9000 4350
Wire Wire Line
	9000 4350 9000 4400
$EndSCHEMATC
