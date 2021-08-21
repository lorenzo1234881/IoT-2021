#include "read_sensor.h"
#include "mqtt_logic.h"
#include "ctl_actuators.h"

#include "xtimer.h"

#include <stdio.h>
#include <stdlib.h>

#define TX_INTERVAL (5000LU * US_PER_MS)

#define MSG_TEMPLATE "{\
\"deviceId\":\"%s\",\
\"temperature\":%d,\
\"light\":%d,\
\"led\":%d,\
\"buzzer\":%d\
}"

#define BUFMSG_MAXLEN 200

int main(void)
{

	emcute_topic_t topic;

	init_sensors();
	init_actuators();
	mqtt_init(&topic);

	xtimer_ticks32_t t = xtimer_now();

	while(1) {
		
		char bufmsg[BUFMSG_MAXLEN];

		int lux = read_lux();
		int temp = read_temperature();
		int led = read_led();
		int buzzer = read_buzzer();

		sprintf(bufmsg, MSG_TEMPLATE, DEVICE_ID, temp, lux, led, buzzer);

		mqtt_pub(topic, bufmsg, strlen(bufmsg));
		
		xtimer_periodic_wakeup(&t, TX_INTERVAL);
	}

	return 0;

}
