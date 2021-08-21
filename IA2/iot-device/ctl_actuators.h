#pragma once
#include "net/emcute.h"

typedef union {
    struct {    
        unsigned led0 : 1;
        unsigned led1 : 1;
        unsigned led2 : 1;
    };
    unsigned all;    
} led_state;

void init_actuators(void);
void update_ctl_states(const emcute_topic_t *topic, void *data, size_t len);
void read_led(led_state *ld);
