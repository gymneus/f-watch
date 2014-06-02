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
Sheet 2 4
Title "Freewatch USB & Power supply"
Date ""
Rev "v1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L USB-MICRO-AB P1
U 1 1 537A6938
P 3150 2550
F 0 "P1" H 2600 2700 60  0000 C CNN
F 1 "USB-MICRO-AB" H 2600 1900 60  0000 C CNN
F 2 "" H 2600 2250 60  0000 C CNN
F 3 "" H 2600 2250 60  0000 C CNN
	1    3150 2550
	1    0    0    -1  
$EndComp
$Comp
L TPS780180300 U3
U 1 1 537A6A66
P 8250 3350
F 0 "U3" H 8800 3500 60  0000 C CNN
F 1 "TPS780180300" H 8800 2800 60  0000 C CNN
F 2 "" H 8250 3350 60  0000 C CNN
F 3 "" H 8250 3350 60  0000 C CNN
	1    8250 3350
	1    0    0    -1  
$EndComp
$Comp
L bq24072 U2
U 1 1 537A6AF5
P 4950 2950
F 0 "U2" H 5500 3100 60  0000 C CNN
F 1 "bq24072" H 5550 1800 60  0000 C CNN
F 2 "" H 7100 3050 60  0000 C CNN
F 3 "" H 7100 3050 60  0000 C CNN
	1    4950 2950
	1    0    0    -1  
$EndComp
$Comp
L MAX17047 U4
U 1 1 537A6B46
P 4950 5100
F 0 "U4" H 6050 4400 60  0000 C CNN
F 1 "MAX17047" H 5500 5250 60  0000 C CNN
F 2 "" H 5600 5250 60  0000 C CNN
F 3 "" H 5600 5250 60  0000 C CNN
	1    4950 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 537A6D7D
P 3250 3100
F 0 "#PWR01" H 3250 3100 30  0001 C CNN
F 1 "GND" H 3250 3030 30  0001 C CNN
F 2 "" H 3250 3100 60  0000 C CNN
F 3 "" H 3250 3100 60  0000 C CNN
	1    3250 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 537A6D91
P 1950 3100
F 0 "#PWR02" H 1950 3100 30  0001 C CNN
F 1 "GND" H 1950 3030 30  0001 C CNN
F 2 "" H 1950 3100 60  0000 C CNN
F 3 "" H 1950 3100 60  0000 C CNN
	1    1950 3100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 537A6DD4
P 3400 2900
F 0 "#PWR03" H 3400 2990 20  0001 C CNN
F 1 "+5V" H 3400 2990 30  0000 C CNN
F 2 "" H 3400 2900 60  0000 C CNN
F 3 "" H 3400 2900 60  0000 C CNN
	1    3400 2900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR04
U 1 1 537A6E4C
P 10150 3300
F 0 "#PWR04" H 10150 3400 30  0001 C CNN
F 1 "VCC" H 10150 3400 30  0000 C CNN
F 2 "" H 10150 3300 60  0000 C CNN
F 3 "" H 10150 3300 60  0000 C CNN
	1    10150 3300
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 537A6E8C
P 2200 5200
F 0 "P3" V 2150 5200 40  0000 C CNN
F 1 "CONN_2" V 2250 5200 40  0000 C CNN
F 2 "" H 2200 5200 60  0000 C CNN
F 3 "" H 2200 5200 60  0000 C CNN
	1    2200 5200
	-1   0    0    -1  
$EndComp
Text GLabel 4250 2550 2    40   BiDi ~ 0
USB_DP
Text GLabel 4250 2650 2    40   BiDi ~ 0
USB_DM
Text Notes 4200 4000 0    40   ~ 0
NTC
$Comp
L GND #PWR05
U 1 1 537B8F5D
P 6250 4000
F 0 "#PWR05" H 6250 4000 30  0001 C CNN
F 1 "GND" H 6250 3930 30  0001 C CNN
F 2 "" H 6250 4000 60  0000 C CNN
F 3 "" H 6250 4000 60  0000 C CNN
	1    6250 4000
	1    0    0    -1  
