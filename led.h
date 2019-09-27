#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef enum {ROUGE = 0, VERT, JAUNE, LEDS_NBR} LEDS;
const char gpioBaseFolder[] = "/sys/class/gpio/";

char  ledsState[LEDS_NBR];

FILE *fd;

int init_leds();
void set_led(LEDS led);
void clear_led(LEDS led);
void switch_led(LEDS led);
void blink_led(LEDS led);
