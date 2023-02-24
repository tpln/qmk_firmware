#!/bin/bash
python3 -m pip install --user qmk
sudo apt install gcc-avr avr-libc
util/qmk_install.sh 
qmk generate-compilation-database -kb splitkb/kyria/rev1 -km tpln
