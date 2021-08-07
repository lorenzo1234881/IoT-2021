#include "read_sensor.h"

#include "hts221.h"
#include "hts221_params.h"

hts221_t hts221_dev;

int init_sensors(void)
{	
	if (hts221_init(&hts221_dev, &hts221_params[0]) != HTS221_OK) {
	    puts("failed to initialize hts221");
	    return 1;
	}
	if (hts221_power_on(&hts221_dev) != HTS221_OK) {
	    puts("failed to set power on");
	    return 2;
	}
	if (hts221_set_rate(&hts221_dev, hts221_dev.p.rate) != HTS221_OK) {
	    puts("failed to set continuous mode");
	    return 3;
	}

	return 0;
}

int read_hum(void)
{
        uint16_t hum = 0;
        if (hts221_read_humidity(&hts221_dev, &hum) != HTS221_OK) {
            puts("failed to read humidity");
        }

	return hum;
}

int read_temperature(void)
{
        int16_t temp = 0;
        if (hts221_read_temperature(&hts221_dev, &temp) != HTS221_OK) {
            puts("failed to read temperature");
        }

	return temp;
}
