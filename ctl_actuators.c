#include "ctl_actuators.h"
#include "jsmn/jsmn.h"

#define MAX_TOKENS 5

led_state curr_led_state;

void init_actuators(void)
{
    LED0_OFF;
    LED1_OFF;
    LED2_OFF;

    curr_led_state.all = 0;
}

void read_led(led_state *ld)
{
    ld->all = curr_led_state.all;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void update_ctl_states(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

#if CTL_ACTUATORS_DEBUG
    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int)topic->id);
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("");
#endif

    jsmn_parser p;
    jsmntok_t t[MAX_TOKENS];
    led_state new_led_state;
    int r;
    int i;

    new_led_state.all = -1;

    jsmn_init(&p);
    r = jsmn_parse(&p, in, len, t, MAX_TOKENS);
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
    }

    int deviceId = -1;

    for( i = 1; i < r; i++) 
    {
        if (jsoneq(in, &t[i], "deviceId") == 0) {            
            deviceId = atoi(in + t[i+1].start);
            i++;
        }
        if (jsoneq(in, &t[i], "led0") == 0) {            
            new_led_state.led0 = atoi(in + t[i+1].start);
            i++;
        }
        if (jsoneq(in, &t[i], "led1") == 0) {            
            new_led_state.led1 = atoi(in + t[i+1].start);
            i++;
        }
        if (jsoneq(in, &t[i], "led2") == 0) {            
            new_led_state.led2 = atoi(in + t[i+1].start);
            i++;
        }
        else {
            printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
            in + t[i].start);
            return;
        }
    }

    if(deviceId != DEVICE_ID)
        break;

    if(new_led_state.all != -1 && new_led_state.all != curr_led_state.all)
    {
        if(new_led_state.led0) {LED0_ON;}
        else {LED0_OFF;}
        if(new_led_state.led1) {LED1_ON;}
        else {LED1_OFF;}
        if(new_led_state.led2) {LED2_ON;}
        else {LED2_OFF;}

        curr_led_state.all = new_led_state.all;

        #if CTL_ACTUATORS_DEBUG
        printf("change state led: %d\n", curr_led_state.all);
        #endif
    }
}
