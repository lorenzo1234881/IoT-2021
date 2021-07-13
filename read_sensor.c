#include "read_sensor.h"

#include "xtimer.h"

#include "periph/adc.h"
#include "periph/gpio.h"
#include "periph/pwm.h"
#include "analog_util.h"

#define TEMPSENSORPIN 0  /* A0 PA0 */
#define PHOTORESPIN 4  /* A4 PC1 */

#define ADC_RES ADC_RES_12BIT

int init_sensors(void)
{	
	if (adc_init(ADC_LINE(PHOTORESPIN)) < 0) {
		printf("Initialization of ADC_LINE(%u) failed\n", PHOTORESPIN);
		return -1;		
	}

	if (adc_init(ADC_LINE(TEMPSENSORPIN)) < 0) {
		printf("Initialization of ADC_LINE(%u) failed\n", TEMPSENSORPIN);
		return -1;
	}
	return 0;
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

#if READ_SENSOR_DEBUG
	printf("ADC_LINE(%u): raw value: %i, lux: %i\n", photorespin, sample, lux);
#endif

	return lux;
}

int read_lux(void)
{
	return read_photores(PHOTORESPIN);
}

int read_temperature(void)
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

#if READ_SENSOR_DEBUG
	printf("tmp36: raw value: %d\n", sample);
#endif

	return temperature;
}
