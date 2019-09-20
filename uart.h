#ifndef UART_H
#define UART_H

/*
 * This function is to help you to open the UART connection between the PC and the Raspberry Pi
 * Its parameter is the UART device file, like "/dev/ttyAMA0" (pi) or "/dev/ttyUSB0" (pc)
 * It returns a file descriptor that you can use with open(2), write(2), close(2), ... (see `man 2 open`, etc)
 * If it returns -1, that means it has failed
 */

int uart_open(char const*filename);

#endif
