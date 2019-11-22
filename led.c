#ifdef RASP
#include "led.h"

const char gpioBaseFolder[] = "/sys/class/gpio/";

char ledsNumber[LEDS_NBR][3];
char leds[LEDS_NBR][50];
char ledsState[LEDS_NBR] = {0};

char ledsBlink[LEDS_NBR] = {0};
pthread_cond_t ledsConds[LEDS_NBR];
pthread_mutex_t ledsMutexs[LEDS_NBR];
pthread_mutex_t blink;

/**
 * \file led.c
 * \brief Permet de controller des leds sur le GPIO d'une raspberry pi
 * \author Nicolas LOXOL et Felix DHUMES
 * \version 1.0
 * \date 15/11/2019
 *
 * permet de contrôller les gpio: mise à l'état haut, état bas,
 * et clignotement de manière non bloquante
 */


/**
 * \fn int init_leds()
 * \brief Configure les fichiers de contrôle des GPIO
 * \return 0 si l'opération s'est bien passée, 1 sinon
 */

int init_leds() {
	char fileName[50] = {0};
	int status = 0;

	strcpy( ledsNumber[ROUGE] , "16");
	strcpy( ledsNumber[VERT]  , "20");
	strcpy( ledsNumber[JAUNE] , "21");

	for(int i =  0; i < LEDS_NBR; i++) {
		pthread_cond_init(ledsConds + i, NULL);
		pthread_mutex_init(ledsMutexs + i, NULL);

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


/**
 * \fn void set_led(LEDS led)
 * \brief Allume une led
 * \param led la led à allumer
 */

void set_led(LEDS led) {
	char fileName[sizeof(leds[led]) + 5];
	strcpy(fileName, leds[led]);
	strcat(fileName, "value");

	fd = fopen(fileName, "w");
	if(fd != NULL) {
		pthread_mutex_lock(ledsMutexs + led);
		ledsBlink[led] = 0;
		pthread_cond_signal(ledsConds + led);
		pthread_mutex_unlock(ledsMutexs + led);

		ledsState[led] = 1;
		fputs("1", fd);
		fclose(fd);
	}
}


/**
 * \fn void clear_led(LEDS led)
 * \brief Eteint une led
 * \param led la led à etteindre
 */
void clear_led(LEDS led) {
	char fileName[sizeof(leds[led]) + 5];
	strcpy(fileName, leds[led]);
	strcat(fileName, "value");

	fd = fopen(fileName, "w");
	if(fd != NULL) {
		pthread_mutex_lock(ledsMutexs + led);
		ledsBlink[led] = 0;
		pthread_cond_signal(ledsConds + led);
		pthread_mutex_unlock(ledsMutexs + led);

		ledsState[led] = 0;
		fputs("0", fd);
		fclose(fd);
	}
}


/**
 * \fn void switch_led(LEDS led)
 * \brief Bascule l'état d'une led
 * \param led la led à basculer
 */
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


/**
 * \fn void blink_led(blinkInfo _info)
 * \brief Lance un thread qui s'occupe de faire clignoter une led
 * \param _info contient la led à faire clignoter, et sa demi période de clignotement
 */

void blink_led(blinkInfo _info) {
	pthread_t tid;

	printf("info.period: %d\n", _info.period);

	blinkInfo *info = malloc(sizeof(blinkInfo));
	info->led = _info.led;
	info->period = _info.period;

	pthread_mutex_lock(&blink);

	pthread_create(&tid, NULL, blink_led_thread, info);
	pthread_detach(tid); // should use pthread_attr_setdetachstate and launch in detached state
}

/**
 * \fn void blink_led_thread(void *i)
 * \brief thread dédié au clignotement d'une led
 * \param *i un pointeur vers une structure blinkInfo
 *
 * thread avec une attente passive pour ne pas utiliser le processeur. Construit pour qu'un appel à
 * set_led ou clear_led ait un effet immédiat
 */

void *blink_led_thread(void *i) {
	struct timespec timeToWait;
	struct timeval tv;

	blinkInfo info;
	info.led = ((blinkInfo*) i)->led;
	info.period = ((blinkInfo*) i)->period;

	free(i);

	printf("info.period thread: %d", info.period);

	pthread_mutex_unlock(&blink);


	ledsBlink[info.led] = 1;

	while(ledsBlink[info.led]) {
		switch_led(info.led);

		gettimeofday(&tv, NULL);
		timeToWait.tv_sec = tv.tv_sec + info.period / 1000;
		timeToWait.tv_nsec = tv.tv_usec * 1000 + 1000000 * (info.period % 1000);
		timeToWait.tv_sec += timeToWait.tv_nsec / (1000000000);
		timeToWait.tv_nsec %= (1000000000);
		/*
		printf("temps reception: %d temps debloquage: %d info.period: %d\n", tv.tv_usec/1000, timeToWait.tv_nsec/1000000, info.period);
		*/
		pthread_cond_timedwait(ledsConds + info.led, ledsMutexs + info.led, &timeToWait);
	}

	pthread_mutex_unlock(ledsMutexs + info.led);

	return NULL;
}
#endif
