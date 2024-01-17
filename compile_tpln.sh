#!/bin/bash
rm keyboards/splitkb/kyria/keymaps/tpln/rules.mk
ln -s rules.mk.rev1 keyboards/splitkb/kyria/keymaps/tpln/rules.mk
qmk compile  -kb splitkb/kyria/rev1 -km tpln
