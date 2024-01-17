#!/bin/bash
rm keyboards/splitkb/kyria/keymaps/tpln/rules.mk
ln -s rules.mk.rev3 keyboards/splitkb/kyria/keymaps/tpln/rules.mk
qmk compile -e CONVERT_TO=liatris -kb splitkb/kyria/rev3 -km tpln
