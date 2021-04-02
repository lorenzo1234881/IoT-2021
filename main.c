#include "read_sensor.h"
#include "mqtt_logic.h"

#include "xtimer.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{

	emcute_topic_t topic;

	init_adc_lines();

	mqtt_init(&topic);

	xtimer_ticks32_t t = xtimer_now();

	int r,g,b;
	int temp;

	while(1) {
		
		char buf_msg[100];

		read_rgb(&r, &g, &b);
		temp = read_tmp36();

		sprintf(buf_msg, "{\
		\"r\":%d,\
		\"g\":%d,\
		\"b\":%d,\
		\"temp\":%d\
		}", r, g, b, temp);

		mqtt_pub(topic, buf_msg, strlen(buf_msg));
		
		xtimer_periodic_wakeup(&t, (1000LU * US_PER_MS));
	}

	return 0;

}
