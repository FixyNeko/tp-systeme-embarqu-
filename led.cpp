#include "led.hpp"

using std::string;
using std::cout;
using std::endl;

extern std::string ledsNumber[];
extern std::string leds[];
extern bool        ledsState[];

int main(int argc, char ** argv) {
	
	
	init_leds();
	
	for(int i = 0; i < 8; i++) {
		switch_led(ROUGE);
		usleep(100 * 1000);
	}
	
	return 0;
}

int init_leds() {
	int status = 0;
	
	ledsNumber[ROUGE] = "16";
	ledsNumber[VERT]  = "20";
	ledsNumber[JAUNE] = "21";
	
	for(int i =  0; i < LEDS_NBR; i++) {
		fd = fopen((gpioBaseFolder + "export").c_str(), "w");
		if(fd == NULL) {
			std::cerr << "Erreur d'ouverture du fichier export" << std::endl;
			status = 1;
		}
		else {
			if(fputs(string(ledsNumber[i]).c_str(), fd) == EOF) {
				std::cerr << "Erreur d'export de la led " << ledsNumber[i] << std::endl;
				status = 1;
			}
			else {
				fclose(fd);
				
				leds[i] = gpioBaseFolder + "gpio" + ledsNumber[i] + "/";
				
				fd = fopen((leds[i] + "direction").c_str(), "w");
				if(fd != NULL) {
					fputs("out", fd);
					fclose(fd);
				}
				
				fd = fopen((leds[i] + "value").c_str(), "w");
				if(fd != NULL) {
					ledsState[i] = false;
					fputs("0", fd);
					fclose(fd);
				}
			}
		}
	}
	
	return status;
}

void set_led(LEDS led) {
	fd = fopen((leds[led] + "value").c_str(), "w");
	if(fd != NULL) {
		ledsState[led] = true;
		fputs("1", fd);
		fclose(fd);
	}
}

void clear_led(LEDS led) {
	fd = fopen((leds[led] + "value").c_str(), "w");
	if(fd != NULL) {
		ledsState[led] = false;
		fputs("0", fd);
		fclose(fd);
	}
}

void switch_led(LEDS led) {
	fd = fopen((leds[led] + "value").c_str(), "w");
	if(fd != NULL) {
		ledsState[led] = !ledsState[led];
		fputs(ledsState[led]? "1" : "0", fd);
		fclose(fd);
	}
}

void blink_led(LEDS led) {
	
}
