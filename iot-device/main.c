#include "read_sensor.h"
#include "edge_analytics.h"

#include "net/loramac.h"     /* core loramac definitions */
#include "semtech_loramac.h" /* package API */
#include "fmt.h"

#include <stdio.h>
#include <string.h>

#include <xtimer.h>

#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"

static sx127x_t sx127x;

#define TX_INTERVAL (5000LU * US_PER_MS)

#define MSG_MAXLEN 200

#define MSG_TEMPLATE "{\
\"temperature\":%d,\
\"temperatureMax\":%d,\
\"temperatureMin\":%d,\
\"temperatureAvg\":%d,\
\"humidity\":%d,\
\"humidityMax\":%d,\
\"humidityMin\":%d,\
\"humidityAvg\":%d\
}"


extern semtech_loramac_t loramac;  /* The loramac stack device descriptor */
/* define the required keys for OTAA, e.g over-the-air activation (the
   null arrays need to be updated with valid LoRa values) */
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

int main(void)
{
	char message[MSG_MAXLEN];
	xtimer_ticks32_t t;
		
	init_sensors();

    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
	
	/* Initialize the radio driver */
    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;

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
	
	t = xtimer_now();
	
	while(1) 
	{

		humidity = read_hum();
		temperature = read_temperature();

		aggregate_data_update(&agg_humidity, humidity);
		aggregate_data_update(&agg_temperature, temperature);		
		
		sprintf(message, MSG_TEMPLATE,
				temperature, agg_temperature.max, agg_temperature.min, agg_temperature.avg,
				humidity, agg_humidity.max, agg_humidity.min, agg_humidity.avg);
		
		int ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));
		if (ret != SEMTECH_LORAMAC_TX_DONE) {
			printf("semtech_loramac_send() returned %d\n", ret);
			return 1;
		}
	
		xtimer_periodic_wakeup(&t, TX_INTERVAL);
	}
}
