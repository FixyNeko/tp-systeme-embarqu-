COMPILATEUR = gcc
FLAG = -Wall -Wextra -pthread -g
C_FILE = main.c tcp.c led.c

all: pc rasp

pc : $(C_FILE)
	$(COMPILATEUR) $(C_FILE) -o pc $(FLAG)

rasp: $(C_FILE)
	$(COMPILATEUR) -D RASP $(C_FILE) -o rasp $(FLAG)
	
	
