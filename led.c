#include <cstdio>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

enum LEDS {ROUGE = 0, VERT, JAUNE, LEDS_NBR};

const string gpioBaseFolder = "/sys/class/gpio/";
string ledsNumber[LEDS_NBR];
string leds[LEDS_NBR];

int main(int argc, char ** argv) {
	FILE *gpioInit;
	FILE *gpioControl;
	
	ledsNumber[ROUGE] = "16";
	ledsNumber[VERT] = "20";
	ledsNumber[JAUNE] = "21";
	
	cout << gpioBaseFolder + "export" << endl;
	
	for(int i =  0; i < LEDS_NBR; i++) {
		gpioInit = fopen((gpioBaseFolder + "export").c_str(), "w");
		if(gpioInit == NULL) {
			cout << "Erreur d'ouverture du fichier export" << endl;
			return 1;
		}
		if(fputs(string(ledsNumber[i]).c_str(), gpioInit) == EOF) {
			cout << "Erreur d'export de la led " << ledsNumber[i] << endl;
			return 1;
		}
		fclose(gpioInit);
		
		leds[i] = gpioBaseFolder + "gpio" + ledsNumber[i] + "/";
		
		gpioControl = fopen((leds[i] + "direction").c_str(), "w");
		if(gpioControl != NULL) {
			fputs("out", gpioControl);
			fclose(gpioControl);
		}
		
		gpioControl = fopen((leds[i] + "value").c_str(), "w");
		if(gpioControl != NULL) {
			fputs("0", gpioControl);
			fclose(gpioControl);
		}
	}
	
	for(int i =  0; i < LEDS_NBR; i++) {
		fopen(leds[i].c_str(), "w");
	}
	
	return 0;
}
