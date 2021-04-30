#include "net/emcute.h"

#define STATE_NORMAL 0
#define STATE_ALLARMING 1
#define STATE_CRITICAL 2

void init_actuators(void);
// Update actuators status after receive a json message: ex, {"plant_status": STATE_*}
void update_state(const emcute_topic_t *topic, void *data, size_t len);
int read_state(void);