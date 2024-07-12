#include <stdint.h>
#include "keycode.h"
//#include "keycode_legacy.h"
#include "keycodes.h"
#include "process_auto_shift.h"
#include "process_combo.h"
#include "quantum_keycodes.h"
#include "print.h"
#include QMK_KEYBOARD_H

// TODO
// * all controller specific code behind flags
// * combo for switching in/out mouse layer - or double tap soimething?

// 
// * See https://github.com/qmk/qmk_firmware/blob/master/docs/feature_split_keyboard.md
//   for sending data to the slave slide
// * Synchronize sys-keys to match cmd+shift keys in i3 config
// * Doubletap left thumb (hold to move) = switch to move layer, double tap back to base
// * brackets:
//   * () = hold right pinky, tap d for ( or f for )
//   * {} = hold left pinky, tap j for { or k for }
//   * [] = tap left or right pinky

// DONE
// * Home-row modifiers
//    * E.g. A and ; == ctrl when held, shifted when held longer than tapping term but nothing else pressed
//    * Can not be part of combos
//    * Or this https://jasoncarloscox.com/blog/combo-mods/

#if CONVERT_TO == liatris
#    define LIATRIS
#else
#    define AVR
#endif

#ifdef LIATRIS
//#define DISPLAY_FANCY_LAYER
#endif

// Layers:
// 0 = base
#define L_BASE 0
// 1 = symbols: function keys, numbers, shift+numbers
#define L_NUM 1
// 2 = brackets
#define L_MOVE 2
// 3 = movement
#define L_SYS 3
#define L_GAMING 4
#define L_MOUSE 5
#define L_LIGHTS 6
#define L_SWITCH 7

#define KC_DASH KC_MINUS

#define LN_RUN LGUI(KC_N)
#define LN_DRUN LGUI(LCTL(KC_N))
#define LN_WEB LGUI(LSFT(KC_W))
#define LN_TERM LGUI(KC_H)
#define LN_SHOT LGUI(LSFT(KC_S))

#define MAX_DANCES 20
enum tap_dance_codes {
 D_OB = 0, // open brackets, ({[
 D_CB, // closing brackets, )}]
 D_QT, // Quotes, '"`
 D_DSH, // dashes, -_~
 D_EQ, // =+
 D_TAB, // tab, alt+tab
 D_RUN, // meta+n, shift+meta+n,
 #ifdef TPLN_HOME_ROW_BRACKETS
 D_F,
 D_D,
 D_S,
 D_J,
 D_K,
 D_L,
 #endif
};

uint16_t active_dance_keys[MAX_DANCES];

typedef struct {
     bool is_press_action;
     uint8_t step;
} tap;

enum {
     SINGLE_TAP = 1,
     SINGLE_HOLD,
     DOUBLE_TAP,
     TAP_AND_HOLD,
     DOUBLE_SINGLE_TAP,
     MORE_TAPS
};

#ifdef TPLN_MACROS
enum custom_keycodes {
    M_ARROW_RD = SAFE_RANGE,  //=>
    M_ARROW_LD,               //<=
    M_ARROW_RS,               //->
    M_DQT,                    //""
    M_CMT,                    // //
};
#else
#    define M_ARROW_RD _______
#    define M_ARROW_LD _______
#    define M_ARROW_RS _______
#    define M_DQT _______
#    define M_CMT _______

#endif

#ifdef TPLN_SWITCH_LAYER
#    define TPLN_MO_L_SWITCH MO(L_SWITCH)
#else
#    define TPLN_MO_L_SWITCH _______
#endif


#include "combos.h"

static tap dance_state[8];


// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// Dances;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * BASE
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  -/_   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | ({[    |   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : | '/"/`  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |   Z  |   X  |   C  |   V  |   B  | Mov  | Del  |  | i3   | BkSpc|   N  |   M  | ,  < | . >  | /?|  | =/+    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | Fkeys| LAlt | Sym  | Spc  | Super|  | Esc/ | Enter| Shift| ~    | None |
 *                        |      |      |      |      |      |  | Ctrl |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_BASE] = LAYOUT(
    TD(D_TAB), KC_Q   , KC_W  , KC_E   , KC_R   , KC_T   ,                                                     KC_Y, KC_U   , KC_I   , KC_O   , KC_P   , TD(D_DSH),
    TD(D_OB), KC_A   ,  KC_S  , KC_D   , KC_F   , KC_G   ,                                                     KC_H, KC_J   , KC_K   , KC_L  , KC_SCLN, TD(D_CB),
    TD(D_QT), KC_Z   ,  KC_X  , KC_C   , KC_V   , KC_B   , KC_LALT  , TO(L_MOUSE), TO(L_LIGHTS),MO(L_SYS), KC_N, KC_M   , KC_COMM, KC_DOT , KC_SLSH, TD(D_EQ),
    KC_LSFT, KC_LGUI, KC_LCTL , KC_SPC, MO(L_MOVE) , KC_BSPC     ,LT(L_NUM, KC_ENT), MT(MOD_RCTL, KC_ESC), KC_LGUI, MT(MOD_LSFT, KC_DEL)
    ),
/*
 * NUM
 *
 */
    [L_NUM] = LAYOUT(
LALT(KC_SPACE),   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, _______,
LGUI(KC_G),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0, TO(L_MOUSE),
         M_DQT, _______, _______, _______,M_ARROW_RS,M_ARROW_LD,_______,_______,_______,_______,M_ARROW_RD,KC_BSLS,KC_PIPE, KC_SLSH,   M_CMT, _______,
                                  _______, _______, _______, TD(D_QT), TD(D_EQ), _______, _______, _______, _______, _______
     ),

 /*
 * SYS
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |  (   |  )   |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |  {   |  }   |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |  [   |  ]   |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_SYS] = LAYOUT(
       _______, LGUI(LSFT(KC_Q)),  LN_WEB,          _______,          _______,         LN_TERM,                                     KC_AUDIO_VOL_UP   , LALT(LGUI(LSFT(KC_U))), LALT(LGUI(LSFT(KC_F))),             _______, LGUI(LSFT(LCTL(KC_P))), _______,
       _______, LGUI(LSFT(KC_A)), LN_SHOT, LGUI(LSFT(KC_D)), LGUI(LSFT(KC_F)),          _______,                                     KC_AUDIO_VOL_DOWN, KC_MEDIA_PREV_TRACK   , KC_MEDIA_PLAY_PAUSE   , KC_MEDIA_NEXT_TRACK, LALT(LGUI(LSFT(KC_L))) , RGB_VAD,
       _______, LGUI(LSFT(KC_Z)), _______,          _______, LGUI(LSFT(KC_P)), LGUI(LSFT(KC_B)), _______, _______, _______, _______,         TD(D_RUN), _______               ,             RGB_SPD   ,             RGB_SPI,         RGB_SAD, RGB_SAI,
                                              KC_CAPS_LOCK ,          _______,          _______, _______, _______, TPLN_MO_L_SWITCH, _______,           _______,               _______ , AS_TOGG
     ),
/*
 * MOV
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      | i3up |      |      |                              | PgUp | Home | Up   | End  |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | i3l  | i3dn | i3r  |      |                              | PgDn | Left | Down | Right|      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_MOVE] = LAYOUT(
       _______,     KC_F9,    KC_F10,    KC_F11,    KC_F12,      _______,                                        KC_PGUP         ,   KC_HOME   , KC_UP     ,       KC_END, LGUI(KC_M)   , LCTL(KC_PLUS),
       _______,LGUI(KC_1),LGUI(KC_2),LGUI(KC_3),LGUI(KC_4),LGUI(KC_5)   ,                                        KC_PGDN         ,   KC_LEFT   , KC_DOWN   ,     KC_RIGHT, LCTL(KC_F)   , LCTL(KC_MINUS),
       _______,LCTL(KC_Z),LCTL(KC_W),LCTL(KC_U),LCTL(KC_Y),LCTL(KC_K)   ,     _______,_______,_______,  _______ ,LGUI(LSFT(KC_R)),   LGUI(KC_S), LGUI(KC_B), LGUI(KC_DOT), LGUI(KC_SLSH), LGUI(LSFT(KC_M)),
       _______,   _______,  _______ ,   _______,   _______,LGUI(KC_SCLN),LCTL(KC_SPC),        _______,LGUI(KC_9),LGUI(KC_0)
     ),
        
/*
 * GAMING
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_GAMING] = LAYOUT(
        KC_TAB, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
       _______, _______, _______,    KC_D,    KC_F, _______,                                     _______,    KC_J,    KC_K, _______, _______, _______,
  KC_CAPS_LOCK, _______, _______, _______, _______, _______,  KC_LALT, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                    KC_F9,   KC_F5, _______,  KC_SPC, KC_LSFT, _______, KC_ENTER, KC_ESC, _______, _______
    ),
                
/*
 * Layer template
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_MOUSE] = LAYOUT(
                       _______   , _______, _______   , KC_MS_UP  , _______    , KC_MS_WH_UP  ,                                          KC_MS_WH_UP  ,    _______, KC_MS_UP  ,    _______, _______, _______,
                       TO(L_BASE), _______, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, KC_MS_WH_DOWN,                                          KC_MS_WH_DOWN, KC_MS_LEFT, KC_MS_DOWN,KC_MS_RIGHT, _______, TO(L_BASE),
                       _______   , _______, _______   , _______   , _______    , KC_MS_BTN1   ,_______   , TO(L_BASE), _______, _______, KC_MS_BTN2   , TO(L_BASE), _______    ,   _______, _______, _______,
                       _______   , _______, _______   , _______   , _______    , _______      ,KC_MS_BTN1, KC_MS_BTN2, _______, _______
    )

    #ifdef TPLN_SWITCH_LAYER
    ,
/*
 * SWITCH
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_SWITCH] = LAYOUT(
       _______,  _______, _______, _______,  _______, _______,                                   _______, _______, _______, _______, _______, _______,
       _______,    TO(0),   TO(1),   TO(2),    TO(3),   TO(4),                                     TO(5), _______, _______, _______, _______, _______,
       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
     ),
/*
 * LIGHTS
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [L_LIGHTS] = LAYOUT(
                        RGB_MODE_FORWARD, LGUI(LCTL(LSFT(KC_F1))),LGUI(LCTL(LSFT(KC_F2))),LGUI(LCTL(LSFT(KC_F3))),LGUI(LCTL(LSFT(KC_F4))),LGUI(LCTL(LSFT(KC_F5))),LGUI(LCTL(LSFT(KC_F6))),LGUI(LCTL(LSFT(KC_F7))),LGUI(LCTL(LSFT(KC_F8))),LGUI(LCTL(LSFT(KC_F9))),LGUI(LCTL(LSFT(KC_F10))),_______,
                _______,LGUI(LCTL(LSFT(KC_A))), LGUI(LCTL(LSFT(KC_S))), LGUI(LCTL(LSFT(KC_D))), LGUI(LCTL(LSFT(KC_F))),LGUI(LCTL(LSFT(KC_G))),                                       _______, _______, _______, _______, _______, _______,
                _______, _______, _______, _______, _______, _______, _______, _______,TO(L_BASE), _______, _______, _______, _______, _______, _______, _______,
                                  _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______
     ),
#endif
    

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

uint8_t dance_step(tap_dance_state_t *state);

uint8_t dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
       // DOUBLE_SINGLE_TAP means doubletapping the key and then quickly pressing some other key
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TAP_AND_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

void on_dance(tap_dance_state_t *state, void *user_data, uint16_t code) {
    if(state->count == 3) {
        tap_code16(code);
        tap_code16(code);
        tap_code16(code);
    }
    if(state->count > 3) {
        tap_code16(code);
    }
}

void dance_finished_double(tap_dance_state_t *state, void *user_data,
                    int dance_id,
                    uint16_t single_tap_code,
                    uint16_t hold_code,
                    uint16_t double_tap_code,
                    uint16_t tap_and_hold_code) {

   uint16_t code = 0;
   uint8_t count = 1;
   dance_state[dance_id].step = dance_step(state);
   switch (dance_state[dance_id].step) {
      case SINGLE_TAP:
         code = single_tap_code; break;
      case SINGLE_HOLD:
         code = hold_code; break;
      case DOUBLE_TAP:
         code = double_tap_code;
         // repeat twice if double_tap_code same as single_tap_code
         if (double_tap_code == single_tap_code) {
            count = 2;
         }
         break;
      case TAP_AND_HOLD:
         code = tap_and_hold_code;
         break;
      case DOUBLE_SINGLE_TAP:
         code = double_tap_code;
         if (double_tap_code == single_tap_code) {
            count = 2;
         }
         break;
      default:
         // Do nothing
         count = 0;
         break;
   }
   while (count > 0 && code != 0) {
      register_code16(code);
      count--;
   }
   active_dance_keys[dance_id] = code;

}

void dance_finished_tap_or_hold(tap_dance_state_t *state, void *user_data,
                    int dance_id,
                    uint16_t tap_code,
                    uint16_t hold_code) {

   dance_finished_double(state, user_data, dance_id, tap_code, hold_code, tap_code, tap_code);
}

void dance_reset(tap_dance_state_t *state, void *user_data,
                 int dance_id) {
    wait_ms(10);
    uint16_t code = active_dance_keys[dance_id];
    if (code != 0) {
            unregister_code16(code);
    }
    active_dance_keys[dance_id] = 0;
    dance_state[dance_id].step = 0;
}


// Quotes dance: '"`

void on_dance_quotes(tap_dance_state_t *state, void *user_data);
void dance_quotes_finished(tap_dance_state_t *state, void *user_data);
void dance_quotes_reset(tap_dance_state_t *state, void *user_data);
 
void on_dance_quotes(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_QUOTE);
}


void dance_quotes_finished(tap_dance_state_t *state, void *user_data) {
        dance_finished_double(state, user_data, D_QT, KC_QUOTE, KC_GRAVE, KC_DQUO, KC_QUOTE);
}

void dance_quotes_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state, user_data, D_QT);
}

// Tab dance
void on_dance_tab(tap_dance_state_t *state, void *user_data);
void dance_tab_finished(tap_dance_state_t *state, void *user_data);
void dance_tab_reset(tap_dance_state_t *state, void *user_data);

void on_dance_tab(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_TAB);
}

void dance_tab_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_double(state,
                         user_data,
                         D_TAB,
                         KC_TAB,
                         LSFT(KC_TAB),
                         KC_TAB,
                         KC_TAB);
}

void dance_tab_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state,
               user_data,
               D_TAB);
}

// open brackets dance
void on_dance_open_brackets(tap_dance_state_t *state, void *user_data);
void dance_open_brackets_finished(tap_dance_state_t *state, void *user_data);
void dance_open_brackets_reset(tap_dance_state_t *state, void *user_data);

void on_dance_open_brackets(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_LPRN);
}

void dance_open_brackets_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_double(state,
                         user_data,
                         D_OB,
                         KC_LPRN,
                         KC_LCBR,
                         KC_LPRN,
                         KC_LBRC);
}

void dance_open_brackets_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state,
               user_data,
               D_OB);
}

//close brackets dance
void on_dance_close_brackets(tap_dance_state_t *state, void *user_data);
void dance_close_brackets_finished(tap_dance_state_t *state, void *user_data);
void dance_close_brackets_reset(tap_dance_state_t *state, void *user_data);

void on_dance_close_brackets(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_RPRN);
}

void dance_close_brackets_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_double(state,
                         user_data,
                         D_CB,
                         KC_RPRN,
                         KC_RCBR,
                         KC_RPRN,
                         KC_RBRC);
}

void dance_close_brackets_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state,
               user_data,
               D_CB);
}

// equal dance
void on_dance_equal(tap_dance_state_t *state, void *user_data);
void dance_equal_finished(tap_dance_state_t *state, void *user_data);
void dance_equal_reset(tap_dance_state_t *state, void *user_data);

void on_dance_equal(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_EQUAL);
}

void dance_equal_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_tap_or_hold(state,
                              user_data,
                              D_EQ,
                              KC_EQUAL,
                              KC_PLUS);
}

void dance_equal_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state,
               user_data,
               D_EQ);
}

// dash dance
void on_dance_dash(tap_dance_state_t *state, void *user_data);
void dance_dash_finished(tap_dance_state_t *state, void *user_data);
void dance_dash_reset(tap_dance_state_t *state, void *user_data);

void on_dance_dash(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_DASH);
}

void dance_dash_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_double(state,
                         user_data,
                         D_DSH,
                         KC_DASH,
                         KC_UNDS,
                         KC_DASH,
                         KC_TILD);
}

void dance_dash_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state,
               user_data,
               D_DSH);
}

// Run dance
void on_dance_run(tap_dance_state_t *state, void *user_data);
void dance_run_finished(tap_dance_state_t *state, void *user_data);
void dance_run_reset(tap_dance_state_t *state, void *user_data);

void on_dance_run(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, LGUI(KC_N));
}

void dance_run_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_tap_or_hold(state,
                              user_data,
                              D_RUN,
                              LGUI(KC_N),
                              LCTL(LGUI(KC_N)));
}

void dance_run_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state,
               user_data,
               D_RUN);
}

//////////////////// 
// HOME ROW BRACKETS
////////////////////

#ifdef TPLN_HOME_ROW_BRACKETS
// F dance
void on_dance_f(tap_dance_state_t *state, void *user_data);
void dance_f_finished(tap_dance_state_t *state, void *user_data);
void dance_f_reset(tap_dance_state_t *state, void *user_data);
void on_dance_f(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_F);
}
void dance_f_finished(tap_dance_state_t *state, void *user_data) {
        dance_finished(state,
                       user_data,
                       D_F,
                       KC_F,
                       LSFT(KC_F),
                       KC_F,
                       KC_LPRN);
}
void dance_f_reset(tap_dance_state_t *state, void *user_data) {
        dance_reset(state,
                    user_data,
                    D_F,
                    KC_F,
                    LSFT(KC_F),
                    KC_F,
                    KC_LPRN);
}

// D dance
void on_dance_d(tap_dance_state_t *state, void *user_data);
void dance_d_finished(tap_dance_state_t *state, void *user_data);
void dance_d_reset(tap_dance_state_t *state, void *user_data);
void on_dance_d(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_D);
}
void dance_d_finished(tap_dance_state_t *state, void *user_data) {
        dance_finished(state,
                       user_data,
                       D_D,
                       KC_D,
                       LSFT(KC_D),
                       KC_D,
                       KC_LCBR);
}
void dance_d_reset(tap_dance_state_t *state, void *user_data) {
        dance_reset(state,
                    user_data,
                    D_D,
                    KC_D,
                    LSFT(KC_D),
                    KC_D,
                    KC_LCBR);
}

// S dance
void on_dance_s(tap_dance_state_t *state, void *user_data);
void dance_s_finished(tap_dance_state_t *state, void *user_data);
void dance_s_reset(tap_dance_state_t *state, void *user_data);
void on_dance_s(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_S);
}
void dance_s_finished(tap_dance_state_t *state, void *user_data) {
        dance_finished(state,
                       user_data,
                       D_S,
                       KC_S,
                       LSFT(KC_S),
                       KC_S,
                       KC_LEFT_BRACKET);
}
void dance_s_reset(tap_dance_state_t *state, void *user_data) {
        dance_reset(state,
                    user_data,
                    D_S,
                    KC_S,
                    LSFT(KC_S),
                    KC_S,
                    KC_LEFT_BRACKET);
}

// J
void on_dance_j(tap_dance_state_t *state, void *user_data);
void dance_j_finished(tap_dance_state_t *state, void *user_data);
void dance_j_reset(tap_dance_state_t *state, void *user_data);
void on_dance_j(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_J);
}
void dance_j_finished(tap_dance_state_t *state, void *user_data) {
        dance_finished(state,
                       user_data,
                       D_J,
                       KC_J,
                       LSFT(KC_J),
                       KC_J,
                       KC_RPRN);
}
void dance_j_reset(tap_dance_state_t *state, void *user_data) {
        dance_reset(state,
                    user_data,
                    D_J,
                    KC_J,
                    LSFT(KC_J),
                    KC_J,
                    KC_RPRN);
}

// K dance
void on_dance_k(tap_dance_state_t *state, void *user_data);
void dance_k_finished(tap_dance_state_t *state, void *user_data);
void dance_k_reset(tap_dance_state_t *state, void *user_data);
void on_dance_k(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_K);
}
void dance_k_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_double(state, user_data,
                         D_K,
                         KC_K,
                         LSFT(KC_K),
                         KC_K,
                         KC_RCBR);
);
}
void dance_k_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state, user_data, D_K);
}

// L dance
void on_dance_l(tap_dance_state_t *state, void *user_data);
void dance_l_finished(tap_dance_state_t *state, void *user_data);
void dance_l_reset(tap_dance_state_t *state, void *user_data);
void on_dance_l(tap_dance_state_t *state, void *user_data) {
    on_dance(state, user_data, KC_L);
}
void dance_l_finished(tap_dance_state_t *state, void *user_data) {
   dance_finished_double(state, user_data,
                         D_L,
                         KC_L,
                         LSFT(KC_L),
                         KC_L,
                         KC_RIGHT_BRACKET);
}
void dance_l_reset(tap_dance_state_t *state, void *user_data) {
   dance_reset(state, user_data, D_L);
}
#endif

// Dance table
tap_dance_action_t tap_dance_actions[] = {
        [D_QT] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_quotes, dance_quotes_finished, dance_quotes_reset),
        [D_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_tab, dance_tab_finished, dance_tab_reset),
        [D_OB] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_open_brackets, dance_open_brackets_finished, dance_open_brackets_reset),
        [D_CB] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_close_brackets, dance_close_brackets_finished, dance_close_brackets_reset),
        [D_EQ] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_equal, dance_equal_finished, dance_equal_reset),        
        [D_DSH] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_dash, dance_dash_finished, dance_dash_reset),
        [D_RUN] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_run, dance_run_finished, dance_run_reset),
        #ifdef TPLN_HOME_ROW_BRACKETS
        [D_F] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_f, dance_f_finished, dance_f_reset),
        [D_D] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_d, dance_d_finished, dance_d_reset),
        [D_S] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_s, dance_s_finished, dance_s_reset),
        [D_J] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_j, dance_j_finished, dance_j_reset),
        [D_K] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_k, dance_k_finished, dance_k_reset),
        [D_L] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_l, dance_l_finished, dance_l_reset),
        #endif
        //        [DANCE_1] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_1, dance_1_finished, dance_1_reset),
};

// Duration of holding a key to be determined holding instead of tapping
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
        // Add more time for pinky dances
    switch (keycode) {
        case TD(D_QT):
    case TD(D_EQ):
    case TD(D_DSH):
        case TD(D_OB):
        case TD(D_CB):
            return TAPPING_TERM + 50;
        default:
            return TAPPING_TERM;
    }
}


#ifdef TPLN_MACROS
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case M_ARROW_RD: //=>
          if (record->event.pressed) {
                  SEND_STRING(SS_TAP(X_EQUAL) SS_DELAY(100) SS_LSFT(SS_TAP(X_DOT)));
          }
          break;
  case M_ARROW_LD: //<=
          if (record->event.pressed) {
                  SEND_STRING(SS_LSFT(SS_TAP(X_COMMA)) SS_DELAY(100) SS_TAP(X_EQUAL));
          }
          break;
  case M_ARROW_RS: //->
          if (record->event.pressed) {
                  SEND_STRING(SS_TAP(X_MINUS) SS_DELAY(100) SS_LSFT(SS_TAP(X_DOT)));
          }
          break;
  case M_DQT: //""
          if (record->event.pressed) {
                  SEND_STRING(SS_LSFT(SS_TAP(X_QUOT)) SS_DELAY(100) SS_LSFT(SS_TAP(X_QUOT)));
          }
          break;
  case M_CMT: // //
          if (record->event.pressed) {
                  SEND_STRING(SS_TAP(X_SLASH) SS_DELAY(100) SS_TAP(X_SLASH));
          }
          break;
  }
  return true;
}
#endif


/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

//static uint8_t prev_layer = 0;
//static uint8_t flash_count = 0;

void oled_write_spaces(uint8_t n, bool invert) {

        for (uint8_t c = 0; c < n; c++) {
                oled_write_P(PSTR(" "), invert);;
        }
}

void print_empty_lines(uint8_t number) {
        for (uint8_t i = 0; i < number; i++) {
                oled_write_P(PSTR("\n"), false);
        }
}

void print_layer(const char* name, uint8_t spaces_before, uint8_t spaces_after, uint8_t number) {

#ifdef DISPLAY_FANCY_LAYER
        print_empty_lines(number);
        oled_write_spaces(spaces_before, true);
        oled_write_P(name, false);
        oled_write_spaces(spaces_after, true);
        oled_write_P(PSTR("\n"), false);
        print_empty_lines(5-number);
#else
        oled_write_P(name, false);
#endif
}

void set_led_color_from_layer(int8_t current_layer) {

        switch(current_layer) {
        case L_BASE:
                rgb_matrix_set_color_all(RGB_BLACK);
                break;
        case L_NUM:
                rgb_matrix_set_color_all(RGB_YELLOW);
                break;
        case L_MOVE:
                rgb_matrix_set_color_all(RGB_CYAN);
                break;
        case L_SYS:
                rgb_matrix_set_color_all(RGB_GREEN);
                break;
        case L_GAMING:
                rgb_matrix_set_color_all(RGB_PURPLE);
                break;
        default:
                rgb_matrix_set_color_all(RGB_WHITE);
                break;
        }
}

void on_layer_changed(int8_t prev_layer, int8_t current_layer) {

        //#ifdef RGB_MATRIX_ENABLE
        //set_led_color_from_layer(current_layer);
        //#endif
        
        if (current_layer == L_GAMING) {
                autoshift_disable();
                combo_disable();
        }
        else {
                autoshift_enable();
                combo_enable();
        }
}

// Enables or disables autoshift depending on the layer.
void check_for_layer_change(void) {
        static int8_t prev_layer = L_BASE;
        int8_t current_layer = get_highest_layer(layer_state|default_layer_state);
        if (prev_layer != current_layer) {
                on_layer_changed(prev_layer, current_layer);
        }
        prev_layer = current_layer;
        return;
}

bool oled_task_user(void) {    /* uint8_t layer = biton32(layer_state); */

#ifdef TPLN_SWITCH_LAYER
        static int8_t c = 0;
        static int8_t i = 1;
#endif
        
#ifdef DISPLAY_MODS
        uint8_t mods = get_mods();
#endif
        if (is_keyboard_master()) {
                int8_t current_layer = get_highest_layer(layer_state|default_layer_state);
                check_for_layer_change();
                
                switch (current_layer) {
                case L_BASE:
                        print_layer(PSTR(" BASE "), 2, 12, 0);
                        break;
                case L_NUM:
                        print_layer(PSTR(" NUM "), 4, 11, 1);
                        break;
                case L_MOVE:
                        print_layer(PSTR(" MOVE "), 6, 8, 2);
                        break;
                case L_SYS:
                        print_layer(PSTR(" SYS "), 8, 7, 3);
                        break;
                case L_GAMING:
                        print_layer(PSTR(" GAMING "), 10, 2, 4);
                        break;
                case L_MOUSE:
                        print_layer(PSTR(" MOUSE "), 12, 0, 5);
                        break;
                case L_LIGHTS:
                        print_layer(PSTR(" LIGHTS "), 12, 0, 6);
                        break;
#ifdef TPLN_SWITCH_LAYER
                case L_SWITCH:
                        c = c + i;
                        if (c % 5 == 0) {
                                i = -i;
                        }
                        print_layer(PSTR(" SWITCH "), 6, 6, c % 6);
#endif
                        break;
                }
#ifdef debug_RGB_MATRIX_ENABLE                
                if (rgb_matrix_is_enabled()) {
                        oled_write_P(PSTR("E1"), false );
                }
                else {
                        oled_write_P(PSTR("E0"), false);
                }
                char mode[] = {'A', 0};
                mode[0] = mode[0] + (char)rgb_matrix_get_mode();
                oled_write_P(mode, false);
#endif                        
                //oled_write_P(PSTR("%d\n", rgb_matrix_get_mode()), false); 
#ifdef DISPLAY_MODS
                oled_write_P((mods & MOD_MASK_CTRL) ? PSTR("CTRL ") : PSTR("     "), false);
                oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHIFT ") : PSTR("      "), false);
                oled_write_P((mods & MOD_MASK_ALT) ? PSTR("ALT ") : PSTR("    "), false);
                oled_write_P((mods & MOD_MASK_GUI) ? PSTR("META \n") : PSTR("     \n"), false);

                
                // Write host Keyboard LED Status to OLEDs
                led_t led_usb_state = host_keyboard_led_state();
                //oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
                oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
                //oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
#endif
        } else {
                // slave side display, seems this one doesn't know what is going on.
                // in order to show something meaningful, need to first transfer data
                // over the TRRS cable...
                /* static int slave_anim = 0; */
                /* slave_anim++; */
                /* print_layer("O", slave_anim % 16, 16-(slave_anim % 16), 3); */

        }
        return false;
}
#endif

#ifdef ENCODER_ENABLE

void encoder_left_default(bool clockwise) {
        // Volume control
        if (clockwise) {
                tap_code(KC_VOLU);
        } else {
                tap_code(KC_VOLD);
        }
}

void encoder_right_default(bool clockwise) {
        if (clockwise) {
                //tap_code16(LCTL(KC_EQUAL));
                tap_code16(KC_MS_WH_UP);
        } else {
                //tap_code16(LCTL(KC_MINUS));
                tap_code16(KC_MS_WH_DOWN);
        }        
}

void encoder_left_mouse_layer(bool clockwise) {

        if (clockwise) {
                tap_code16(KC_MS_RIGHT);
        } else {
                tap_code16(KC_MS_LEFT);
        }
}

void encoder_right_mouse_layer(bool clockwise) {
        // Page up/Page down
        if (clockwise) {
                tap_code16(KC_MS_UP);
        } else {
                tap_code16(KC_MS_DOWN);
        }
}

void encoder_left_lights_layer(bool clockwise) {

        bool ctrl = get_mods() & MOD_MASK_CTRL;
        if (!ctrl) {
                if (clockwise) {
                        rgb_matrix_increase_sat_noeeprom();
                } else {
                        rgb_matrix_decrease_sat_noeeprom();
                }
        }
        else {
                if (clockwise) {
                        rgb_matrix_increase_speed_noeeprom();
                } else {
                        rgb_matrix_decrease_speed_noeeprom();
                }

        }
}

void encoder_right_lights_layer(bool clockwise) {

        bool ctrl = get_mods() & MOD_MASK_CTRL;

        if (!ctrl) {
                if (clockwise) {
                        rgb_matrix_increase_val_noeeprom();
                } else {
                        rgb_matrix_decrease_val_noeeprom();
                }
        }
        else {
                if (clockwise) {
                        rgb_matrix_increase_hue_noeeprom();
                } else {
                        rgb_matrix_decrease_hue_noeeprom();
                }
        }
}

void encoder_right_move_layer(bool clockwise) {

        // Replace with something else if needed
        encoder_right_default(clockwise);
}

void encoder_left_num_layer(bool clockwise) {
        if (clockwise) {
                tap_code(KC_RIGHT);
        } else {
                tap_code(KC_LEFT);
        }
}

bool encoder_update_user(uint8_t index, bool clockwise) {

        uprintf("encoder_update_user(%d, %d)\n", index, clockwise);
        int8_t current_layer = get_highest_layer(layer_state|default_layer_state);

        switch (index) {
        case 0: // left half encoder
                switch (current_layer) {
                case L_MOUSE:
                        //encoder_left_mouse_layer(clockwise);
                        encoder_left_default(clockwise);
                        break;
                case L_NUM:
                        encoder_left_num_layer(clockwise);
                        break;
                case L_LIGHTS:
                        encoder_left_lights_layer(clockwise);
                        break;
                default:
                        encoder_left_default(clockwise);
                        break;                
                };
                break;
        case 1: // right half encoder
                switch (current_layer) {
                case L_MOUSE:
                        //encoder_right_mouse_layer(clockwise);
                        encoder_right_default(clockwise);
                        break;
                case L_MOVE:
                        encoder_right_move_layer(clockwise);
                        break;
                case L_LIGHTS:
                        encoder_right_lights_layer(clockwise);
                        break;
                default:
                        encoder_right_default(clockwise);
                        break;                
                };
                break;
        default:
                return true;
                break;
        }
        return false;
}

#endif

void keyboard_post_init_user(void) {
        #ifdef CONSOLE_ENABLE
        debug_enable=true;
        #endif
        //debug_matrix=true;
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_enable_noeeprom(); // enables RGB, without saving settings
        rgb_matrix_mode(2);
        //rgb_matrix_sethsv_noeeprom(HSV_BLUE);
#endif
        /* rgb_matrix_set_color_all(0,0,0); // sets the color to red without saving */
        /* rgb_matrix_set_color_all(0,0,0); // sets the color to red without saving */
}

#ifdef RGB_MATRIX_ENABLE
// Per layer RGB color
/* bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { */

/*         switch(get_highest_layer(layer_state|default_layer_state)) { */
/*         case L_BASE: */
/*                 rgb_matrix_set_color_all(RGB_BLACK); */
/*                 break; */
/*         case L_NUM: */
/*                 rgb_matrix_set_color_all(RGB_YELLOW); */
/*                 break; */
/*         case L_MOVE: */
/*                 rgb_matrix_set_color_all(RGB_CYAN); */
/*                 break; */
/*         case L_SYS: */
/*                 rgb_matrix_set_color_all(RGB_GREEN); */
/*                 break; */
/*         case L_GAMING: */
/*                 rgb_matrix_set_color_all(RGB_PURPLE); */
/*                 break; */
/*         default: */
/*                 rgb_matrix_set_color_all(RGB_WHITE); */
/*                 break; */
/*         } */

/*     return false; */
/* } */
#endif


void set_powerled_on(bool on) {

#ifdef LIATRIS
  // Set our LED pin as output
  setPinOutput(24);
  if (on) {
          writePinLow(24);
  }
  else {
          // Turn the LED off
          // (Due to technical reasons, high is off and low is on)
          writePinHigh(24);
  }
#endif
}

void keyboard_pre_init_user(void) {
        // bright light! bright light!
        set_powerled_on(false);
}

