#include "read_sensor.h"
#include "mqtt_logic.h"
#include "ctl_actuators.h"

#include "xtimer.h"

#include <stdio.h>
#include <stdlib.h>

#define TX_INTERVAL (5000LU * US_PER_MS)

#define MSG_TEMPLATE "{\
		\"plant_state\":%d,\
		\"red\":%d,\
		\"green\":%d,\
		\"blue\":%d,\
		\"temperature\":%d\
		}"
#define BUFMSG_MAXLEN 200

int main(void)
{

	emcute_topic_t topic;

	init_sensors();
	init_actuators();
	mqtt_init(&topic);

	xtimer_ticks32_t t = xtimer_now();

	int r,g,b;
	int temp;

	while(1) {
		
		char bufmsg[BUFMSG_MAXLEN];

		int state = read_state();
		read_rgb(&r, &g, &b);
		temp = read_temperature();

		sprintf(bufmsg, MSG_TEMPLATE, state, r, g, b, temp);

		mqtt_pub(topic, bufmsg, strlen(bufmsg));
		
		xtimer_periodic_wakeup(&t, TX_INTERVAL);
	}

	return 0;

}
