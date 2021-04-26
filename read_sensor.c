#include "read_sensor.h"

#include "xtimer.h"
#include "thread.h"

#include "periph/adc.h"
#include "periph/gpio.h"
#include "periph/pwm.h"
#include "analog_util.h"

#define TEMPSENSORPIN 0  /* A0 PA0 */
#define REDSENSORPIN 1   /* A1 PA1 */
#define GREENSENSORPIN 3 /* A3 PB0 */
#define BLUESENSORPIN 4  /* A4 PC1 */

#define ADC_RES ADC_RES_12BIT

char buzzer_thread_stack[THREAD_STACKSIZE_DEFAULT];

// init adc lines
int init_sensors(void)
{
	const int analogpins[] = {REDSENSORPIN, GREENSENSORPIN, BLUESENSORPIN, TEMPSENSORPIN};
	const int npins = sizeof(analogpins) / sizeof(int);
	
	int errpin;

	for(int i = 0; i < npins; i++) {
		if (adc_init(ADC_LINE(analogpins[i])) < 0) {
			errpin = analogpins[i];
			goto err;
		}
	}

	return 0;

err:
	printf("Initialization of ADC_LINE(%u) failed\n", errpin);
	return -1;

}

int read_photores(int photorespin)
{
	int sample = 0;
	int lux = 0;

	sample = adc_sample(photorespin, ADC_RES);
	lux = adc_util_map(sample, ADC_RES, 10, 100);

	if(sample < 0) {
		printf("ADC_LINE(%u): selected resolution not applicable\n", photorespin);
	}

	printf("ADC_LINE(%u): raw value: %i, lux: %i\n", photorespin, sample, lux);
	
	return lux;
}

void read_rgb(int* r, int* g, int* b)
{
	*r = read_photores(REDSENSORPIN);
	*g = read_photores(GREENSENSORPIN);
	*b = read_photores(BLUESENSORPIN);
}

int read_tmp36(void)
{
	int sample;

	sample = adc_sample(TEMPSENSORPIN, ADC_RES);
	if(sample < 0) {
		printf("ADC_LINE(%u): selected resolution not applicable\n", TEMPSENSORPIN);
		return -1;
	}
	
	// A variation 10 millivolt corresponds to a variation of 1 celcius degree
	// the sensor can also read -0 temperatures that's why we have to subtract 0.5
	int voltage = adc_util_map(sample, ADC_RES, 0, 5000);
	float temperature = (voltage - 500) /10.0;

	printf("tmp36: raw value: %d\n", sample);

	return temperature;
}
