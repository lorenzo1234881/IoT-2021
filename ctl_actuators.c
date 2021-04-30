#include "ctl_actuators.h"
#include "jsmn/jsmn.h"

/* BUZZERPIN 3 D3 */
/* LEDPIN 4 D4 */

#define LEDPIN GPIO_PIN(PORT_B, 5)

#define MAX_TOKENS 3

int ctl_actuators_state;

void poweron_buzzer(void)
{	
    pwm_poweron(0);
	return;
}

void poweroff_buzzer(void)
{	
    pwm_poweroff(0);
	return;
}

void poweron_led(void)
{
	gpio_set(LEDPIN);
}

void poweroff_led(void)
{
	gpio_clear(LEDPIN);
}


void init_actuators(void)
{
    // init led pin
	gpio_init(LEDPIN, GPIO_OUT);
    poweroff_led();

	xtimer_ticks32_t  t = xtimer_now();
	pwm_init(0, PWM_LEFT, 440, 65535);
	pwm_set(0, 1, 32768);
    xtimer_periodic_wakeup(&t, (1000LU * US_PER_MS));
    pwm_poweroff(0);

    ctl_actuators_state = STATE_NORMAL;
}

int read_state(void)
{
    return ctl_actuators_state;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void update_state(const emcute_topic_t *topic, void *data, size_t len)
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
    int update_state = 0;
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
        if (jsoneq(in, &t[i], "plant_status") == 0) {            
            update_state = atoi(in + t[i+1].start);
            i++;
        }
        else {
            printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
            in + t[i].start);
            return;
        }

    if(update_state == ctl_actuators_state)
        return;

#if CTL_ACTUATORS_DEBUG
    printf("change state to %d\n", update_state);
#endif

    switch (update_state)
    {
    case STATE_NORMAL:
        poweroff_led();
        poweroff_buzzer();
        break;
    case STATE_ALLARMING:
        poweron_led();
        poweroff_buzzer();
        break;
    case STATE_CRITICAL:
        poweron_led();
        poweron_buzzer();
        break;
    default:
        printf("format state non recognized\n");
        return;
    }

    ctl_actuators_state = update_state;
}