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
#    define RGB_MATRIX_VAL_STEP 8 // brightness step

// nice
#    define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT    // Static gradient left to right, speed controls how much gradient changes
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP      // How hot is your WPM!
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE   // Pulses keys hit to hue & value then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  // Hue & value pulse near multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // Hue & value pulse away on the same column and row of multiple key hits then fades value out
#    define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON // Full gradient Chevron shapped scrolling left to right */
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN        // That famous computer simulation                                                     
// 
// 
// not nice
//#    define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN

//all 
    /* #define ENABLE_RGB_MATRIX_ALPHAS_MODS         // Static dual hue, speed is hue for secondary hue */
    /* #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN    // Static gradient top to bottom, speed controls how much gradient changes */
    /* #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT    // Static gradient left to right, speed controls how much gradient changes */
    /* #define ENABLE_RGB_MATRIX_BREATHING           // Single hue brightness cycling animation */
    /* #define ENABLE_RGB_MATRIX_BAND_SAT        // Single hue band fading saturation scrolling left to right */
    /* #define ENABLE_RGB_MATRIX_BAND_VAL        // Single hue band fading brightness scrolling left to right */
    /* #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT   // Single hue 3 blade spinning pinwheel fades saturation */
    /* #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL   // Single hue 3 blade spinning pinwheel fades brightness */
    /* #define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT     // Single hue spinning spiral fades saturation */
    /* #define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL     // Single hue spinning spiral fades brightness */
    /* #define ENABLE_RGB_MATRIX_CYCLE_ALL           // Full keyboard solid hue cycling through full gradient */
    /* #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT    // Full gradient scrolling left to right */
    /* #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN       // Full gradient scrolling top to bottom */
    /* #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN        // Full gradient scrolling out to in */
    /* #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL   // Full dual gradients scrolling out to in */
    /* #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON  // Full gradient Chevron shapped scrolling left to right */
    /* #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL      // Full gradient spinning pinwheel around center of keyboard */
    /* #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL        // Full gradient spinning spiral around center of keyboard */
    /* #define ENABLE_RGB_MATRIX_DUAL_BEACON         // Full gradient spinning around center of keyboard */
    /* #define ENABLE_RGB_MATRIX_RAINBOW_BEACON      // Full tighter gradient spinning around center of keyboard */
    /* #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS   // Full dual gradients spinning two halfs of keyboard */
    /* #define ENABLE_RGB_MATRIX_FLOWER_BLOOMING     // Full tighter gradient of first half scrolling left to right and second half scrolling right to left */
    /* #define ENABLE_RGB_MATRIX_RAINDROPS           // Randomly changes a single key's hue */
    /* #define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS // Randomly changes a single key's hue and saturation */
    /* #define ENABLE_RGB_MATRIX_HUE_BREATHING       // Hue shifts up a slight ammount at the same time then shifts back */
    /* #define ENABLE_RGB_MATRIX_HUE_PENDULUM        // Hue shifts up a slight ammount in a wave to the right then back to the left */
    /* #define ENABLE_RGB_MATRIX_HUE_WAVE            // Hue shifts up a slight ammount and then back down in a wave to the right */
    /* #define ENABLE_RGB_MATRIX_PIXEL_FRACTAL       // Single hue fractal filled keys pulsing horizontally out to edges */
    /* #define ENABLE_RGB_MATRIX_PIXEL_FLOW          // Pulsing #define ENABLE_RGB flow along LED wiring with random hues */
    /* #define ENABLE_RGB_MATRIX_PIXEL_RAIN          // Randomly light keys with random hues */
    /* #define ENABLE_RGB_MATRIX_TYPING_HEATMAP      // How hot is your WPM! */
    /* #define ENABLE_RGB_MATRIX_DIGITAL_RAIN        // That famous computer simulation */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE   // Pulses keys hit to hue & value then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE      // Static single hue pulses keys hit to shifted hue then fades to current hue */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       // Hue & value pulse near a single key hit then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  // Hue & value pulse near multiple key hits then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS      // Hue & value pulse the same column and row of a single key hit then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS // Hue & value pulse the same column and row of multiple key hits then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS      // Hue & value pulse away on the same column and row of a single key hit then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // Hue & value pulse away on the same column and row of multiple key hits then fades value out */
    /* #define ENABLE_RGB_MATRIX_SPLASH              // Full gradient & value pulse away from a single key hit then fades value out */
    /* #define ENABLE_RGB_MATRIX_MULTISPLASH         // Full gradient & value pulse away from multiple key hits then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_SPLASH        // Hue & value pulse away from a single key hit then fades value out */
    /* #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH   // Hue & value pulse away from multiple key hits then fades value out */
    /* #define ENABLE_RGB_MATRIX_STARLIGHT           // LEDs turn on and off at random at varying brightness maintaining user set color */
    /* #define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE  // LEDs turn on and off at random at varying brightness modifies user set hue by +- 30 */
    /* #define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT  // LEDs turn on and off at random at varying brightness modifies user set saturation by +- 30 */
    /* #define ENABLE_RGB_MATRIX_RIVERFLOW           // Modification to breathing animation, offset's animation depending on key location to simulate a river flowing */
            
#endif

#define ENCODER_RESOLUTION 2
//#define RETRO_SHIFT 180
