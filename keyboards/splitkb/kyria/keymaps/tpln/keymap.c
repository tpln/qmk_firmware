#include "keycode.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H

// TODO
// * Doubletap left thumb (hold to move) = switch to move layer, double tap back to base

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
#define L_SWITCH 6

#define KC_DASH KC_MINUS

#define LN_RUN LGUI(KC_N)
#define LN_DRUN LGUI(LCTL(KC_N))
#define LN_WEB LGUI(LSFT(KC_W))
#define LN_TERM LGUI(KC_H)
#define LN_SHOT LGUI(LSFT(KC_S))

enum tap_dance_codes {
 D_OB = 0, // open brackets, ({[
 D_CB, // closing brackets, )}]
 D_QT, // Quotes, '"`
 D_DSH, // dashes, -_~
 D_EQ, // =+
 D_TAB, // tab, alt+tab
 D_SPC, // space, _
 D_RUN // meta+n, shift+meta+n
};

typedef struct {
     bool is_press_action;
     uint8_t step;
} tap;

enum {
     SINGLE_TAP = 1,
     SINGLE_HOLD,
     DOUBLE_TAP,
     DOUBLE_HOLD,
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
    TD(D_TAB), KC_Q   , KC_W  , KC_E   , KC_R   , KC_T   ,                                                        KC_Y                , KC_U   , KC_I   , KC_O   , KC_P   , TD(D_DSH),
    TD(D_OB), KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                                                         KC_H                , KC_J   , KC_K   , KC_L   , KC_SCLN, TD(D_CB),
    TD(D_QT), KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_LALT  , MO(L_MOUSE), TPLN_MO_L_SWITCH,MO(L_SYS)        , KC_N                , KC_M   , KC_COMM, KC_DOT , KC_SLSH, TD(D_EQ),
    KC_LSFT, KC_LGUI, KC_LCTL , KC_SPC, MO(L_MOVE) , KC_BSPC     ,LT(L_NUM, KC_ENT), MT(MOD_RCTL, KC_ESC), KC_LGUI, MT(MOD_LSFT, KC_DEL)
    ),
/*
 * NUM
 *
 */
    [L_NUM] = LAYOUT(
LALT(KC_SPACE),   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, _______,
    LGUI(KC_G),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
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
       _______, LGUI(LSFT(KC_Q)),  LN_WEB,          _______,          _______,         LN_TERM,                                     KC_AUDIO_VOL_UP   , LALT(LGUI(LSFT(KC_U))), LALT(LGUI(LSFT(KC_F))),             _______,          _______, _______,
       _______, LGUI(LSFT(KC_A)), LN_SHOT, LGUI(LSFT(KC_D)), LGUI(LSFT(KC_F)),          _______,                                     KC_AUDIO_VOL_DOWN, KC_MEDIA_PREV_TRACK   , KC_MEDIA_PLAY_PAUSE   , KC_MEDIA_NEXT_TRACK, LALT(LGUI(LSFT(KC_L))) , _______,
       _______, LGUI(LSFT(KC_Z)), _______,          _______, LGUI(LSFT(KC_P)), LGUI(LSFT(KC_B)), _______, _______, _______, _______,         TD(D_RUN), KC_AUDIO_MUTE         ,             _______   ,             _______,          _______, _______,
                                              KC_CAPS_LOCK ,          _______,          _______, _______, _______, _______, _______,           _______,               _______ , KC_ASTG
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
       _______,LCTL(KC_Z),LCTL(KC_W),LCTL(KC_U),LCTL(KC_Y),LCTL(KC_K)   ,     _______,_______,_______,KC_MINUS  ,LGUI(LSFT(KC_R)),   LGUI(KC_S), LGUI(KC_B), LGUI(KC_DOT), LGUI(KC_SLSH), LGUI(LSFT(KC_M)),
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
       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
  KC_CAPS_LOCK, _______, _______, _______, _______, _______,  KC_ESC, KC_LALT, _______, _______, _______, _______, _______, _______, _______, _______,
                                    KC_F9,   KC_F5, _______,  KC_SPC, KC_LSFT, _______, _______, _______, _______, _______
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
                       _______, _______, _______, _______, _______, _______,                                     KC_MS_WH_UP  , KC_MS_BTN1, KC_MS_UP  , KC_MS_BTN2 , _______, _______,
                       _______, _______, _______, _______, _______, _______,                                     KC_MS_WH_DOWN, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, _______, _______,
                       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ ,      _______,    _______,   _______ ,     _______, _______, _______,
                       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
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

uint8_t dance_step(qk_tap_dance_state_t *state);

uint8_t dance_step(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

// Quotes dance: '"`

void on_dance_quotes(qk_tap_dance_state_t *state, void *user_data);
void dance_quotes_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_quotes_reset(qk_tap_dance_state_t *state, void *user_data);
 
void on_dance_quotes(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_QUOTE);
        tap_code16(KC_QUOTE);
        tap_code16(KC_QUOTE);
    }
    if(state->count > 3) {
        tap_code16(KC_QUOTE);
    }
}