$EndComp
Text Notes 6450 4100 0    40   ~ 0
When EN1=0, EN2=1\nI_IN_MAX = K_ILIM / R_ILIM\nK_ILIM = 1550 A*ohm
$Comp
L GND #PWR06
U 1 1 537BA5A0
P 4550 3200
F 0 "#PWR06" H 4550 3200 30  0001 C CNN
F 1 "GND" H 4550 3130 30  0001 C CNN
F 2 "" H 4550 3200 60  0000 C CNN
F 3 "" H 4550 3200 60  0000 C CNN
	1    4550 3200
	1    0    0    -1  
$EndComp
$Comp
L RES R9
U 1 1 537BAF4C
P 5600 6350
F 0 "R9" H 5570 6410 39  0000 C CNN
F 1 "10m" H 5600 6350 30  0000 C CNN
F 2 "" H 5600 6350 60  0000 C CNN
F 3 "" H 5600 6350 60  0000 C CNN
	1    5600 6350
	1    0    0    -1  
$EndComp
$Comp
L CAP C1
U 1 1 537BB217
P 4550 3050
F 0 "C1" V 4500 3100 39  0000 C CNN
F 1 "1uF" V 4600 3120 39  0000 C CNN
F 2 "" H 4550 3050 60  0000 C CNN
F 3 "" H 4550 3050 60  0000 C CNN
	1    4550 3050
	0    1    1    0   
$EndComp
$Comp
L CAP C2
U 1 1 537BB33E
P 6700 3450
F 0 "C2" V 6650 3500 39  0000 C CNN
F 1 "4.7uF" V 6750 3550 39  0000 C CNN
F 2 "" H 6700 3450 60  0000 C CNN
F 3 "" H 6700 3450 60  0000 C CNN
	1    6700 3450
	0    1    1    0   
$EndComp
$Comp
L CAP C6
U 1 1 537BB3B2
P 3950 5700
F 0 "C6" V 3900 5750 39  0000 C CNN
F 1 "100nF" V 4000 5800 39  0000 C CNN
F 2 "" H 3950 5700 60  0000 C CNN
F 3 "" H 3950 5700 60  0000 C CNN
	1    3950 5700
	0    1    1    0   
$EndComp
$Comp
L RES R6
U 1 1 537BB40F
P 4850 4050
F 0 "R6" H 4820 4110 39  0000 C CNN
F 1 "RES" H 4850 4050 30  0000 C CNN
F 2 "" H 4850 4050 60  0000 C CNN
F 3 "" H 4850 4050 60  0000 C CNN
	1    4850 4050
	0    -1   -1   0   
$EndComp
$Comp
L RES R5
U 1 1 537BB487
P 4700 4050
F 0 "R5" H 4670 4110 39  0000 C CNN
F 1 "RES" H 4700 4050 30  0000 C CNN
F 2 "" H 4700 4050 60  0000 C CNN
F 3 "" H 4700 4050 60  0000 C CNN
	1    4700 4050
	0    -1   -1   0   
$EndComp
$Comp
L RES R4
U 1 1 537BB4BE
P 4550 4050
F 0 "R4" H 4520 4110 39  0000 C CNN
F 1 "RES" H 4550 4050 30  0000 C CNN
F 2 "" H 4550 4050 60  0000 C CNN
F 3 "" H 4550 4050 60  0000 C CNN
	1    4550 4050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR07
U 1 1 537BB757
P 4350 4250
F 0 "#PWR07" H 4350 4250 30  0001 C CNN
F 1 "GND" H 4350 4180 30  0001 C CNN
F 2 "" H 4350 4250 60  0000 C CNN
F 3 "" H 4350 4250 60  0000 C CNN
	1    4350 4250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 537BBADB
P 6400 6450
F 0 "#PWR08" H 6400 6450 30  0001 C CNN
F 1 "GND" H 6400 6380 30  0001 C CNN
F 2 "" H 6400 6450 60  0000 C CNN
F 3 "" H 6400 6450 60  0000 C CNN
	1    6400 6450
	1    0    0    -1  
$EndComp
$Comp
L RES R8
U 1 1 537BC060
P 4600 5300
F 0 "R8" H 4570 5360 39  0000 C CNN
F 1 "10k" H 4600 5300 30  0000 C CNN
F 2 "" H 4600 5300 60  0000 C CNN
F 3 "" H 4600 5300 60  0000 C CNN
	1    4600 5300
	0    -1   -1   0   
