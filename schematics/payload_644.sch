v 20110115 2
C 45700 43300 1 0 0 ATmega644P-dip.sym
{
T 46100 51000 5 10 0 0 0 0 1
device=Atmega644P
T 48800 48900 5 10 1 1 0 0 1
refdes=U1
T 45700 43300 5 10 0 0 0 0 1
footprint=DIP40
}
C 57100 49900 1 180 0 modular_8p8c.sym
{
T 57000 45800 5 10 0 0 180 0 1
footprint=modular_8p8c_lp.fp
T 55900 47100 5 10 1 1 0 0 1
refdes=X1
}
C 51800 45100 1 0 0 max489-1.sym
{
T 53450 47400 5 10 0 0 0 0 1
device=MAX489
T 53050 45650 5 10 1 1 0 0 1
refdes=U2
T 53450 47200 5 10 0 0 0 0 1
footprint=DIP14
}
C 51900 43800 1 270 0 crystal-1.sym
{
T 52400 43600 5 10 0 0 270 0 1
device=CRYSTAL
T 52200 43600 5 10 1 1 270 0 1
refdes=U3
T 52600 43600 5 10 0 0 270 0 1
symversion=0.1
T 51700 43900 5 10 1 1 0 0 1
value=12MHz
T 51900 43800 5 10 0 0 0 0 1
footprint=CRYSTAL 300
}
C 52500 43600 1 0 0 capacitor-1.sym
{
T 52700 44300 5 10 0 0 0 0 1
device=CAPACITOR
T 52700 44100 5 10 1 1 0 0 1
refdes=C2
T 52700 44500 5 10 0 0 0 0 1
symversion=0.1
T 53100 43900 5 10 1 1 0 0 1
value=22pF
T 52500 43600 5 10 0 1 0 0 1
footprint=AXIAL_LAY 200
}
C 52500 42900 1 0 0 capacitor-1.sym
{
T 52700 43600 5 10 0 0 0 0 1
device=CAPACITOR
T 52700 43400 5 10 1 1 0 0 1
refdes=C1
T 52700 43800 5 10 0 0 0 0 1
symversion=0.1
T 53100 43200 5 10 1 1 0 0 1
value=22pF
T 52500 42900 5 10 0 1 0 0 1
footprint=AXIAL_LAY 200
}
N 52600 47600 52600 48000 4
N 50600 48000 53000 48000 4
N 49400 48200 50600 48200 4
N 50600 46200 50600 48600 4
N 54400 49400 54000 49400 4
N 54000 42900 54000 49800 4
N 54000 49800 54400 49800 4
N 49400 45200 53000 45200 4
N 49400 45200 49400 45400 4
N 52500 45600 52500 45200 4
N 52700 45600 52700 45200 4
N 49400 43800 52500 43800 4
N 52500 43100 49400 43100 4
N 49400 43100 49400 43600 4
N 53400 43100 54000 43100 4
N 53400 43800 54000 43800 4
N 53300 47100 53800 47100 4
N 53800 47100 53800 47800 4
N 53800 47800 54400 47800 4
N 53300 46700 53500 46700 4
N 53500 46700 53500 49000 4
N 53500 49000 54400 49000 4
N 54200 47400 54200 48500 4
N 53300 46500 53900 46500 4
N 53600 46500 53600 48600 4
N 53600 48600 54400 48600 4
N 53300 46100 53800 46100 4
N 53700 46100 53700 48200 4
N 53700 48200 54400 48200 4
C 53900 42600 1 0 0 gnd-1.sym
C 43700 52000 1 270 0 connector4-1.sym
{
T 44600 50200 5 10 0 0 270 0 1
device=CONNECTOR_4
T 43700 52000 5 10 1 1 0 0 1
refdes=CONN2
T 43700 52000 5 10 0 0 0 0 1
footprint=CONNECTOR 4 1
}
T 45100 51600 9 10 1 0 0 0 1
Camera Header
T 44000 51400 9 10 1 0 270 0 1
Camera RX
T 44300 51400 9 10 1 0 270 0 1
Camera TX
T 44900 51400 9 10 1 0 270 0 1
Camera Vcc
T 44600 51400 9 10 1 0 270 0 1
Camera GND
N 44200 50300 44200 44600 4
N 44200 44600 45800 44600 4
N 43900 44400 43900 50300 4
N 43500 44400 45800 44400 4
N 42600 42800 53500 42800 4
N 53500 42800 53500 43100 4
N 45800 45000 45300 45000 4
N 45300 45000 45300 49300 4
N 45300 49300 51400 49300 4
N 51400 49300 51400 46300 4
N 51400 46300 51800 46300 4
N 51800 46900 50300 46900 4
N 50300 46900 50300 49400 4
N 50300 49400 45200 49400 4
N 45200 49400 45200 44800 4
N 45200 44800 45800 44800 4
N 51800 45200 51800 46100 4
N 51800 46600 51000 46600 4
N 51000 46600 51000 49200 4
N 44900 49200 51000 49200 4
T 39900 45800 9 10 1 0 0 0 2
microSD Card Header
- SPI Bus
T 40500 48300 9 10 1 0 0 0 1
\_CS\_
T 40500 48000 9 10 1 0 0 0 1
DI
T 40500 47700 9 10 1 0 0 0 1
VDD
T 40500 47400 9 10 1 0 0 0 1
CLK
T 40500 47100 9 10 1 0 0 0 1
VSS
T 40500 46800 9 10 1 0 0 0 1
DO
T 40500 46500 9 10 1 0 0 0 1
NC
N 42600 47100 42600 42800 4
N 41600 47100 42600 47100 4
N 41600 48000 45000 48000 4
N 45000 48000 45000 47600 4
N 45000 47600 45800 47600 4
N 41600 48300 45100 48300 4
N 45100 48300 45100 47800 4
N 45100 47800 45800 47800 4
N 44500 50300 44500 44900 4
N 44500 44900 45000 44900 4
N 45000 44900 45000 42800 4
N 41600 47400 44800 47400 4
N 44800 47400 44800 47200 4
N 44800 47200 45800 47200 4
N 45800 47400 45000 47400 4
N 45000 47400 45000 46800 4
N 45000 46800 41600 46800 4
T 55799 47716 9 10 1 0 0 0 1
5V
T 55806 47889 9 10 1 0 0 0 1
3V3
T 55806 48067 9 10 1 0 0 0 1
Y
T 55799 48232 9 10 1 0 0 0 1
B
T 55806 48390 9 10 1 0 0 0 1
A
T 55799 48555 9 10 1 0 0 0 1
Z
T 55799 48734 9 10 1 0 0 0 1
GND
T 55800 48914 9 10 1 0 0 0 1
GND
T 55500 49600 9 10 1 0 0 0 2
Autopilot
Payload Socket
N 50600 48400 49400 48400 4
N 41600 47700 41900 47700 4
N 41900 46500 41900 49700 4
N 41900 49700 53400 49700 4
N 44800 50300 44800 49700 4
N 54400 47400 53400 47400 4
N 53400 47400 53400 49700 4
T 50200 51500 9 10 1 0 0 0 1
\_RESET\_
T 47900 51900 9 10 1 0 0 0 1
SPI_MOSI
T 50200 51900 9 10 1 0 0 0 1
SPI_MISO
T 50200 52300 9 10 1 0 0 0 1
SPI_CLK
N 49400 47600 51500 47600 4
N 51500 47600 51500 51800 4
C 51600 48500 1 0 0 resistor-2.sym
{
T 52000 48850 5 10 0 0 0 0 1
device=RESISTOR
T 51800 48800 5 10 1 1 0 0 1
refdes=R1
T 51800 48300 5 10 1 1 0 0 1
value=10K
T 51600 48500 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
N 51600 48600 51500 48600 4
N 52500 48600 52500 48000 4
C 51700 49100 1 0 0 switch-pushbutton-nc-1.sym
{
T 51250 48900 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NC
T 52300 49350 5 10 1 1 0 0 1
refdes=S1
T 51700 49100 5 10 0 0 0 0 1
footprint=AXIAL_LAY 200
}
N 51700 49200 51500 49200 4
N 52700 49200 54000 49200 4
C 43100 48700 1 90 0 resistor-2.sym
{
T 42750 49100 5 10 0 0 90 0 1
device=RESISTOR
T 42800 48900 5 10 1 1 90 0 1
refdes=R3
T 42600 49500 5 10 1 1 0 0 1
value=68R
T 43100 48700 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
C 43600 48700 1 90 0 resistor-2.sym
{
T 43250 49100 5 10 0 0 90 0 1
device=RESISTOR
T 43300 48900 5 10 1 1 90 0 1
refdes=R4
T 43100 49500 5 10 1 1 0 0 1
value=68R
T 43600 48700 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
C 42500 48700 1 90 0 resistor-2.sym
{
T 42150 49100 5 10 0 0 90 0 1
device=RESISTOR
T 42200 48900 5 10 1 1 90 0 1
refdes=R2
T 42000 49500 5 10 1 1 0 0 1
value=68R
T 42500 48700 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
N 43500 48700 43500 46800 4
N 43000 48700 43000 47400 4
N 42400 48700 42400 48000 4
N 42400 49600 42400 49800 4
N 42400 49800 47600 49800 4
N 47600 51800 48800 51800 4
N 43500 49600 43500 49900 4
N 43500 49900 51000 49900 4
N 51000 49900 51000 51800 4
N 43000 50000 51100 50000 4
N 43000 50000 43000 49600 4
C 54200 46400 1 270 0 capacitor-2.sym
{
T 54900 46200 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 54700 46100 5 10 1 1 0 0 1
refdes=C4
T 55100 46200 5 10 0 0 270 0 1
symversion=0.1
T 54600 45600 5 10 1 1 0 0 1
value=48uF
T 54200 46400 5 10 0 1 0 0 1
footprint=AXIAL_LAY 200
}
C 55000 46400 1 270 0 capacitor-2.sym
{
T 55700 46200 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 55500 46100 5 10 1 1 0 0 1
refdes=C3
T 55900 46200 5 10 0 0 270 0 1
symversion=0.1
T 55400 45600 5 10 1 1 0 0 1
value=48uF
T 55000 46400 5 10 0 1 0 0 1
footprint=AXIAL_LAY 200
}
C 50800 45300 1 90 0 capacitor-1.sym
{
T 50100 45500 5 10 0 0 90 0 1
device=CAPACITOR
T 50900 46100 5 10 1 1 180 0 1
refdes=C6
T 49900 45500 5 10 0 0 90 0 1
symversion=0.1
T 50700 45400 5 10 1 1 0 0 1
value=22nF
T 50800 45300 5 10 0 1 0 0 1
footprint=AXIAL_LAY 200
}
N 50600 45300 50600 45200 4
N 50600 48600 49400 48600 4
N 54100 45300 54100 47400 4
N 54100 46400 54400 46400 4
N 54400 47000 54400 46700 4
N 55200 46400 55200 46700 4
N 55200 46700 54400 46700 4
N 54000 45500 55200 45500 4
C 51400 45300 1 90 0 capacitor-1.sym
{
T 50700 45500 5 10 0 0 90 0 1
device=CAPACITOR
T 51500 46100 5 10 1 1 180 0 1
refdes=C5
T 50500 45500 5 10 0 0 90 0 1
symversion=0.1
T 51300 45400 5 10 1 1 0 0 1
value=22nF
T 51400 45300 5 10 0 1 0 0 1
footprint=AXIAL_LAY 200
}
N 51200 45300 51200 45200 4
N 51200 46200 51200 48000 4
C 53100 50800 1 0 0 header10-2.sym
{
T 53100 52800 5 10 0 1 0 0 1
device=HEADER10
T 53700 52900 5 10 1 1 0 0 1
refdes=J2
T 53100 50800 5 10 0 0 0 0 1
footprint=CONNECTOR 5 2
}
T 53300 50600 9 10 1 0 0 0 1
JTAG header
T 49000 50900 9 10 1 0 0 0 1
ISP header
N 51500 51400 50200 51400 4
N 51500 51800 53100 51800 4
N 47600 51800 47600 49800 4
T 52500 51900 9 10 1 0 0 0 1
\_RESET\_
N 51000 51800 50200 51800 4
N 51100 50000 51100 52200 4
N 51100 52200 50200 52200 4
N 45800 46400 45700 46400 4
N 45700 46400 45700 50100 4
N 45800 46200 45600 46200 4
N 45600 46200 45600 50200 4
N 45800 46000 45500 46000 4
N 45500 46000 45500 50300 4
N 45800 45800 45400 45800 4
N 45400 45800 45400 50400 4
N 51300 48000 51300 50500 4
N 47700 50500 55200 50500 4
N 47700 50500 47700 52200 4
N 53100 52200 52000 52200 4
N 55200 50500 55200 51400 4
N 55200 51400 54500 51400 4
N 44500 49600 52300 49600 4
N 52300 49600 52300 52600 4
N 48800 51400 48600 51400 4
N 48600 51400 48600 49600 4
T 48300 52300 9 10 1 0 0 0 1
VCC
T 52700 52300 9 10 1 0 0 0 1
VCC
T 54600 51500 9 10 1 0 0 0 1
VCC
T 48400 51500 9 10 1 0 0 0 1
GND
T 52700 52700 9 10 1 0 0 0 1
GND
T 52700 51100 9 10 1 0 0 0 1
GND
N 52300 51000 53100 51000 4
N 52300 52600 53100 52600 4
N 45700 50100 55600 50100 4
N 55600 50100 55600 52600 4
N 55600 52600 54500 52600 4
N 45600 50200 55500 50200 4
N 55500 50200 55500 51800 4
N 55500 51800 54500 51800 4
N 45500 50300 55400 50300 4
N 55400 50300 55400 52200 4
N 55400 52200 54500 52200 4
N 45400 50400 55300 50400 4
N 55300 50400 55300 51000 4
N 55300 51000 54500 51000 4
T 54600 52700 9 10 1 0 0 0 1
TCK
T 54600 52300 9 10 1 0 0 0 1
TDO
T 54600 51900 9 10 1 0 0 0 1
TMS
T 54600 51100 9 10 1 0 0 0 1
TDI
T 53500 40000 9 10 1 0 0 0 1
Autopilot Payload based on ATmega644P processor
T 53500 39700 9 10 1 0 0 0 1
payload_644.sch
T 53500 39400 9 10 1 0 0 0 1
1
T 55000 39400 9 10 1 0 0 0 1
1
T 57600 39400 9 10 1 0 0 0 1
Andrew Busse
C 39900 46300 1 0 0 connector8-1.sym
{
T 40000 49500 5 10 0 0 0 0 1
device=CONNECTOR_8
T 40000 48900 5 10 1 1 0 0 1
refdes=CONN1
T 39900 46300 5 10 0 1 0 0 1
footprint=hirose-dm3at.fp
}
T 40500 48600 9 10 1 0 0 0 1
NC
C 48800 51200 1 0 0 header6-2.sym
{
T 48800 52400 5 10 0 1 0 0 1
device=HEADER6
T 49400 52500 5 10 1 1 0 0 1
refdes=J1
T 48800 51200 5 10 0 0 0 0 1
footprint=CONNECTOR 3 2
}
C 45800 41900 1 0 0 led-2.sym
{
T 46600 42100 5 10 1 1 0 0 1
refdes=D1
T 45900 42500 5 10 0 0 0 0 1
device=LED
T 45800 41900 5 10 0 2 0 0 1
footprint=LED3
}
C 45800 41400 1 0 0 led-2.sym
{
T 46600 41600 5 10 1 1 0 0 1
refdes=D2
T 45900 42000 5 10 0 0 0 0 1
device=LED
T 45800 41400 5 10 0 2 0 0 1
footprint=LED3
}
C 45800 40900 1 0 0 led-2.sym
{
T 46600 41100 5 10 1 1 0 0 1
refdes=D3
T 45900 41500 5 10 0 0 0 0 1
device=LED
T 45800 40900 5 10 0 2 0 0 1
footprint=LED3
}
C 45800 40400 1 0 0 led-2.sym
{
T 46600 40600 5 10 1 1 0 0 1
refdes=D4
T 45900 41000 5 10 0 0 0 0 1
device=LED
T 45800 40400 5 10 0 2 0 0 1
footprint=LED3
}
C 47000 41900 1 0 0 resistor-2.sym
{
T 47400 42250 5 10 0 0 0 0 1
device=RESISTOR
T 47800 42100 5 10 1 1 0 0 1
refdes=R5
T 47000 41900 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
C 47000 41400 1 0 0 resistor-2.sym
{
T 47400 41750 5 10 0 0 0 0 1
device=RESISTOR
T 47800 41600 5 10 1 1 0 0 1
refdes=R6
T 47000 41400 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
C 47000 40900 1 0 0 resistor-2.sym
{
T 47400 41250 5 10 0 0 0 0 1
device=RESISTOR
T 47800 41100 5 10 1 1 0 0 1
refdes=R7
T 47000 40900 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
C 47000 40400 1 0 0 resistor-2.sym
{
T 47400 40750 5 10 0 0 0 0 1
device=RESISTOR
T 47800 40600 5 10 1 1 0 0 1
refdes=R8
T 47000 40400 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
N 45800 43600 45800 42000 4
N 45800 43800 45700 43800 4
N 45700 43800 45700 41500 4
N 45700 41500 45800 41500 4
N 45800 44000 45600 44000 4
N 45600 44000 45600 41000 4
N 45600 41000 45800 41000 4
N 45800 44200 45500 44200 4
N 45500 44200 45500 40500 4
N 45500 40500 45800 40500 4
N 46700 42000 47000 42000 4
N 46700 41500 47000 41500 4
N 46700 41000 47000 41000 4
N 46700 40500 47000 40500 4
N 48100 40500 48100 42800 4
N 47900 42000 48100 42000 4
N 47900 41500 48100 41500 4
N 47900 41000 48100 41000 4
N 47900 40500 48100 40500 4
T 46300 42500 9 10 1 0 0 0 1
Debug LEDs
C 54200 45300 1 270 0 led-2.sym
{
T 54400 44400 5 10 1 1 0 0 1
refdes=D5
T 54800 45200 5 10 0 0 270 0 1
device=LED
T 54200 45300 5 10 0 2 0 0 1
footprint=LED3
}
C 54200 44100 1 270 0 resistor-2.sym
{
T 54550 43700 5 10 0 0 270 0 1
device=RESISTOR
T 54500 43400 5 10 1 1 0 0 1
refdes=R9
T 54200 44100 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
N 54300 44400 54300 44100 4
N 54100 45300 54700 45300 4
N 54000 43200 55800 43200 4
T 54700 44000 9 10 1 0 0 0 2
3V3 Power
LED
T 46800 53000 9 10 1 0 0 0 1
Programming Headers
N 44900 49200 44900 46600 4
N 44900 46600 45800 46600 4
N 49400 45800 50200 45800 4
N 50200 42500 50200 45800 4
N 49400 46000 50100 46000 4
N 50100 42400 50100 46000 4
C 51800 40100 1 90 0 connector8-1.sym
{
T 48600 40200 5 10 0 0 90 0 1
device=CONNECTOR_8
T 49200 40500 5 10 1 1 180 0 1
refdes=CONN3
T 51800 40100 5 10 0 1 0 0 1
footprint=CONNECTOR 8 1
}
N 50000 42300 50000 46200 4
N 50000 46200 49400 46200 4
N 49400 46400 49900 46400 4
N 49900 42200 49900 46400 4
N 49400 46600 49800 46600 4
N 49800 42100 49800 46600 4
N 49400 46800 49700 46800 4
N 49700 42000 49700 46800 4
N 49400 47000 49600 47000 4
N 49600 41900 49600 47000 4
N 49400 47200 49500 47200 4
N 49500 41800 49500 47200 4
N 49600 41900 49800 41900 4
N 49800 41900 49800 41800 4
N 49700 42000 50100 42000 4
N 50100 42000 50100 41800 4
N 49800 42100 50400 42100 4
N 50400 42100 50400 41800 4
N 49900 42200 50700 42200 4
N 50700 42200 50700 41800 4
N 50000 42300 51000 42300 4
N 51000 42300 51000 41800 4
N 50100 42400 51300 42400 4
N 51300 42400 51300 41800 4
N 50200 42500 51600 42500 4
N 51600 42500 51600 41800 4
T 52000 40600 9 10 1 0 0 0 3
Port A
Expansion
Header
N 47700 52200 48800 52200 4
N 52000 52200 52000 50500 4
C 37100 39300 1 0 0 title-A2.sym
T 52800 49300 9 10 1 0 0 0 2
Reset
Switch
C 53100 48200 1 270 0 resistor-2.sym
{
T 53450 47800 5 10 0 0 270 0 1
device=RESISTOR
T 52700 47700 5 10 1 1 0 0 1
refdes=R10
T 53100 48200 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
C 53700 45600 1 270 0 resistor-2.sym
{
T 54050 45200 5 10 0 0 270 0 1
device=RESISTOR
T 53200 45200 5 10 1 1 0 0 1
refdes=R11
T 53700 45600 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
N 53200 47300 53400 47300 4
N 53400 47300 53400 47100 4
N 53000 48000 53000 48500 4
N 53000 48500 54200 48500 4
N 53200 48200 53500 48200 4
N 53000 45200 53000 44400 4
N 53000 44400 54000 44400 4
N 53800 45600 53900 45600 4
N 53900 45600 53900 46500 4
N 53800 44700 53600 44700 4
N 53600 44700 53600 45700 4
N 53600 45700 53800 45700 4
N 53800 45700 53800 46100 4
C 57100 45300 1 180 0 connector2-1.sym
{
T 56900 44300 5 10 0 0 180 0 1
device=CONNECTOR_2
T 57100 44500 5 10 1 1 180 0 1
refdes=CONN4
T 57100 45300 5 10 0 0 0 0 1
footprint=CONNECTOR 1 2
}
N 55400 45100 54700 45100 4
N 54700 45100 54700 45300 4
N 55400 44800 55400 44500 4
N 55400 44500 55800 44500 4
N 55800 44500 55800 43200 4
C 43600 45400 1 90 0 resistor-2.sym
{
T 43250 45800 5 10 0 0 90 0 1
device=RESISTOR
T 43300 45700 5 10 1 1 180 0 1
refdes=R11
T 43100 46200 5 10 1 1 0 0 1
value=10k
T 43600 45400 5 10 0 1 0 0 1
footprint=AXIAL_LAY 400
}
N 43500 45400 43500 44400 4
N 43500 46300 43500 46500 4
N 41900 46500 43500 46500 4
