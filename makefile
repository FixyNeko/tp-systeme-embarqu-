COMPILATEUR = gcc
FLAG = -Wall -Wextra 
C_FILE = main.c tcp.c uart.c

reseau : $(C_FILE)
	$(COMPILATEUR) $(C_FILE) -o reseau $(FLAG)


	
	
