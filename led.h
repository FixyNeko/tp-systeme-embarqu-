#ifndef LED_DEF
#define LED_DEF

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>


typedef enum {ROUGE = 0, VERT, JAUNE, LEDS_NBR} LEDS;

typedef struct {LEDS led; int period;} blinkInfo;

extern const char gpioBaseFolder[];

extern char ledsState[];

extern char ledsBlink[];
extern pthread_cond_t  ledsConds[];
extern pthread_mutex_t ledsMutexs[];

FILE *fd;

int init_leds();
void set_led(LEDS led);
void clear_led(LEDS led);
void switch_led(LEDS led);
void blink_led(blinkInfo info);
void *blink_led_thread(void *led);

#endif
