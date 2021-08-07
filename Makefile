BOARD ?= b-l072z-lrwan1

APPLICATION = iot_lora

BOARD_WITHOUT_LORAMAC_RX := \
    arduino-mega2560 \
    i-nucleo-lrwan1 \
    stm32f0discovery \
    waspmote-pro \

LORA_DRIVER ?= sx1276
LORA_REGION ?= EU868

USEPKG += semtech-loramac
USEMODULE += $(LORA_DRIVER)

# load loramac RX if board supports it
ifeq (,$(filter $(BOARD),$(BOARD_WITHOUT_LORAMAC_RX)))
  USEMODULE += semtech_loramac_rx
endif

USEMODULE += auto_init_loramac
USEMODULE += xtimer
USEMODULE += fmt
USEMODULE += hts221


FEATURES_OPTIONAL += periph_eeprom

# Default IotLab Config to run the test
ifneq (,$(filter iotlab%,$(MAKECMDGOALS)))
  IOTLAB_NODES ?= 1
  IOTLAB_TYPE  ?= st-lrwan1:sx1276
  IOTLAB_SITE  ?= saclay
  include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab
endif

DEVICE_ID = 1
CFLAGS += -DDEVICE_ID='"$(DEVICE_ID)"'

include $(RIOTBASE)/Makefile.include
