#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
  BASE = 0,
  RAISE,
  ARROW,
  ARROW_RAISE,
  FUNCTION,
  MOUSE,
  WHEEL
};

enum custom_keycodes {
  KC_ACCL = SAFE_RANGE,
  KC_WEEL
};

#define KC_____   KC_TRNS
#define KC_XXXX   KC_NO

#define KC_D_MOUS LT(MOUSE, KC_D)
#define KC_ALT_SP LALT_T(KC_SPC)
#define KC_RAI_B  LT(RAISE, KC_B)
#define KC_ARRAI  MO(ARROW_RAISE)
#define KC_S_UNDS LSFT_T(KC_UNDS)
#define KC_V_ARR  LT(ARROW, KC_V)

#define KC_RST  RESET
#define KC_MUP  KC_MS_U
#define KC_MDN  KC_MS_D
#define KC_MLFT KC_MS_L
#define KC_MRGT KC_MS_R
#define KC_WUP  KC_WH_U
#define KC_WDN  KC_WH_D
#define KC_WLFT KC_WH_L
#define KC_WRGT KC_WH_R

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Missing: QUOT, JYEN, MINS, LANG2, ESC, FN, TAB, LANG1 */

  [BASE] = LAYOUT_kc( \
//       ,-----------------------------------------. ,-----------------------------------------.
           Q    , W    , E    , R    , T    , LBRC ,   RBRC , Y    , U    , I    , O    , P    , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+------.
    LCTL , A    , S    ,D_MOUS, F    , G    ,                 H    , J    , K    , L    , SCLN , ENT  , \
//|------+------+------+------+------+------|               |------+------+------+------+------+------|
   S_UNDS, Z    , X    , C    , V_ARR,ALT_SP,                 RAI_B, N    , M    , COMM , DOT  , SLSH   \
//`-----------------------------------------'               `-----------------------------------------'
  ),

  /* Missing: EQL, RST */

  [RAISE] = LAYOUT_kc( \
//       ,-----------------------------------------. ,------------------------------------------.
           1    , 2    , 3    , 4    , 5    , LPRN ,   RPRN  , 6    , 7    , 8    , 9    , 0    , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+-------.
    ____ , ____ , BSLS , ____ , ____ , ____ ,                  ____ , ____ , ____ , PIPE , ____ , PLUS , \
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
    ____ , EXLM , AT   , HASH , DLR  , PERC ,                  CIRC , AMPR , ASTR , ____ , ____ , ____   \
//`------+------+------+------+------+------'                `------+------+------+------+------+------'
  ),

  [ARROW] = LAYOUT_kc( \
//       ,-----------------------------------------. ,-----------------------------------------.
           ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+------.
    ____ , ____ , ____ , ____ , ____ , ____ ,                 LEFT , DOWN , UP   , RGHT , ____ , ____ , \
//|------+------+------+------+------+------|               |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,                 ARRAI, ____ , ____ , ____ , ____ , ____   \
//`------+------+------+------+------+------'               `------+------+------+------+------+------'
  ),

  [ARROW_RAISE] = LAYOUT_kc( \
//       ,-----------------------------------------. ,-----------------------------------------.
    ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+------.
    ____ , ____ , ____ , ____ , ____ , ____ ,                 HOME , PGDN , PGUP , END  , ____ , ____ , \
//|------+------+------+------+------+------|               |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,                 ____ , ____ , ____ , ____ , ____ , ____   \
//`------+------+------+------+------+------'               `------+------+------+------+------+------'
  ),

  [FUNCTION] = LAYOUT_kc( \
//       ,-----------------------------------------. ,-----------------------------------------.
           F1   , F2   , F3   , F4   , F5   , ____ ,   ____ , F6   , F7   , F8   , F9   , PSCR , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+------.
    ____ , ____ , ____ , ____ , ____ , LGUI ,                 ____ , ____ , ____ , ____ , ____ , ____ , \
//|------+------+------+------+------+------|               |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , BSPC ,                 ____ , ____ , ____ , ____ , ____ , ____   \
//`------+------+------+------+------+------'               `------+------+------+------+------+------'
  ),

  [MOUSE] = LAYOUT_kc( \
//       ,-----------------------------------------. ,-----------------------------------------.
           ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+------.
    ____ , ____ , WEEL , ____ , ACCL , ____ ,                 MLFT , MDN  , MUP  , MRGT , ____ , ____ , \
//|------+------+------+------+------+------|               |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , BTN1 , BTN2 ,                 ____ , ____ , ____ , ____ , ____ , ____   \
//`------+------+------+------+------+------'               `------+------+------+------+------+------'
  ),

  [WHEEL] = LAYOUT_kc( \
//       ,-----------------------------------------. ,-----------------------------------------.
           ____ , ____ , ____ , ____ , ____ , ____ ,   ____ , ____ , ____ , ____ , ____ , ____ , \
//,------+------+------+------+------+------+------' `------+------+------+------+------+------+------.
    ____ , ____ , ____ , ____ , ____ , ____ ,                 WRGT , WUP  , WDN  , WLFT , ____ , ____ , \
//|------+------+------+------+------+------|               |------+------+------+------+------+------|
    ____ , ____ , ____ , ____ , ____ , ____ ,                 ____ , ____ , ____ , ____ , ____ , ____   \
//`------+------+------+------+------+------'               `------+------+------+------+------+------'
  ),

};

extern uint8_t mk_time_to_max, mk_wheel_time_to_max, mk_max_speed, mk_wheel_max_speed, mk_delay, mk_interval;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
   case KC_ACCL:
    if (record->event.pressed) {
      mk_max_speed = MOUSEKEY_ACL_MAX_SPEED;
      mk_wheel_max_speed = MOUSEKEY_ACL_WHEEL_MAX_SPEED;
      mk_time_to_max = 0;
      mk_wheel_time_to_max = 0;
    } else {
      mk_max_speed = MOUSEKEY_MAX_SPEED;
      mk_wheel_max_speed = MOUSEKEY_WHEEL_MAX_SPEED;
      mk_time_to_max = MOUSEKEY_TIME_TO_MAX;
      mk_wheel_time_to_max = MOUSEKEY_WHEEL_TIME_TO_MAX;
    }
    return false;
   case KC_WEEL:
    if (record->event.pressed) {
      mk_delay = MOUSEKEY_WHEEL_DELAY / 10;
      mk_interval = MOUSEKEY_WHEEL_INTERVAL;
      layer_on(WHEEL);
    } else {
      mk_delay = MOUSEKEY_DELAY / 10;
      mk_interval = MOUSEKEY_INTERVAL;
      layer_off(WHEEL);
    }
    break;
  }
  return true;
}
