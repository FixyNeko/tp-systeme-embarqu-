COMPILATEUR = gcc
FLAG = -Wall -Wextra -pthread
C_FILE = main.c tcp.c

reseau : $(C_FILE)
	$(COMPILATEUR) $(C_FILE) -o reseau $(FLAG)


	
	
