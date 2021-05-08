#include "net/emcute.h"

void init_actuators(void);
void update_ctl_states(const emcute_topic_t *topic, void *data, size_t len);
int read_led(void);
int read_buzzer(void);