$EndComp
$Comp
L CAP C7
U 1 1 537BC192
P 4600 5950
F 0 "C7" V 4550 6000 39  0000 C CNN
F 1 "10nF" V 4650 6050 39  0000 C CNN
F 2 "" H 4600 5950 60  0000 C CNN
F 3 "" H 4600 5950 60  0000 C CNN
	1    4600 5950
	0    1    1    0   
$EndComp
$Comp
L CAP C8
U 1 1 537BC112
P 4850 5950
F 0 "C8" V 4800 6000 39  0000 C CNN
F 1 "100nF" V 4900 6050 39  0000 C CNN
F 2 "" H 4850 5950 60  0000 C CNN
F 3 "" H 4850 5950 60  0000 C CNN
	1    4850 5950
	0    1    1    0   
$EndComp
Text Notes 2150 5000 0    100  ~ 20
+
Text Notes 2150 5500 0    100  ~ 20
-
$Comp
L GND #PWR09
U 1 1 537BDAA5
P 6700 3600
F 0 "#PWR09" H 6700 3600 30  0001 C CNN
F 1 "GND" H 6700 3530 30  0001 C CNN
F 2 "" H 6700 3600 60  0000 C CNN
F 3 "" H 6700 3600 60  0000 C CNN
	1    6700 3600
	1    0    0    -1  
$EndComp
$Comp
L CAP C3
U 1 1 537BDBFA
P 7850 3450
F 0 "C3" V 7800 3500 39  0000 C CNN
F 1 "1uF" V 7900 3550 39  0000 C CNN
F 2 "" H 7850 3450 60  0000 C CNN
F 3 "" H 7850 3450 60  0000 C CNN
	1    7850 3450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR010
U 1 1 537BDC3C
P 7850 3600
F 0 "#PWR010" H 7850 3600 30  0001 C CNN
F 1 "GND" H 7850 3530 30  0001 C CNN
F 2 "" H 7850 3600 60  0000 C CNN
F 3 "" H 7850 3600 60  0000 C CNN
	1    7850 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 537BDD25
P 8150 3800
F 0 "#PWR011" H 8150 3800 30  0001 C CNN
F 1 "GND" H 8150 3730 30  0001 C CNN
F 2 "" H 8150 3800 60  0000 C CNN
F 3 "" H 8150 3800 60  0000 C CNN
	1    8150 3800
	1    0    0    -1  
$EndComp
$Comp
L CAP C4
U 1 1 537BE25D
P 10150 3500
F 0 "C4" V 10100 3550 39  0000 C CNN
F 1 "4.7uF" V 10200 3600 39  0000 C CNN
F 2 "" H 10150 3500 60  0000 C CNN
F 3 "" H 10150 3500 60  0000 C CNN
	1    10150 3500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR012
U 1 1 537BE5A4
P 10150 3650
F 0 "#PWR012" H 10150 3650 30  0001 C CNN
F 1 "GND" H 10150 3580 30  0001 C CNN
F 2 "" H 10150 3650 60  0000 C CNN
F 3 "" H 10150 3650 60  0000 C CNN
	1    10150 3650
	1    0    0    -1  
$EndComp
Text GLabel 4250 2750 2    40   Output ~ 0
USB_ID
Text Notes 6250 5700 0    40   ~ 0
I2C address = b'0110110
$Comp
L RES R7
U 1 1 537C006A
P 7050 5200
F 0 "R7" H 7020 5260 39  0000 C CNN
F 1 "200k" H 7050 5200 30  0000 C CNN
F 2 "" H 7050 5200 60  0000 C CNN
F 3 "" H 7050 5200 60  0000 C CNN
	1    7050 5200
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR013
U 1 1 537C0183
P 7050 5050
F 0 "#PWR013" H 7050 5150 30  0001 C CNN
F 1 "VCC" H 7050 5150 30  0000 C CNN
F 2 "" H 7050 5050 60  0000 C CNN
F 3 "" H 7050 5050 60  0000 C CNN
	1    7050 5050
	1    0    0    -1  
