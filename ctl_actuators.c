#include "ctl_actuators.h"

#define LEDPIN GPIO_PIN(PORT_B, 5) /* 4 D3 */

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

void update_state(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int)topic->id);
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("");

    int update_state = atoi(in);

    if(update_state == ctl_actuators_state)
        return;

    switch (update_state)
    {
    case STATE_NORMAL:
        printf("change state update to %d", update_state);
        poweroff_led();
        poweroff_buzzer();
        break;
    case STATE_ALLARMING:
        printf("change state update to %d", update_state);
        poweron_led();
        poweroff_buzzer();
        break;
    case STATE_CRITICAL:
        printf("change state update to %d", update_state);
        poweron_led();
        poweron_buzzer();
        break;
    default:
        printf("format state non recognized\n");
        return;
    }

    ctl_actuators_state = update_state;
}