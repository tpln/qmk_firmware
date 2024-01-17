#!/bin/bash
#set -e
if [ -n "$1" ]; then
    ./compile_tpln_liatris_kyria3.sh && sudo umount /tmp/t ; sudo mount /dev/${1} /tmp/t ; sudo cp splitkb_kyria_rev3_tpln_liatris.uf2 /tmp/t ; sleep 5; sudo umount /tmp/t
else
  echo "Usage: flash <block device name>, example: flash sdh1. Use dmesg to find out the device when resetttin Kyria."
fi