$EndComp
Text GLabel 7800 5350 2    40   Output ~ 0
FG_ALRT_N
Text GLabel 7800 5450 2    40   BiDi ~ 0
SDA
Text GLabel 7800 5550 2    40   Input ~ 0
SCL
$Comp
L RES R1
U 1 1 537C0F32
P 6300 2750
F 0 "R1" H 6270 2810 39  0000 C CNN
F 1 "100k" H 6300 2750 30  0000 C CNN
F 2 "" H 6300 2750 60  0000 C CNN
F 3 "" H 6300 2750 60  0000 C CNN
	1    6300 2750
	0    -1   -1   0   
$EndComp
$Comp
L RES R2
U 1 1 537C12D1
P 6500 2750
F 0 "R2" H 6470 2810 39  0000 C CNN
F 1 "100k" H 6500 2750 30  0000 C CNN
F 2 "" H 6500 2750 60  0000 C CNN
F 3 "" H 6500 2750 60  0000 C CNN
	1    6500 2750
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR014
U 1 1 537C1C5E
P 6300 2550
F 0 "#PWR014" H 6300 2650 30  0001 C CNN
F 1 "VCC" H 6300 2650 30  0000 C CNN
F 2 "" H 6300 2550 60  0000 C CNN
F 3 "" H 6300 2550 60  0000 C CNN
	1    6300 2550
	1    0    0    -1  
$EndComp
Text GLabel 6900 2950 2    40   Output ~ 0
BC_PGOOD_N
Text GLabel 6900 3050 2    40   Output ~ 0
BC_CHG_N
Text Notes 3050 4000 0    40   ~ 0
t_PRECHG = K_TMR * R_TMR\nt_MAXCHG = 10 * K_TMR * R_TMR\nK_TMR = 48 s/kohm
Text Notes 3050 4200 0    40   ~ 0
I_CHG = K_ISET / R_ISET\nK_ISET = 890 A*ohm
Text Notes 10350 3250 0    50   ~ 10
VCC = 3V
Text Notes 600  1200 0    50   ~ 0
Copyright Julian Lewis 2014.\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\nYou may redistribute and modify this documentation under the terms of the\nCERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is distributed\nWITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF MERCHANTABILITY,\nSATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.\nPlease see the CERN OHL v.1.2 for applicable conditions
$Comp
L PRTR5V0U2X U1
U 1 1 537DA889
P 3800 1650
F 0 "U1" H 4300 1800 60  0000 C CNN
F 1 "PRTR5V0U2X" H 4300 1000 60  0000 C CNN
F 2 "" H 3800 1450 60  0000 C CNN
F 3 "" H 3800 1450 60  0000 C CNN
	1    3800 1650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR015
U 1 1 537DA930
P 3750 1600
F 0 "#PWR015" H 3750 1690 20  0001 C CNN
F 1 "+5V" H 3750 1690 30  0000 C CNN
F 2 "" H 3750 1600 60  0000 C CNN
F 3 "" H 3750 1600 60  0000 C CNN
	1    3750 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 537DA953
P 3750 2200
F 0 "#PWR016" H 3750 2200 30  0001 C CNN
F 1 "GND" H 3750 2130 30  0001 C CNN
F 2 "" H 3750 2200 60  0000 C CNN
F 3 "" H 3750 2200 60  0000 C CNN
	1    3750 2200
	1    0    0    -1  
$EndComp
$Comp
L CAP C5
U 1 1 537DB009
P 3400 3500
F 0 "C5" V 3350 3550 39  0000 C CNN
F 1 "4.7uF" V 3450 3600 39  0000 C CNN
F 2 "" H 3400 3500 60  0000 C CNN
F 3 "" H 3400 3500 60  0000 C CNN
	1    3400 3500
	0    1    1    0   
$EndComp
$Comp
L +BATT #PWR017
U 1 1 537DB368
P 2800 5050
F 0 "#PWR017" H 2800 5000 20  0001 C CNN
F 1 "+BATT" H 2800 5150 30  0000 C CNN
F 2 "" H 2800 5050 60  0000 C CNN
F 3 "" H 2800 5050 60  0000 C CNN
	1    2800 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3050 2050 3050
Wire Wire Line
	1950 2750 1950 3100
Wire Wire Line
	3150 3050 3250 3050
Wire Wire Line
	3250 3050 3250 3100
Wire Wire Line
	3150 2950 4950 2950
Wire Wire Line
	2050 2750 1950 2750
Connection ~ 1950 3050
Wire Wire Line
	2050 2950 1950 2950
