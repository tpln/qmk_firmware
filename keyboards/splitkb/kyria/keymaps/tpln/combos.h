// REMEMBER TO CHANGE COMBO_COUNT IN config.h

// Two key combos are easily hit when typing. Don't use them!

#include "quantum_keycodes.h"
const uint16_t PROGMEM combo1[] = { KC_S, KC_D, KC_F, COMBO_END}; // sdf -> ctrl left
const uint16_t PROGMEM combo2[] = { KC_Q, KC_W, KC_E, KC_R, COMBO_END}; // qwer -> ctrl g = emacs cancel
const uint16_t PROGMEM combo3[] = { KC_D, KC_F, KC_G, COMBO_END}; // dfg -> ctrl right 
const uint16_t PROGMEM combo4[] = { KC_A, KC_S, KC_D, KC_F, COMBO_END}; // asdf -> home
const uint16_t PROGMEM combo5[] = { KC_S, KC_D, KC_F, KC_G, COMBO_END}; // sdfg -> end
const uint16_t PROGMEM combo6[] = { KC_W, KC_E, KC_R, COMBO_END}; // wer -> page up
const uint16_t PROGMEM combo7[] = { KC_X, KC_C, KC_V, COMBO_END}; // xcv -> page down
const uint16_t PROGMEM combo8[] = {KC_Z, KC_X, KC_C, KC_V, COMBO_END};  // zxcv -> ctrl z undo
const uint16_t PROGMEM combo9[] = { KC_J, KC_K, KC_L, COMBO_END}; // jkl -> i3 focus
const uint16_t PROGMEM combo10[] = { KC_H, KC_J, KC_K, COMBO_END}; // hjk -> emacs focus
const uint16_t PROGMEM combo11[] = { KC_Y, KC_U, KC_I, COMBO_END}; // yui -> i3 prev workspace
const uint16_t PROGMEM combo12[] = { KC_U, KC_I, KC_O, COMBO_END}; // uio -> i3 next workspace

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo1, LCTL(KC_LEFT)),
    COMBO(combo2, LCTL(KC_G)),
    COMBO(combo3, LCTL(KC_RIGHT)),
    COMBO(combo4, KC_HOME),
    COMBO(combo5, KC_END),
    COMBO(combo6, KC_PGUP),
    COMBO(combo7, KC_PGDOWN),
    COMBO(combo8, LCTL(KC_Z)),
    COMBO(combo9, LGUI(LSFT(KC_M))),
    COMBO(combo10, LGUI(KC_M)),
    COMBO(combo11, LGUI(KC_9)),
    COMBO(combo12, LGUI(KC_0)),
};
