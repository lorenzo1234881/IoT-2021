# name of your application
APPLICATION = iot_ia2

# If no BOARD is found in the environment, use this default:
BOARD ?= iotlab-m3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../A8/riot/RIOT

USEMODULE += lps331ap
USEMODULE += isl29020
USEMODULE += xtimer
FEATURES_REQUIRED += periph_gpio periph_spi

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6
USEMODULE += gnrc_ipv6_default
# Include MQTT-SN
USEMODULE += emcute
# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Comment this out to join RPL DODAGs even if DIOs do not contain
# DODAG Configuration Options (see the doc for more info)
# CFLAGS += -DCONFIG_GNRC_RPL_DODAG_CONF_OPTIONAL_ON_JOIN

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# The Broker address, port and the default MQTT topic to subscribe.
SERVER_PORT = 1885
MQTT_PUB_TOPIC = local_to_aws_2
MQTT_SUB_TOPIC = aws_to_local_2

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

include $(RIOTBASE)/Makefile.include
