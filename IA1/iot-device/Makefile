# name of your application
APPLICATION = iot_ia1

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401re

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos

FEATURES_REQUIRED += periph_adc
FEATURES_REQUIRED += periph_pwm

USEMODULE += analog_util

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

# Specify the mandatory networking modules for IPv6
USEMODULE += gnrc_ipv6_default

# Include MQTT-S client
USEMODULE += emcute

# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single

USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc

USEMODULE += xtimer

FEATURES_REQUIRED += periph_gpio

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# include UHCP client
USE_DHCPV6 ?= 0

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Ethos/native TAP interface and UHCP prefix can be configured from make command
TAP ?= tap0
IPV6_PREFIX ?= fe80:2::/64

# The Broker address, port and the default MQTT topic to subscribe.
SERVER_ADDR = fe80::1
SERVER_PORT = 1885
MQTT_PUB_TOPIC = local_to_aws_1
MQTT_SUB_TOPIC = aws_to_local_1

DEVICE_ID = 1

# Debug flags
CTL_ACTUATORS_DEBUG = 1
MQTT_LOGIC_DEBUG = 1
READ_SENSOR_DEBUG = 1

CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_PUB_TOPIC='"$(MQTT_PUB_TOPIC)"'
CFLAGS += -DMQTT_SUB_TOPIC='"$(MQTT_SUB_TOPIC)"'
CFLAGS += -DDEVICE_ID='"$(DEVICE_ID)"'

CFLAGS += -DCTL_ACTUATORS_DEBUG=$(CTL_ACTUATORS_DEBUG)
CFLAGS += -DMQTT_LOGIC_DEBUG=$(MQTT_LOGIC_DEBUG)
CFLAGS += -DREAD_SENSOR_DEBUG=$(READ_SENSOR_DEBUG)

ETHOS_BAUDRATE ?= 115200
# executes $(RIOTTOOLS)/ethos/setup_network.sh that sets up a tap device,
# configures a prefix and starts a uhcpd server serving that prefix towards the tap device
include $(CURDIR)/Makefile.ethos.conf

include $(RIOTBASE)/Makefile.include

.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)
