// REMEMBER TO CHANGE COMBO_COUNT IN config.h

// Two key combos are easily hit when typing. Don't use them!

#include "keycode.h"
#include "quantum_keycodes.h"


const uint16_t PROGMEM combo_emacs_focus[] = { KC_X, KC_C, KC_V, COMBO_END}; 
const uint16_t PROGMEM combo_emacs_cancel[] = { KC_Z, KC_X, KC_C, KC_V, COMBO_END}; 
const uint16_t PROGMEM combo_i3_focus_next[] = { KC_M, KC_COMM, KC_DOT, COMBO_END}; 
const uint16_t PROGMEM combo_i3_prev_workspace[] = { KC_Y, KC_U, KC_I, COMBO_END}; 
const uint16_t PROGMEM combo_i3_next_workspace[] = {KC_U, KC_I, KC_O, COMBO_END};

// Home-row modifier combos, ala https://jasoncarloscox.com/blog/combo-mods/
// control 
const uint16_t PROGMEM combo_left_ctrl[] = { KC_F, KC_D, COMBO_END}; 
const uint16_t PROGMEM combo_right_ctrl[] = { KC_J, KC_K, COMBO_END}; 
// super
const uint16_t PROGMEM combo_left_super[] = { KC_F, KC_S, COMBO_END}; 
const uint16_t PROGMEM combo_right_super[] = { KC_J, KC_L, COMBO_END}; 
// shift
const uint16_t PROGMEM combo_left_shift[] = { KC_F, KC_A, COMBO_END}; 
const uint16_t PROGMEM combo_right_shift[] = {KC_J, KC_SCLN, COMBO_END};  
// control super
const uint16_t PROGMEM combo_left_control_super[] = { KC_F, KC_D, KC_S, COMBO_END}; 
const uint16_t PROGMEM combo_right_control_super[] = { KC_J, KC_K, KC_L, COMBO_END}; 

// control shift
const uint16_t PROGMEM combo_left_control_shift[] = { KC_F, KC_D, KC_A, COMBO_END}; 
const uint16_t PROGMEM combo_right_control_shift[] = { KC_J, KC_K, KC_SCLN, COMBO_END}; 

// control super shift
const uint16_t PROGMEM combo_left_control_super_shift[] = { KC_F, KC_D, KC_S, KC_A, COMBO_END}; 
const uint16_t PROGMEM combo_right_control_super_shift[] = { KC_J, KC_K, KC_L, KC_SCLN, COMBO_END}; 

// super shift
const uint16_t PROGMEM combo_left_super_shift[] = { KC_F, KC_S, KC_A, COMBO_END}; 
const uint16_t PROGMEM combo_right_super_shift[] = { KC_J, KC_L, KC_SCLN, COMBO_END}; 


combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo_emacs_cancel, LCTL(KC_G)),
    COMBO(combo_i3_focus_next, LGUI(LSFT(KC_M))),
    COMBO(combo_emacs_focus, LGUI(KC_M)),
    COMBO(combo_i3_prev_workspace, LGUI(KC_9)),
    COMBO(combo_i3_next_workspace, LGUI(KC_0)),

    COMBO(combo_left_ctrl, KC_LCTL),
    COMBO(combo_left_super, KC_LGUI),
    COMBO(combo_left_shift, KC_LSFT),
    COMBO(combo_left_control_super, LCTL(KC_LGUI)),
    COMBO(combo_left_control_shift, LCTL(KC_LSFT)),
    COMBO(combo_left_control_super_shift, LCTL(LGUI(KC_LSFT))),
    COMBO(combo_left_super_shift, LGUI(KC_LSFT)),
    
    COMBO(combo_right_ctrl, KC_RCTL),
    COMBO(combo_right_super, KC_RGUI),
    COMBO(combo_right_shift, KC_RSFT),
    COMBO(combo_right_control_super, RCTL(KC_RGUI)),
    COMBO(combo_right_control_shift, RCTL(KC_RSFT)),
    COMBO(combo_right_control_super_shift, RCTL(RGUI(KC_RSFT))),
    COMBO(combo_right_super_shift, RGUI(KC_RSFT)),

    
};
