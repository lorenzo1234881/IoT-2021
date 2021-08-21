#include "ctl_actuators.h"
#include "jsmn/jsmn.h"

/* BUZZERPIN 3 D3 */
/* LEDPIN 4 D4 */

#define LEDPIN GPIO_PIN(PORT_B, 5)

#define MAX_TOKENS 5

#define POWERON_BUZZER() {\
    pwm_poweron(0); \
}

#define POWEROFF_BUZZER() {\
    pwm_poweroff(0);\
}

#define POWERON_LED() {\
    gpio_set(LEDPIN); \
}

#define POWEROFF_LED() {\
    gpio_clear(LEDPIN); \
}

int led_state;
int buzzer_state;

void init_actuators(void)
{
    // init led pin
	gpio_init(LEDPIN, GPIO_OUT);
    POWEROFF_LED();

	xtimer_ticks32_t  t = xtimer_now();
	pwm_init(0, PWM_LEFT, 440, 65535);
	pwm_set(0, 1, 32768);
    xtimer_periodic_wakeup(&t, (1000LU * US_PER_MS));
    pwm_poweroff(0);

    led_state = 0;
    buzzer_state = 0;
}

int read_led(void)
{
    return led_state;
}

int read_buzzer(void)
{
    return buzzer_state;
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
    int new_led_state = -1;
    int new_buzzer_state = -1;
    int r;
    int i;

    jsmn_init(&p);
    r = jsmn_parse(&p, in, len, t, MAX_TOKENS);
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
    }

    for( i = 1; i < r; i++) 
    {
        if (jsoneq(in, &t[i], "led") == 0) {            
            new_led_state = atoi(in + t[i+1].start);
            i++;
        }
        else if (jsoneq(in, &t[i], "buzzer") == 0) {            
            new_buzzer_state = atoi(in + t[i+1].start);
            i++;
        }
        else {
            printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
            in + t[i].start);
            return;
        }
    }

    if(new_led_state != -1 && new_led_state != led_state)
    {
        if(new_led_state) {
            POWERON_LED();
        }
        else {
            POWEROFF_LED();
        }

        led_state = new_led_state;

        #if CTL_ACTUATORS_DEBUG
        printf("change state led: %d\n", led_state);
        #endif
    }

    if(new_buzzer_state != -1 && new_buzzer_state != buzzer_state)
    {
        if(new_buzzer_state) {
            POWERON_BUZZER();
        }
        else {
            POWEROFF_BUZZER();
        }
        
        buzzer_state = new_buzzer_state;

        #if CTL_ACTUATORS_DEBUG
        printf("change state led: %d\n", buzzer_state);
        #endif
    }
}
