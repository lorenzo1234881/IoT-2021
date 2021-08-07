#include "net/emcute.h"

int mqtt_init(emcute_topic_t* topic);
int mqtt_pub(emcute_topic_t topic, char* msg_pub, size_t len_msg_pub);