#include QMK_KEYBOARD_H

// Fillers to make layering more clear
#define KC_____ KC_TRNS
#define KC_XXXX KC_NO
#define KC_FN   MO(1)
#define KC_LTOG RGB_TOG
#define KC_LMOD RGB_MOD
#define KC_LHUI RGB_HUI
#define KC_LHUD RGB_HUD
#define KC_LSAI RGB_SAI
#define KC_LSAD RGB_SAD
#define KC_LVAI RGB_VAI
#define KC_LVAD RGB_VAD

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_kc(
    JYEN, 1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    MINS, EQL,  BSPC, \
    TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC, RBRC, BSLS, \
    LCTL, A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,       ENT,  \
    LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH,       UP,   FN,   \
    LCTL, LALT, ESC,  LGUI,                   SPC,                    LEFT, DOWN, RGHT  \
  ),
  [1] = LAYOUT_kc(
    ____, F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  F11,  F12,  ____, \
    ____, LTOG, LHUI, LSAI, LVAI, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, \
    ____, LMOD, LHUD, LSAD, LVAD, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX,       ____, \
    ____, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX, XXXX,       PGUP, ____, \
    ____, ____, ____, ____,                   ____,                   HOME, PGDN, END   \
  ),
};
