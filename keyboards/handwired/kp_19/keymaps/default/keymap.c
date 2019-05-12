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

#include "fft.c"
#include "analog.h"
#include "matrix.h"

enum layers {
    DEFAULT,
    VOLUP,
    VOLDN,
    AMPUP,
    AMPDN,
    VIB
};

enum custom_keycodes {
    KC_AMPUP = SAFE_RANGE,
    KC_AMPDOWN,
    KC_VIBUP,
    KC_VIBDOWN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEFAULT] = LAYOUT(
        LT(VOLUP, KC_MEDIA_REWIND),
        LT(VOLDN, KC_MEDIA_FAST_FORWARD),
        LT(AMPUP, KC_MEDIA_PLAY_PAUSE),
        LT(AMPDN, KC_MEDIA_PLAY_PAUSE)
    ),
    [VOLUP] = LAYOUT(
        _______, LT(VIB, KC__VOLUP), _______, _______
    ),
    [VOLDN] = LAYOUT(
        LT(VIB, KC__VOLDOWN), _______, _______, _______
    ),
    [AMPUP] = LAYOUT(
        _______, _______, _______, KC_AMPUP
    ),
    [AMPDN] = LAYOUT(
        _______, _______, KC_AMPDOWN, _______
    ),
    [VIB] = LAYOUT(
        _______, _______, KC_VIBDOWN, KC_VIBUP
    )
};

/* USER TASKS */

#define AUDIO_INPUT 8

#define POWER_OFF_DELAY 250 /* old value: 50 */
#define POWER_ON_DELAY  50
#define POWER_THRESHOLD 5

/* KICK LED */
#define KICK_BAND_FROM      1
#define KICK_BAND_TO        1
#define KICK_WINDOW_SIZE    10
#define KICK_COEFFICIENT    1.5 /* 0.45 -> 0.65 */
#define KICK_TH_WINDOW_SIZE 70
#define KICK_TH_COEFFICIENT 1.5 /* 0.3 */
#define KICK_ABS_TH         24
#define KICK_ALPHA    ((float)2 / (1 + KICK_WINDOW_SIZE))
#define KICK_TH_ALPHA ((float)2 / (1 + KICK_TH_WINDOW_SIZE))

/* RGB LED */
#define BLUE_BAND_FROM    4       /* 4 -> 5 -> 4 */
#define BLUE_BAND_TO      7       /* 9 -> 11 -> 10 -> 6 -> 7 */
#define RED_BAND_FROM     8       /* 10 -> 12 -> 11 -> 7 -> 8 */
#define RED_BAND_TO       24      /* 31 -> 34 -> 33 -> 34 -> 33 -> 30 -> 23 -> 25 -> 24 */
#define GREEN_BAND_FROM   25      /* 32 -> 35 -> 34 -> 35 -> 34 -> 31 -> 24 -> 26 -> 25 */
#define GREEN_BAND_TO     63
#define RGB_WINDOW_SIZE   40      /* 50 -> 30 -> 40 */
#define RGB_THRESHOLD     55      /* 55 -> 50 */
#define BLUE_COEFFICIENT  1.30    /* 0.95 -> 1.30 */
#define RED_COEFFICIENT   1.10    /* 0.95 -> 1.11 -> 1.10 */
#define GREEN_COEFFICIENT 1.15    /* 0.95 -> 1.11 -> 1.15 */
#define RGB_ALPHA ((float)2 / (1 + RGB_WINDOW_SIZE))

#define min(A, B) ((A) < (B) ? (A) : (B))
#define max(A, B) ((A) > (B) ? (A) : (B))

float amp = 1.0;
int vib_duration = 40;