void dance_quotes_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_QT].step = dance_step(state);
    switch (dance_state[D_QT].step) {
    case SINGLE_TAP: register_code16(KC_QUOTE); break; // '
    case SINGLE_HOLD: register_code16(KC_GRAVE); break; // `
    case DOUBLE_TAP: register_code16(KC_DQUO); break; // "
        case DOUBLE_SINGLE_TAP: tap_code16(KC_QUOTE); register_code16(KC_QUOTE);
    }
}

void dance_quotes_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_QT].step) {
        case SINGLE_TAP: unregister_code16(KC_QUOTE); break;
        case SINGLE_HOLD: unregister_code16(KC_GRAVE); break;
        case DOUBLE_TAP: unregister_code16(KC_DQUO); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_QUOTE); break;
    }
    dance_state[D_QT].step = 0;
}

// Tab dance
void on_dance_tab(qk_tap_dance_state_t *state, void *user_data);
void dance_tab_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_tab_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_tab(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_TAB);
        tap_code16(KC_TAB);
        tap_code16(KC_TAB);
    }
    if(state->count > 3) {
        tap_code16(KC_TAB);
    }
}

void dance_tab_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_TAB].step = dance_step(state);
    switch (dance_state[D_TAB].step) {
        case SINGLE_TAP: register_code16(KC_TAB); break;
        case SINGLE_HOLD: register_code16(LALT(KC_TAB)); break;
        case DOUBLE_TAP: register_code16(KC_TAB); register_code16(KC_TAB); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_TAB); register_code16(KC_TAB);
    }
}

void dance_tab_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_TAB].step) {
        case SINGLE_TAP: unregister_code16(KC_TAB); break;
        case SINGLE_HOLD: unregister_code16(LALT(KC_TAB)); break;
        case DOUBLE_TAP: unregister_code16(KC_TAB); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_TAB); break;
    }
    dance_state[D_TAB].step = 0;
}

// open brackets dance
void on_dance_open_brackets(qk_tap_dance_state_t *state, void *user_data);
void dance_open_brackets_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_open_brackets_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_open_brackets(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_LPRN);
        tap_code16(KC_LPRN);
        tap_code16(KC_LPRN);
    }
    if(state->count > 3) {
        tap_code16(KC_LPRN);
    }
}

void dance_open_brackets_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_OB].step = dance_step(state);
    switch (dance_state[D_OB].step) {
        case SINGLE_TAP: register_code16(KC_LPRN); break;
        case SINGLE_HOLD: register_code16(KC_LCBR); break;
        case DOUBLE_TAP: register_code16(KC_LPRN); register_code16(KC_LPRN); break;
        case DOUBLE_HOLD: register_code16(KC_LBRACKET); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_LPRN); register_code16(KC_LPRN);
    }
}

void dance_open_brackets_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_OB].step) {
        case SINGLE_TAP: unregister_code16(KC_LPRN); break;
        case SINGLE_HOLD: unregister_code16(KC_LCBR); break;
        case DOUBLE_TAP: unregister_code16(KC_LPRN); break;
        case DOUBLE_HOLD: unregister_code16(KC_LBRACKET); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_LPRN); break;
    }
    dance_state[D_OB].step = 0;
}

//close brackets dance
void on_dance_close_brackets(qk_tap_dance_state_t *state, void *user_data);
void dance_close_brackets_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_close_brackets_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_close_brackets(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_RPRN);
        tap_code16(KC_RPRN);
        tap_code16(KC_RPRN);
    }
    if(state->count > 3) {
        tap_code16(KC_RPRN);
    }
}

void dance_close_brackets_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_CB].step = dance_step(state);
    switch (dance_state[D_CB].step) {
        case SINGLE_TAP: register_code16(KC_RPRN); break;
        case SINGLE_HOLD: register_code16(KC_RCBR); break;
        case DOUBLE_TAP: register_code16(KC_RPRN); register_code16(KC_RPRN); break;
        case DOUBLE_HOLD: register_code16(KC_RBRACKET); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_RPRN); register_code16(KC_RPRN);
    }
}

void dance_close_brackets_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_CB].step) {
        case SINGLE_TAP: unregister_code16(KC_RPRN); break;
        case SINGLE_HOLD: unregister_code16(KC_RCBR); break;
        case DOUBLE_TAP: unregister_code16(KC_RPRN); break;
        case DOUBLE_HOLD: unregister_code16(KC_RBRACKET); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_RPRN); break;
    }
    dance_state[D_CB].step = 0;
}

// space dance
void on_dance_space(qk_tap_dance_state_t *state, void *user_data);
void dance_space_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_space_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_space(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
    }
    if(state->count > 3) {
        tap_code16(KC_SPACE);
    }
}

void dance_space_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_SPC].step = dance_step(state);
    switch (dance_state[D_SPC].step) {
        case SINGLE_TAP: register_code16(KC_SPACE); break;
        case SINGLE_HOLD: register_code16(KC_UNDS); break;
        case DOUBLE_TAP: register_code16(KC_SPACE); register_code16(KC_SPACE); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_SPACE); register_code16(KC_SPACE);
    }
}

