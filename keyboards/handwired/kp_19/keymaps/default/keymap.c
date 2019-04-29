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

enum layers {
    DEFAULT,
    VOLUP,
    VOLDN,
    AMPUP,
    AMPDN
};

enum custom_keycodes {
    KC_AMPUP = SAFE_RANGE,
    KC_AMPDOWN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEFAULT] = LAYOUT(
        LT(VOLUP, KC_MEDIA_REWIND),
        LT(VOLDN, KC_MEDIA_FAST_FORWARD),
        LT(AMPUP, KC_MEDIA_PLAY_PAUSE),
        LT(AMPDN, KC_MEDIA_PLAY_PAUSE)
    ),
    [VOLUP] = LAYOUT(
        _______, KC__VOLUP, _______, _______
    ),
    [VOLDN] = LAYOUT(
        KC__VOLDOWN, _______, _______, _______
    ),
    [AMPUP] = LAYOUT(
        _______, _______, _______, KC_AMPUP
    ),
    [AMPDN] = LAYOUT(
        _______, _______, KC_AMPDOWN, _______
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

void matrix_scan_user (void) {
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

    backlight_level(min(kick / 16, 16));

    /* if (kick > 255) { */
    /*     register_code(KC_X); */
    /*     unregister_code(KC_X); */
    /* } */
    /*  */
    /* switch((int)min(kick, 255) / 16) { */
    /*   case 0: register_code(KC_0); unregister_code(KC_0); break; */
    /*   case 1: register_code(KC_1); unregister_code(KC_1); break; */
    /*   case 2: register_code(KC_2); unregister_code(KC_2); break; */
    /*   case 3: register_code(KC_3); unregister_code(KC_3); break; */
    /*   case 4: register_code(KC_4); unregister_code(KC_4); break; */
    /*   case 5: register_code(KC_5); unregister_code(KC_5); break; */
    /*   case 6: register_code(KC_6); unregister_code(KC_6); break; */
    /*   case 7: register_code(KC_7); unregister_code(KC_7); break; */
    /*   case 8: register_code(KC_8); unregister_code(KC_8); break; */
    /*   case 9: register_code(KC_9); unregister_code(KC_9); break; */
    /*   case 10: register_code(KC_A); unregister_code(KC_A); break; */
    /*   case 11: register_code(KC_B); unregister_code(KC_B); break; */
    /*   case 12: register_code(KC_C); unregister_code(KC_C); break; */
    /*   case 13: register_code(KC_D); unregister_code(KC_D); break; */
    /*   case 14: register_code(KC_E); unregister_code(KC_E); break; */
    /*   case 15: register_code(KC_F); unregister_code(KC_F); break; */
    /* } */
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
        }
    }
    return true;
}