void matrix_scan_user (void)
{
    /* ---- HAPTIC */

    static int vib_clear_timer = 0;
    static matrix_row_t matrix_prev[MATRIX_ROWS];
    matrix_row_t matrix_row = 0;
    matrix_row_t matrix_change = 0;

    if (vib_clear_timer) {
        if ((int)(vib_duration - timer_elapsed(vib_clear_timer)) < 0) {
            backlight_level(0);
            vib_clear_timer = 0;
        }
    }

    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        matrix_row = matrix_get_row(r);
        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                if (matrix_change & ((matrix_row_t)1<<c)) {
                    if (matrix_row & ((matrix_row_t)1<<c)) {
                        backlight_level(15);
                        vib_clear_timer = timer_read();
                    }
                    matrix_prev[r] ^= ((matrix_row_t)1<<c);
                }
            }
        }
    }

    /* ---- AUDIO-SYNC LED */

    static bool power = false;
    static uint8_t power_count;
    static float kick, kick_average, red, green, blue;

    int8_t re[128], im[128];
    uint8_t i;
    int16_t t, maximum;
    uint16_t abs[128];

    /* collect 128 samples (-128~+128 each) */
    for(i = maximum = 0; i < 128; i++)
    {
        t = (analogRead(AUDIO_INPUT) - 512) * amp;
        maximum = max(t, maximum);
        re[i] = (int8_t)t, im[i] = 0;
    }

    /* power control */
    if((power && maximum < POWER_THRESHOLD)
       || (!power && maximum >= POWER_THRESHOLD))
        power_count ++;
    if((power && power_count >= POWER_OFF_DELAY)
       || (!power && power_count >= POWER_ON_DELAY))
        power = !power, power_count = 0;

    /* FFT (7 = log2(128)) */
    fix_fft(re, im, 7);

    /* calculate absolute values (64 = 128 / 2) */
    for(i = 0; i < 64; i++)
        abs[i] = (int16_t)re[i]*re[i] + (int16_t)im[i]*im[i];

    /* kick led */
    for(t = 0, i = KICK_BAND_FROM; i <= KICK_BAND_TO; i++)
        t += abs[i];
    kick_average = t * KICK_TH_ALPHA + kick_average * (1 - KICK_TH_ALPHA);
    /* t = max(0, (t - kick_average * KICK_TH_COEFFICIENT) * KICK_COEFFICIENT); */
    t = t > kick_average * KICK_TH_COEFFICIENT && t > KICK_ABS_TH ? t * KICK_COEFFICIENT : 0;
    kick = t * KICK_ALPHA + kick * (1 - KICK_ALPHA);

    /* blue_led */
    for(t = 0, i = BLUE_BAND_FROM; i <= BLUE_BAND_TO; i++)
        t += abs[i];
    t = max((t - RGB_THRESHOLD) * BLUE_COEFFICIENT, 0);
    blue = t * RGB_ALPHA + blue * (1 - RGB_ALPHA);

    /* red_led */
    for(t = 0, i = RED_BAND_FROM; i <= RED_BAND_TO; i++)
        t += abs[i];
    t = max((t - RGB_THRESHOLD) * RED_COEFFICIENT, 0);
    red = t * RGB_ALPHA + red * (1 - RGB_ALPHA);

    /* green_led */
    for(t = 0, i = GREEN_BAND_FROM; i <= GREEN_BAND_TO; i++)
        t += abs[i];
    t = max((t - RGB_THRESHOLD) * GREEN_COEFFICIENT, 0);
    green = t * RGB_ALPHA + green * (1 - RGB_ALPHA);

    if (power) {
        rgblight_setrgb_at(min(kick, 255), min(kick, 255), min(kick, 255), 0);
        rgblight_setrgb_at(min(kick, 255), min(kick, 255), min(kick, 255), 4);
        /* rgblight_setrgb_at(0, 0, min(blue, 255), 1); */
        /* rgblight_setrgb_at(min(red, 255), 0, 0, 2); */
        /* rgblight_setrgb_at(0, min(green, 255), 0, 3); */
        rgblight_setrgb_at(min(red, 255), min(green, 255), min(blue, 255), 1);
        rgblight_setrgb_at(min(red, 255), min(green, 255), min(blue, 255), 2);
        rgblight_setrgb_at(min(red, 255), min(green, 255), min(blue, 255), 3);
    } else {
        rgblight_setrgb(0, 0, 0);
        rgblight_setrgb_at(0, 255, 0, 0);
        rgblight_setrgb_at(0, 255, 0, 1);
        rgblight_setrgb_at(0, 255, 0, 2);
        rgblight_setrgb_at(0, 255, 0, 3);
        rgblight_setrgb_at(0, 255, 0, 4);
    }
}

void matrix_init_user (void) {
    backlight_level(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
          case KC_AMPUP:
            if (amp < 10) amp += 0.5;
            return false;
          case KC_AMPDOWN:
            if (amp > 0) amp -= 0.5;
            return false;
          case KC_VIBUP:
            vib_duration += 5;
            return false;
          case KC_VIBDOWN:
            if (vib_duration > 0) vib_duration -= 5;
            return false;
        }
    }
    return true;
}
