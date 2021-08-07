#include <limits.h>

int aggregate_data_init(aggregate_data* ad)
{
	ad->max = INT_MIN;
	ad->min = INT_MAX;
	ad->sum = 0;
	ad->avg = 0;
	ad->n = 0;
}

int aggregate_data_update(aggregate_data* ad, int elem)
{
	if(elem > ad->max) ad->max = elem;
	if(elem < ad->min) ad->min = elem;
	ad->sum += elem;
	ad->n++;
	ad->avg = ad->sum / ad->n;
}
