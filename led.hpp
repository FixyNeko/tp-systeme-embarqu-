#include <cstdio>
#include <string>
#include <iostream>
#include <unistd.h>

enum LEDS {ROUGE = 0, VERT, JAUNE, LEDS_NBR};
const std::string gpioBaseFolder = "/sys/class/gpio/";

std::string ledsNumber[LEDS_NBR];
std::string leds[LEDS_NBR];
bool        ledsState[LEDS_NBR];

FILE *fd;

int init_leds();
void set_led(LEDS led);
void clear_led(LEDS led);
void switch_led(LEDS led);
void blink_led(LEDS led);
