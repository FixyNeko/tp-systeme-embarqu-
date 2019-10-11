#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


typedef enum {ROUGE = 0, VERT, JAUNE, LEDS_NBR} LEDS;

typedef struct {LEDS led; int period;} blinkInfo;

const char gpioBaseFolder[] = "/sys/class/gpio/";

extern char ledsState[];

FILE *fd;

int init_leds();
void set_led(LEDS led);
void clear_led(LEDS led);
void switch_led(LEDS led);
void blink_led(blinkInfo info);
void *blink_led_thread(void *led);
