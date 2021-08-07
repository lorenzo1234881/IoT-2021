#include "read_sensor.h"
#include "edge_analytics.h"

#include "net/loramac.h"     /* core loramac definitions */
#include "semtech_loramac.h" /* package API */

#include <stdio.h>
#include <string.h>

#include <xtimer.h>

#define MSG_MAXLEN 200

#define MSG_TEMPLATE "{\
\"deviceId\":\"%s\",\
\"temperature\":%d,\
\"temperatureMax\":%d,\
\"temperatureMin\":%d,\
\"temperatureAvg\":%d,\
\"humidity\":%d\,
\"humidityMax\":%d,\
\"humidityMin\":%d,\
\"humidityAvg\":%d,\
}"

extern semtech_loramac_t loramac;  /* The loramac stack device descriptor */
/* define the required keys for OTAA, e.g over-the-air activation (the
   null arrays need to be updated with valid LoRa values) */
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x00, 0x1D, 0x4F, 0x81, \
                                                    0xA3, 0xAB, 0x11, 0xBE };
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x70, 0xB3, 0xD5, 0x7E, \
                                                    0xD0, 0x04, 0x2D, 0xA3 };
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = {0xBD,0x33,0x53,0x12,0x5C,0x80,0xDF,0xCF,0x83,0xC7,0x14,0x38,0x57,0x9E,0xAF,0x40};

int main(void)
{
	char message[MSG_MAXLEN];
	
	init_sensors();

	/* 1. initialize the LoRaMAC MAC layer */
	semtech_loramac_init(&loramac);
	/* 2. set the keys identifying the device */
	semtech_loramac_set_deveui(&loramac, deveui);
	semtech_loramac_set_appeui(&loramac, appeui);
	semtech_loramac_set_appkey(&loramac, appkey);
	
	/* Use a fast datarate, e.g. BW125/SF7 in EU868 */  
	semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);
	
	/* 3. join the network */
	if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
	    puts("Join procedure failed");
	    return 1;
	}
	puts("Join procedure succeeded");

	aggregate_data agg_temperature;
	aggregate_data agg_humidity;

	aggregate_data_init(&agg_temperature);
	aggregate_data_init(&agg_humidity);

	int humidity;
        int temperature;
	
	while(1) {

		humidity = read_hum();
		temperature = read_temperature();

		aggregate_data_update(agg_humidity, humidity);
		aggregate_data_update(agg_temperature, temperature);		
		
		sprintf(message, MSG_TEMPLATE, DEVICE_ID,
				temperature, agg_temperature->max, agg_temperature->min, agg_temperature->avg,
				humidity, agg_humidity->max, agg_humidity->min, agg_humidity->avg);
		int ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));
		if (ret != SEMTECH_LORAMAC_TX_DONE) {
			printf("semtech_loramac_send() returned %d\n", ret);
			return 1;
		}
	
		xtimer_sleep(10U);
	    }
}
