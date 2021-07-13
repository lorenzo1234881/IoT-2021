#!/bin/bash

make -C $RIOTBASE"/dist/tools/uhcpd" all
make -C $RIOTBASE"/dist/tools/ethos" all

iotlab_reset
iotlab_flash $RIOTBASE"/examples/gnrc_border_router/bin/iotlab-a8-m3/gnrc_border_router.elf"

$RIOTBASE"/dist/tools/ethos/start_network.sh" /dev/ttyA8_M3 tap0 $INET6_PREFIX"::/"$INET6_PREFIX_LEN 500000