Connection ~ 1950 2950
Wire Wire Line
	2050 2850 1950 2850
Connection ~ 1950 2850
Wire Wire Line
	3150 2550 4250 2550
Wire Wire Line
	3150 2650 4250 2650
Wire Wire Line
	6150 3650 6250 3650
Wire Wire Line
	6250 3650 6250 4000
Wire Wire Line
	6150 3950 6250 3950
Connection ~ 6250 3950
Wire Wire Line
	6450 3850 6150 3850
Wire Wire Line
	6150 3350 8250 3350
Wire Wire Line
	6150 3750 6250 3750
Connection ~ 6250 3750
Wire Wire Line
	2950 6350 5500 6350
Wire Wire Line
	5700 6350 6400 6350
Wire Wire Line
	5800 6150 5800 6350
Connection ~ 5800 6350
Wire Wire Line
	6400 6350 6400 6450
Wire Wire Line
	4350 4200 4850 4200
Wire Wire Line
	4850 4200 4850 4150
Connection ~ 4550 4200
Wire Wire Line
	4700 4150 4700 4200
Connection ~ 4700 4200
Wire Wire Line
	4850 3950 4950 3950
Wire Wire Line
	4700 3950 4700 3850
Wire Wire Line
	4700 3850 4950 3850
Wire Wire Line
	4550 3950 4550 3750
Wire Wire Line
	4550 3750 4950 3750
Wire Wire Line
	2550 5100 4950 5100
Wire Wire Line
	4950 5200 4600 5200
Wire Wire Line
	4400 5400 4950 5400
Wire Wire Line
	4600 5400 4600 5850
Connection ~ 4600 6350
Wire Wire Line
	4850 6050 4850 6350
Connection ~ 4850 6350
Wire Wire Line
	4350 3550 4950 3550
Wire Wire Line
	4950 3450 4850 3450
Wire Wire Line
	4850 3450 4850 3350
Wire Wire Line
	2950 3350 4950 3350
Connection ~ 4850 3350
Wire Wire Line
	4550 3200 4550 3150
Wire Wire Line
	4550 3150 4950 3150
Connection ~ 4600 5400
Wire Wire Line
	6700 3550 6700 3600
Wire Wire Line
	7850 3550 7850 3600
Wire Wire Line
	8250 3650 8150 3650
Wire Wire Line
	8150 3650 8150 3800
Wire Wire Line
	9350 3350 10150 3350
Wire Wire Line
	8250 3550 8150 3550
Wire Wire Line
	8150 3550 8150 3350
Connection ~ 8150 3350
Wire Wire Line
	10150 3300 10150 3400
Connection ~ 10150 3350
Wire Wire Line
	10150 3600 10150 3650
Wire Wire Line
	4850 5100 4850 4800
Wire Wire Line
	4850 4800 6350 4800
Wire Wire Line
	6350 4800 6350 5100
Wire Wire Line
	6350 5100 6250 5100
Connection ~ 4850 5100
Wire Wire Line
	3150 2750 4250 2750
Wire Wire Line
	3400 2900 3400 2950
Connection ~ 3400 2950
Wire Wire Line
	4950 5600 4850 5600
Wire Wire Line
	4850 5600 4850 5850
Wire Wire Line
	6250 5350 7800 5350
Wire Wire Line
	6250 5450 7800 5450
Wire Wire Line
	6250 5550 7800 5550
Wire Wire Line
	7050 5050 7050 5100
Wire Wire Line
	7050 5300 7050 5350
Connection ~ 7050 5350
Wire Wire Line
	6450 3350 6450 3850
Connection ~ 6450 3350
Wire Wire Line
	6150 3450 6450 3450
Connection ~ 6450 3450
Wire Wire Line
	6150 2950 6900 2950
Wire Wire Line
	6150 3050 6900 3050
Wire Wire Line
	6300 2550 6300 2650
Wire Wire Line
	6300 2600 6500 2600
Wire Wire Line
	6500 2600 6500 2650
Connection ~ 6300 2600
Wire Wire Line
	6300 2850 6300 2950
Connection ~ 6300 2950
Wire Wire Line
	6500 2850 6500 3050
Connection ~ 6500 3050
Wire Wire Line
	5600 6150 5600 6200
Wire Wire Line
	5600 6200 5400 6200