void dance_space_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_SPC].step) {
        case SINGLE_TAP: unregister_code16(KC_SPACE); break;
        case SINGLE_HOLD: unregister_code16(KC_UNDS); break;
        case DOUBLE_TAP: unregister_code16(KC_SPACE); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_SPACE); break;
    }
    dance_state[D_SPC].step = 0;
}

// equal dance
void on_dance_equal(qk_tap_dance_state_t *state, void *user_data);
void dance_equal_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_equal_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_equal(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_EQUAL);
        tap_code16(KC_EQUAL);
        tap_code16(KC_EQUAL);
    }
    if(state->count > 3) {
        tap_code16(KC_EQUAL);
    }
}

void dance_equal_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_EQ].step = dance_step(state);
    switch (dance_state[D_EQ].step) {
        case SINGLE_TAP: register_code16(KC_EQUAL); break;
        case SINGLE_HOLD: register_code16(KC_PLUS); break;
        case DOUBLE_TAP: register_code16(KC_EQUAL); register_code16(KC_EQUAL); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_EQUAL); register_code16(KC_EQUAL);
    }
}

void dance_equal_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_EQ].step) {
        case SINGLE_TAP: unregister_code16(KC_EQUAL); break;
        case SINGLE_HOLD: unregister_code16(KC_PLUS); break;
        case DOUBLE_TAP: unregister_code16(KC_EQUAL); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_EQUAL); break;
    }
    dance_state[D_EQ].step = 0;
}

// dash dance
void on_dance_dash(qk_tap_dance_state_t *state, void *user_data);
void dance_dash_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_dash_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_dash(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_DASH);
        tap_code16(KC_DASH);
        tap_code16(KC_DASH);
    }
    if(state->count > 3) {
        tap_code16(KC_DASH);
    }
}

void dance_dash_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_DSH].step = dance_step(state);
    switch (dance_state[D_DSH].step) {
        case SINGLE_TAP: register_code16(KC_DASH); break;
        case SINGLE_HOLD: register_code16(KC_UNDS); break;
        case DOUBLE_TAP: register_code16(KC_DASH); register_code16(KC_DASH); break;
        case DOUBLE_HOLD: register_code16(KC_TILD); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_DASH); register_code16(KC_DASH);
    }
}

void dance_dash_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_DSH].step) {
        case SINGLE_TAP: unregister_code16(KC_DASH); break;
        case SINGLE_HOLD: unregister_code16(KC_UNDS); break;
        case DOUBLE_TAP: unregister_code16(KC_DASH); break;
        case DOUBLE_HOLD: unregister_code16(KC_TILD); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_DASH); break;
    }
    dance_state[D_DSH].step = 0;
}

// Run dance
void on_dance_run(qk_tap_dance_state_t *state, void *user_data);
void dance_run_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_run_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_run(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(LGUI(KC_N));
        tap_code16(LGUI(KC_N));
        tap_code16(LGUI(KC_N));
    }
    if(state->count > 3) {
        tap_code16(LGUI(KC_N));
    }
}

void dance_run_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[D_RUN].step = dance_step(state);
    switch (dance_state[D_RUN].step) {
        case SINGLE_TAP: register_code16(LGUI(KC_N)); break;
        case SINGLE_HOLD: register_code16(LCTL(LGUI(KC_N))); break;
        case DOUBLE_TAP: register_code16(LGUI(KC_N)); register_code16(LGUI(KC_N)); break;
        case DOUBLE_SINGLE_TAP: tap_code16(LGUI(KC_N)); register_code16(LGUI(KC_N));
    }
}

void dance_run_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[D_RUN].step) {
        case SINGLE_TAP: unregister_code16(LGUI(KC_N)); break;
        case SINGLE_HOLD: unregister_code16(LCTL(LGUI(KC_N))); break;
        case DOUBLE_TAP: unregister_code16(LGUI(KC_N)); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(LGUI(KC_N)); break;
    }
    dance_state[D_RUN].step = 0;
}


// Dance table
qk_tap_dance_action_t tap_dance_actions[] = {
        [D_QT] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_quotes, dance_quotes_finished, dance_quotes_reset),
        [D_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_tab, dance_tab_finished, dance_tab_reset),
        [D_OB] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_open_brackets, dance_open_brackets_finished, dance_open_brackets_reset),
        [D_CB] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_close_brackets, dance_close_brackets_finished, dance_close_brackets_reset),
        [D_SPC] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_space, dance_space_finished, dance_space_reset),
        [D_EQ] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_equal, dance_equal_finished, dance_equal_reset),        
        [D_DSH] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_dash, dance_dash_finished, dance_dash_reset),
        [D_RUN] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_run, dance_run_finished, dance_run_reset),
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

// Enables or disables autoshift depending on the layer.
void control_autoshift(int8_t current_layer) {
        static int8_t prev_layer = L_BASE;
        if (prev_layer != current_layer) {
                if (current_layer == L_GAMING) {
                        autoshift_disable();
                }
                else {
                        autoshift_enable();
                }
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

                // Control autoshift depending on the layer. This would be much nicer
                // to call from a hook that gets called when the layer changes. But
                // did not find such a hook so far, and this works OK, so...
                control_autoshift(current_layer);
                
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
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
#endif

