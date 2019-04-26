#include "switch42.h"
#include "analog.h"

#include "fft.c"

extern uint8_t is_master;

/* GLOBAL VARS */

#define BASE      0
#define RAISE     1
#define FUNCTION  2
#define GARAKE    3
#define GARARAISE 4
#define TENKEY    5
#define MOUSE     6
#define WHEEL     7

#define L_BASE      0
#define L_RAISE     (2 << (RAISE - 1))
#define L_FUNCTION  (2 << (FUNCTION - 1))
#define L_GARAKE    (2 << (GARAKE - 1))
#define L_GARARAISE (2 << (GARARAISE - 1))
#define L_TENKEY    (2 << (TENKEY - 1))
#define L_MOUSE     (2 << (MOUSE - 1))
#define L_WHEEL     (2 << (WHEEL - 1))

/* FEATURES */

#ifdef TAP_DANCE_ENABLE
#include "./dance.c"
#endif

/* KEYCODE DEFINITIONS */

#define KC_____ KC_TRNS
#define KC_XXXX KC_NO

#define KC_D_MOUS  LT(MOUSE, KC_D)
#define KC_L1_RAI  LT(RAISE, KC_LANG1)
#define KC_ESC_FN  LT(FUNCTION, KC_ESC)
#define KC_L2_ALT  LALT_T(KC_LANG2)
#define KC_WEEL    MO(WHEEL)
#define KC_BASE    TO(BASE)
#define KC_GARAKE  TG(GARAKE)
#define KC_TENKEY  TG(TENKEY)
#define KC_ALT_EN  LALT_T(KC_ENT)
#define KC_CTL_TB  LCTL_T(KC_TAB)

#ifdef TAP_DANCE_ENABLE
#define KC_GK1     TD(TD_GARAKE1)
#define KC_GK2     TD(TD_GARAKE2)
#define KC_GK3     TD(TD_GARAKE3)
#define KC_GK4     TD(TD_GARAKE4)
#define KC_GK5     TD(TD_GARAKE5)
#define KC_GK6     TD(TD_GARAKE6)
#define KC_GK7     TD(TD_GARAKE7)
#define KC_GK8     TD(TD_GARAKE8)
#define KC_GK9     TD(TD_GARAKE9)
#define KC_GK0_RA  TD(TD_GARAKE0_RAISE)
#define KC_CTL_L1  TD(TD_CTL_LANG1)
#define KC_SFT_L2  TD(TD_SFT_LANG2)
#else
#define KC_GK1     KC_1
#define KC_GK2     KC_2
#define KC_GK3     KC_3
#define KC_GK4     KC_4
#define KC_GK5     KC_5
#define KC_GK6     KC_6
#define KC_GK7     KC_7
#define KC_GK8     KC_8
#define KC_GK9     KC_9
#define KC_GK0_RA  LT(RAISE, KC_0)
#define KC_CTL_L1  LCTL_T(KC_LANG1)
#define KC_SFT_L2  LSFT_T(KC_LANG2)
#endif

#define KC_RST  RESET
#define KC_MUP  KC_MS_U
#define KC_MDN  KC_MS_D
#define KC_MLFT KC_MS_L
#define KC_MRGT KC_MS_R
#define KC_WUP  KC_WH_U
#define KC_WDN  KC_WH_D
#define KC_WLFT KC_WH_L
#define KC_WRGT KC_WH_R
#define KC_ZNHN A(KC_ZKHK)

/* KEYMAPS */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[BASE] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    JYEN , Q    , W    , E    , R    , T    ,   Y    , U    , I    , O    , P    , MINS , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    LCTL , A    , S    ,D_MOUS, F    , G    ,   H    , J    , K    , L    , SCLN , QUOT , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    LSFT , Z    , X    , C    , V    , B    ,   N    , M    , COMM , DOT  , SLSH , BSLS , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                        L2_ALT, SPC  ,ESC_FN,   TAB  , ENT  ,L1_RAI \
//                     `--------------------' `--------------------'
),

[RAISE] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ZNHN , 1    , 2    , 3    , 4    , 5    ,   6    , 7    , 8    , 9    , 0    , EQL  , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , LCBR , LPRN , LBRC ,   RBRC , RPRN , RCBR , ____ , UP   , PLUS , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , EXLM , AT   , HASH , DLR  , PERC ,   CIRC , AMPR , ASTR , LEFT , DOWN , RGHT , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , BSPC , RST  ,  GARAKE, ____ , XXXX  \
//                     `--------------------' `--------------------'
),

[FUNCTION] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ____ , F1   , F2   , F3   , F4   , F5   ,   F6   , F7   , F8   , F9   , PSCR , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , LGUI ,   ____ , ____ , ____ , ____ , PGUP , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , HOME , PGDN , END , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , ____ , XXXX ,   ____ , ____ , ____  \
//                     `--------------------' `--------------------'
),

[GARAKE] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ____ , ____ , ____ , ____ , ____ , ____ ,   ESC  , GK7  , GK8  , GK9  , BSPC , MINS , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,  CTL_TB, GK4  , GK5  , GK6  , SCLN , QUOT , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,   LSFT , GK1  , GK2  , GK3  , DOT  , SLSH , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , ____ , BASE ,   SPC  ,ALT_EN,GK0_RA\
//                     `--------------------' `--------------------'
),

[GARARAISE] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ____ , ____ , ____ , ____ , ____ , ____ ,  TENKEY, BTN1 , MUP  , BTN2 , ZNHN , EQL  , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,  CTL_L1, MLFT , MDN  , MRGT , UP   , PLUS , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,  SFT_L2, WUP  , WDN  , LEFT , DOWN , RGHT , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , ____ , ____ ,   ____ , ____ , XXXX  \
//                     `--------------------' `--------------------'
),

[TENKEY] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , 7    , 8    , 9    , ____ , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , 4    , 5    , 6    , ____ , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , 1    , 2    , 3    , ____ , ____ , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , ____ , ____ ,   ____ , 0    , ____ \
//                     `--------------------' `--------------------'
),

[MOUSE] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , WEEL , ____ , ACL0 , ____ ,   MLFT , MDN  , MUP  , MRGT , ____ , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , BTN1 , BTN2 ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , ____ , ____ ,   ____ , ____ , ____  \
//                     `--------------------' `--------------------'
),

[WHEEL] = LAYOUT_kc( \
//,-----------------------------------------. ,-----------------------------------------.
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,   WRGT , WUP  , WDN  , WLFT , ____ , ____ , \
//|------+------+------+------+------+------| |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//`------+------+------+------+------+------| |------+------+------+------+------+------'
                         ____ , ____ , ____ ,   ____ , ____ , ____  \
//                     `--------------------' `--------------------'
),

};

/* USER TASKS */

#define AUDIO_INPUT 8
#define AMP 1                   /* MAKE THIS COFIGURABLE */

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
        t = (analogRead(AUDIO_INPUT) - 512) * AMP;
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
    return true;
}
