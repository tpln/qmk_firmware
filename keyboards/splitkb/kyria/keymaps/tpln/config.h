/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once


#define TAPPING_TERM 180
#define TAPPING_TERM_PER_KEY

#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 10

#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 100

#define MOUSEKEY_DELAY 50
#define MOUSEKEY_WHEEL_DELAY 100

#define DISPLAY_MODS
#define TPLN_MACROS
#define TPLN_SWITCH_LAYER
//#define TPLN_HOME_ROW_BRACKETS

#define HOLD_ON_OTHER_KEY_PRESS

#define COMBO_COUNT 23

#define COMBO_TERM 25        // how quickly all combo keys must be pressed in succession to trigger
#define COMBO_MUST_HLD_MODS // if a combo triggers a modifier, only trigger when the combo is held
#define COMBO_HOLD_TERM 175  // how long at least one of the combo keys must be held to trigger


// For rev3 liatris based Kyria with per key RGB only. Requires modding rev3/info.json
// to strip away the underglow leds
#ifdef RGB_MATRIX_ENABLE
#undef RGB_MATRIX_LED_COUNT // wrong in rev3/config.h
#undef RGB_MATRIX_SPLIT // this too
#    define RGB_MATRIX_LED_COUNT 50
#    define RGB_MATRIX_SPLIT { 25, 25 }
#    define RGB_MATRIX_VAL_STEP 8   // brightness step
#    define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#    define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#endif

#define ENCODER_RESOLUTION 2
//#define RETRO_SHIFT 180
