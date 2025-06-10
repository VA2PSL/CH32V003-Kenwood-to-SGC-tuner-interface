Simple interface to control an SGC tuner using the AT-300 interface on the back of Kenwood transceiver using the cheap CH32V003 8 pin microcontroller. I built mine on a small proto board, using a SOIC-8 to pdif pcb.
Any 5V regulator can be used, I just happened to have a strip of AMS1117-5.0 regulators on hand. With simple code modification, this could be adapted to ICOM AH-4 protocol, or used with an LDG tuner. See wagiminator github for
the CH32V003 template used. CH32FUN has all the info to configure GCC and the flash software.
