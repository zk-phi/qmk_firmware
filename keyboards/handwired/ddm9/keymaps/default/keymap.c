/* Copyright 2019 zk-phi
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
#include QMK_KEYBOARD_H

enum layers {
    DEFAULT,
    RAISE,
    LOWER
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEFAULT] = LAYOUT(
        KC_G, KC_H, KC_I,
        KC_D, KC_E, KC_F,
        LT(LOWER, KC_A), KC_B, LT(RAISE, KC_C)
    ),
    [RAISE] = LAYOUT(
        RGB_HUI, RGB_SAI, RGB_VAI,
        RGB_HUD, RGB_SAD, RGB_VAD,
        RGB_SPI, RGB_SPD, KC_NO
    ),
    [LOWER] = LAYOUT(
        KC_NO, KC_NO,  KC_NO,
        KC_NO,  RGB_MOD, RGB_TOG,
        KC_NO,  KC_NO,   RESET
    )
};
