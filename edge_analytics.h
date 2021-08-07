typedef struct {
	int max;
	int min;
	int avg;
	int sum;
	int n;
} aggregate_data;

int aggregate_data_init(aggregate_data* ad);
int aggregate_data_update(aggregate_data* ad, int elem)
