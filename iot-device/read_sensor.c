#include "read_sensor.h"

#include "isl29020.h"
#include "isl29020_params.h"

#include "lpsxxx.h"
#include "lpsxxx_params.h"


isl29020_t isl29020_dev;
lpsxxx_t lpsxxx_dev;

int init_sensors(void)
{	
	if(isl29020_init(&isl29020_dev, &isl29020_params[0]) < 0)
	{
		printf("Unable to inizialize isl29020\n");
		return -1;
	}
	if (lpsxxx_init(&lpsxxx_dev, &lpsxxx_params[0]) != LPSXXX_OK) {
        puts("Unable to inizialize lpsxxx\n");
        return 1;
    }

	return 0;
}

int read_lux(void)
{
	return isl29020_read(&isl29020_dev);
}

int read_temperature(void)
{
	int16_t temperature;
	lpsxxx_read_temp(&lpsxxx_dev, &temperature);
	
	return temperature;
}
