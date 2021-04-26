#include "mqtt_logic.h"
#include "ctl_actuators.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef EMCUTE_ID
#define EMCUTE_ID           ("gertrud")
#endif
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_MAXLEN        (64U)

static emcute_sub_t subscription;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);
    return NULL;
}

int mqtt_init(emcute_topic_t* topic)
{
   /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    printf("Connecting to MQTT-SN broker %s port %d.\n",
           SERVER_ADDR, SERVER_PORT);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("error parsing IPv6 address\n");
        return -1;
    }

	/* connect to MQTT-SN */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", SERVER_ADDR,
               (int)gw.port);
        return -1;
    }

    printf("Successfully connected to gateway at [%s]:%i\n",
           SERVER_ADDR, (int)gw.port);

    unsigned flags_sub = EMCUTE_QOS_0;
    subscription.cb = update_state;
    subscription.topic.name = MQTT_SUB_TOPIC;
    if (emcute_sub(&subscription, flags_sub) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", MQTT_SUB_TOPIC);
        return 1;
    }

	topic->name = MQTT_PUB_TOPIC;
	
    /* get topic id */    
	if (emcute_reg(topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return -1;
    }

    return 0;
}


int mqtt_pub(emcute_topic_t topic, char* msg_pub, size_t len_msg_pub)
{
 	unsigned flags_pub = EMCUTE_QOS_0;
	
    if (emcute_pub(&topic, msg_pub, len_msg_pub, flags_pub) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                topic.name, (int)topic.id);
        return 1;
    }

    printf("Published %u bytes to topic '%s [%i]'\n", len_msg_pub, topic.name, topic.id);

    return 0;
}
