#include "led.h"

char ledsNumber[LEDS_NBR][3];
char leds[LEDS_NBR][50];
char ledsState[LEDS_NBR] = {0};

int main(int argc, char ** argv) {
	
	
	init_leds();
	
	for(int i = 0; i < 8; i++) {
		switch_led(ROUGE);
		usleep(100 * 1000);
	}
	
	blink_led((blinkInfo) {.led = JAUNE, .period = 200});
	
	while(1);
	
	return 0;
}

int init_leds() {
	char fileName[50] = {0};
	int status = 0;
	
	strcpy( ledsNumber[ROUGE] , "16");
	strcpy( ledsNumber[VERT]  , "20");
	strcpy( ledsNumber[JAUNE] , "21");
	
	for(int i =  0; i < LEDS_NBR; i++) {
		strcpy(fileName, gpioBaseFolder);
		strcat(fileName, "export");
		
		fd = fopen(fileName, "w");
		if(fd == NULL) {
			perror("Erreur d'ouverture du fichier export\r\n");
			status = 1;
		}
		else {
			if(fputs(ledsNumber[i], fd) == EOF) {
				perror("Erreur d'export d'une led\r\n");
				status = 1;
			}
			else {
				fclose(fd);
				
				strcpy(leds[i], gpioBaseFolder);
				strcat(leds[i], "gpio");
				strcat(leds[i], ledsNumber[i]);
				strcat(leds[i], "/");
				
				strcpy(fileName, leds[i]);
				strcat(fileName, "direction");
				
				fd = fopen(fileName, "w");
				if(fd != NULL) {
					fputs("out", fd);
					fclose(fd);
				}
				
				strcpy(fileName, leds[i]);
				strcat(fileName, "value");
				
				fd = fopen(fileName, "w");
				if(fd != NULL) {
					ledsState[i] = 0;
					fputs("0", fd);
					fclose(fd);
				}
			}
		}
	}
	
	return status;
}

void set_led(LEDS led) {
	char fileName[sizeof(leds[led]) + 5];
	strcpy(fileName, leds[led]);
	strcat(fileName, "value");
	
	fd = fopen(fileName, "w");
	if(fd != NULL) {
		ledsState[led] = 1;
		fputs("1", fd);
		fclose(fd);
	}
}

void clear_led(LEDS led) {
	char fileName[sizeof(leds[led]) + 5];
	strcpy(fileName, leds[led]);
	strcat(fileName, "value");
	
	fd = fopen(fileName, "w");
	if(fd != NULL) {
		ledsState[led] = 0;
		fputs("0", fd);
		fclose(fd);
	}
}

void switch_led(LEDS led) {
	char fileName[sizeof(leds[led]) + 5];
	strcpy(fileName, leds[led]);
	strcat(fileName, "value");
	
	fd = fopen(fileName, "w");
	if(fd != NULL) {
		ledsState[led] = !ledsState[led];
		fputs(ledsState[led]? "1" : "0", fd);
		fclose(fd);
	}
}

void blink_led(blinkInfo info) {
	pthread_t tid;
	
	pthread_create(&tid, NULL, blink_led_thread, &info);
}

void *blink_led_thread(void *i) {
	blinkInfo info = *((blinkInfo*)i);
	
	while(1) {
		switch_led(info.led);
		usleep(info.period * 1000);
	}
}
