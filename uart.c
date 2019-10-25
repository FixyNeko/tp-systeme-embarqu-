#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "uart.h"


#include <pthread.h>

void *receive(void * p) {
	int fd2 = uart_open("/dev/ttyS0");
	printf("fd2: %d\n", fd2);
	char buf[1000];
		
	while(1) {
		int ret = read(fd2, buf, 2);
		
		printf("%s ", buf);
		if(ret == -1) printf("(error)");
		else printf("no error");
		printf("\n");
	}
	
	return 0;
	
}

void *transmit(void * p) {
	int fd1 = open("/dev/ttyS0", O_RDWR|O_NONBLOCK);
	printf("fd1: %d\n", fd1);
	
	char c = 'a';
	
	while(1) {
		c = (c + 1 - 'a') % ('z' - 'a') + 'a';
		write(fd1, &c, 1);
		//for(int i = 0; i < 10000000; i++);
	}
	
	return 0;
	
}

int main() {
	pthread_t receiver, transmitter;
	
	//uart_open("/dev/ttyS0");
	
	pthread_create(&transmitter, NULL, transmit, NULL);
	pthread_create(&receiver, NULL, receive, NULL);
	
	while(1);
	
	return 0;
	
}

int uart_open(char const*filename) 
{
	int speed = B115200;

	struct termios options;
	int fd = open(filename, O_RDWR|O_NONBLOCK);
	if(fd < 0) return fd;

	fcntl(fd, F_SETFL, 0);
	tcgetattr(fd, &options);
	usleep(10000);
	cfsetospeed(&options, speed);
	cfsetispeed(&options, speed);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	options.c_oflag &= ~CRTSCTS;
	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ICANON|ECHO|ECHONL|IEXTEN|ISIG);

	options.c_cc[VMIN] = 1;
	options.c_cc[VTIME] = 40;

	tcsetattr(fd, TCSANOW, &options);
	tcflush(fd, TCIOFLUSH);
	usleep(10000);

	return fd;
}
