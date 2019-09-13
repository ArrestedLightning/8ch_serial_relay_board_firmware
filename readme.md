8 Channel Relay Board Firmware
===============================

This is a basic Arduino firmware for an inexpensive 8-channel, 12V RS232 relay board I got on eBay for about 20 USD.  When I got the board, there was a program on it, but there was no documentation provided on how to communicate with it.  The eBay seller did not have any information on the protocol, so I wrote my own firmware.  The board is labeled "Made for FKECL", "8Relay Module 232", "I/O Drive Controller".  The board is based on an ATMega8 running at 8MHz; the firwmare should be built for an Arduino NG board.  Since that board is supposed to be running at 16MHz, all the timing functions in the code will be running at 1/2 speed.  I uploaded the firmware using an AVR Pocket Programmer from Sparkfun.  The board does have a programming header, but unfortunately it does not appear to be a standard AVR programming header.

According to my notes, the pinout on my board was as follows:

     ________
    |        |
    | 1    2 |
    | 3    4 |
    | 5    6 |
    |________|
	
	1. SCK
	2. MISO
	3. MOSI
	4. RST
	5. VCC
	6. GND
	
Pin 1 is indicated by a square pad on the PCB.
	
I wired it up to my programmer using male-female dupont wire jumpers.  In theory, you could also flash the Arduino bootloader so that the fimware can be updated in the future over serial.

The firmware operates at 9600 baud and uses a simple protocol.  Packets are started with a '<' character.  The desired relay number (1-8, in binary, not ASCII) is then repeated 3 times, followed by the desired relay state (0 or 1, also in binary) repeated 3 times.  The packet is then concluded with a '>' character.  All three relay numbers and states must match for the relays to change states.  This forms a simple, but effective, method of preventing noise on the communications channel from incorrectly switching an undesired relay.  The firmware will return a either a 0 or 1 byte to indicate failure or success, respectively.  There is plenty of room for expansion and improvement, but this was all I needed for my application.  It has been working without any issues for over 4 years now.  

Hopefully this is useful for someone.  Pull requests and comments are welcome.