Wire Wire Line
	5400 6150 5400 6350
Connection ~ 5400 6350
Connection ~ 5400 6200
Connection ~ 4550 2950
Connection ~ 6700 3350
Connection ~ 7850 3350
Wire Notes Line
	550  550  550  1250
Wire Notes Line
	550  1250 3700 1250
Wire Notes Line
	3700 1250 3700 550 
Wire Notes Line
	3700 550  550  550 
Wire Wire Line
	3800 2150 3750 2150
Wire Wire Line
	3750 2150 3750 2200
Wire Wire Line
	3800 1650 3750 1650
Wire Wire Line
	3750 1650 3750 1600
Wire Wire Line
	3800 1850 3400 1850
Wire Wire Line
	3400 1850 3400 2550
Connection ~ 3400 2550
Wire Wire Line
	3800 1950 3500 1950
Wire Wire Line
	3500 1950 3500 2650
Connection ~ 3500 2650
Wire Wire Line
	2950 3350 2950 5100
Connection ~ 2950 5100
Wire Wire Line
	2950 5300 2950 6350
Connection ~ 3950 6350
Wire Wire Line
	2550 5300 2950 5300
Wire Wire Line
	3950 5600 3950 5100
Connection ~ 3950 5100
Wire Wire Line
	3950 5800 3950 6350
Wire Wire Line
	2800 5050 2800 5100
Connection ~ 2800 5100
Wire Wire Line
	4600 6050 4600 6350
Text Notes 5200 2650 0    60   ~ 12
Battery charger
Text Notes 8350 3050 0    60   ~ 12
LDO voltage regulator
Text Notes 5250 4700 0    60   ~ 12
Battery fuel gauge
Text Notes 2200 2250 0    60   ~ 12
Micro USB connector
Text Notes 1700 4800 0    60   ~ 12
Battery connector
$Comp
L GND #PWR018
U 1 1 538727A8
P 3400 3650
F 0 "#PWR018" H 3400 3650 30  0001 C CNN
F 1 "GND" H 3400 3580 30  0001 C CNN
F 2 "" H 3400 3650 60  0000 C CNN
F 3 "" H 3400 3650 60  0000 C CNN
	1    3400 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3350 3400 3400
Connection ~ 3400 3350
Wire Wire Line
	3400 3600 3400 3650
Wire Wire Line
	4950 3250 4850 3250
Wire Wire Line
	4850 3250 4850 3150
Connection ~ 4850 3150
Wire Wire Line
	8250 3750 8150 3750
Connection ~ 8150 3750
$Comp
L RES R27
U 1 1 53876EE2
P 4350 4050
F 0 "R27" H 4320 4110 39  0000 C CNN
F 1 "10k" H 4350 4050 30  0000 C CNN
F 2 "" H 4350 4050 60  0000 C CNN
F 3 "" H 4350 4050 60  0000 C CNN
	1    4350 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4350 3550 4350 3950
Wire Wire Line
	4350 4150 4350 4250
Wire Wire Line
	4550 4150 4550 4200
Connection ~ 4350 4200
$Comp
L RES R28
U 1 1 538775F2
P 4400 5950
F 0 "R28" H 4370 6010 39  0000 C CNN
F 1 "10k" H 4400 5950 30  0000 C CNN
F 2 "" H 4400 5950 60  0000 C CNN
F 3 "" H 4400 5950 60  0000 C CNN
	1    4400 5950
	0    -1   -1   0   
$EndComp
Text Notes 4250 5900 0    40   ~ 0
NTC
Wire Wire Line
	4400 5400 4400 5850
Wire Wire Line
	4400 6050 4400 6350
Connection ~ 4400 6350
$Comp
L GND #PWR019
U 1 1 537BE071
P 9450 3700
F 0 "#PWR019" H 9450 3700 30  0001 C CNN
F 1 "GND" H 9450 3630 30  0001 C CNN
F 2 "" H 9450 3700 60  0000 C CNN
F 3 "" H 9450 3700 60  0000 C CNN
	1    9450 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3550 9450 3550
Wire Wire Line
	9450 3550 9450 3700
Wire Wire Line
	9350 3650 9450 3650
Connection ~ 9450 3650
Text Notes 8350 4100 0    60   ~ 0
TPS73430 is pin compatible and 250mA
$EndSCHEMATC