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
L USB-MICRO-AB CON?
U 1 1 537A6938
P 2950 2250
F 0 "CON?" H 2400 2400 60  0000 C CNN
F 1 "USB-MICRO-AB" H 2400 1600 60  0000 C CNN
F 2 "" H 2400 1950 60  0000 C CNN
F 3 "" H 2400 1950 60  0000 C CNN
	1    2950 2250
	1    0    0    -1  
$EndComp
$Comp
L TPS780180300 U?
U 1 1 537A6A66
P 7500 2500
F 0 "U?" H 8050 2650 60  0000 C CNN
F 1 "TPS780180300" H 8050 2050 60  0000 C CNN
F 2 "" H 7500 2500 60  0000 C CNN
F 3 "" H 7500 2500 60  0000 C CNN
	1    7500 2500
	1    0    0    -1  
$EndComp
$Comp
L bq24072 U?
U 1 1 537A6AF5
P 4900 2350
F 0 "U?" H 5450 2500 60  0000 C CNN
F 1 "bq24072" H 5500 1200 60  0000 C CNN
F 2 "" H 7050 2450 60  0000 C CNN
F 3 "" H 7050 2450 60  0000 C CNN
	1    4900 2350
	1    0    0    -1  
$EndComp
$Comp
L MAX17047 U?
U 1 1 537A6B46
P 4900 4350
F 0 "U?" H 6000 3650 60  0000 C CNN
F 1 "MAX17047" H 5450 4500 60  0000 C CNN
F 2 "" H 5550 4500 60  0000 C CNN
F 3 "" H 5550 4500 60  0000 C CNN
	1    4900 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 537A6D7D
P 3050 2800
F 0 "#PWR?" H 3050 2800 30  0001 C CNN
F 1 "GND" H 3050 2730 30  0001 C CNN
F 2 "" H 3050 2800 60  0000 C CNN
F 3 "" H 3050 2800 60  0000 C CNN
	1    3050 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 537A6D91
P 1750 2800
F 0 "#PWR?" H 1750 2800 30  0001 C CNN
F 1 "GND" H 1750 2730 30  0001 C CNN
F 2 "" H 1750 2800 60  0000 C CNN
F 3 "" H 1750 2800 60  0000 C CNN
	1    1750 2800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 537A6DD4
P 3050 2600
F 0 "#PWR?" H 3050 2690 20  0001 C CNN
F 1 "+5V" H 3050 2690 30  0000 C CNN
F 2 "" H 3050 2600 60  0000 C CNN
F 3 "" H 3050 2600 60  0000 C CNN
	1    3050 2600
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR?
U 1 1 537A6E38
P 3050 4600
F 0 "#PWR?" H 3050 4550 20  0001 C CNN
F 1 "+BATT" H 3050 4700 30  0000 C CNN
F 2 "" H 3050 4600 60  0000 C CNN
F 3 "" H 3050 4600 60  0000 C CNN
	1    3050 4600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 537A6E4C
P 9300 2350
F 0 "#PWR?" H 9300 2450 30  0001 C CNN
F 1 "VCC" H 9300 2450 30  0000 C CNN
F 2 "" H 9300 2350 60  0000 C CNN
F 3 "" H 9300 2350 60  0000 C CNN
	1    9300 2350
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P?
U 1 1 537A6E8C
P 2700 4850
F 0 "P?" V 2650 4850 40  0000 C CNN
F 1 "CONN_2" V 2750 4850 40  0000 C CNN
F 2 "" H 2700 4850 60  0000 C CNN
F 3 "" H 2700 4850 60  0000 C CNN
	1    2700 4850
	-1   0    0    1   
$EndComp
Wire Wire Line
	1850 2750 1750 2750
Wire Wire Line
	1750 2450 1750 2800
Wire Wire Line
	2950 2750 3050 2750
Wire Wire Line
	3050 2750 3050 2800
Wire Wire Line
	2950 2650 3050 2650
Wire Wire Line
	3050 2650 3050 2600
Wire Wire Line
	1850 2450 1750 2450
Connection ~ 1750 2750
Wire Wire Line
	1850 2650 1750 2650
Connection ~ 1750 2650
Wire Wire Line
	1850 2550 1750 2550
Connection ~ 1750 2550
Wire Wire Line
	2950 2250 3700 2250
Wire Wire Line
	2950 2350 3700 2350
Text GLabel 3700 2250 2    47   BiDi ~ 0
USB_DP
Text GLabel 3700 2350 2    47   BiDi ~ 0
USB_DM
Text Notes 2750 4600 2    60   ~ 0
Battery connection
$Comp
L C C?
U 1 1 537A8547
P 6950 3850
F 0 "C?" H 6950 3950 40  0000 L CNN
F 1 "C" H 6956 3765 40  0000 L CNN
F 2 "" H 6988 3700 30  0000 C CNN
F 3 "" H 6950 3850 60  0000 C CNN
	1    6950 3850
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 537A867E
P 5550 5750
F 0 "R?" V 5630 5750 40  0000 C CNN
F 1 "R" V 5557 5751 40  0000 C CNN
F 2 "" V 5480 5750 30  0000 C CNN
F 3 "" H 5550 5750 30  0000 C CNN
	1    5550 5750
	0    1    1    0   
$EndComp
$EndSCHEMATC
