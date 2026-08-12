
#include QMK_KEYBOARD_H
// #include "combo.h"

enum layer_names {
    _BASE,
    _SYM,
    _NAV,
    _NUM,
    _FUN
};

/* Combo J+K -> Backspace */
// const uint16_t PROGMEM jk_combo[] = {R11, R12, COMBO_END};
// combo_t key_combos[] = {
//     COMBO(jk_combo, KC_BSPC),
// };

/* Combo K+L -> Esc */
const uint16_t PROGMEM kl_combo[] = {RGUI_T(KC_K), RCTL_T(KC_L), COMBO_END};
combo_t key_combos[] = {
    COMBO(kl_combo, KC_ESC),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x5_3(
  //┌────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┐
      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      LALT_T(KC_A), LCTL_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F), KC_G,    KC_H,    RSFT_T(KC_J), RGUI_T(KC_K), RCTL_T(KC_L), RALT_T(KC_SCLN),
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      HYPR_T(KC_Z), KC_X,    KC_C,    KC_V,    KC_B,                    KC_N,    KC_M,    KC_COMM, KC_DOT,  HYPR_T(KC_SLSH),
  //└────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┘
                                 KC_ESC,  LT(_SYM,KC_TAB), LT(_NAV,KC_SPC),  LT(_FUN,KC_ENT), LT(_NUM,KC_BSPC), KC_ESC
  //                            └────────┴────────┴───────┘  └────────┴────────┴────────┘
    ),

    [_SYM] = LAYOUT_split_3x5_3(
  //┌────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┐
      KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      LALT_T(S(KC_COMM)), LCTL_T(S(KC_DOT)), LGUI_T(KC_GRV), LSFT_T(KC_QUOT), KC_BSLS, KC_PMNS, RSFT_T(KC_PEQL), RGUI_T(KC_SCLN), RCTL_T(KC_LBRC), RALT_T(KC_RBRC),
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      KC_QUES, KC_SLSH, KC_TILD, KC_DQUO, KC_PIPE,                     KC_UNDS, KC_PPLS, KC_COLN, KC_LCBR, KC_RCBR,
  //└────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┘
                                 KC_NO,   KC_NO,   KC_NO,     KC_ENT,  KC_DEL,  KC_ESC
  //                            └────────┴────────┴───────┘  └────────┴────────┴────────┘
    ),

    [_NAV] = LAYOUT_split_3x5_3(
  //┌────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      KC_LALT, KC_LCTL, KC_LGUI, KC_LSFT, KC_NO,                        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   LGUI(KC_C), LGUI(KC_V), KC_NO,                 KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //└────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┘
                                 KC_NO,   KC_NO,   KC_NO,     KC_ENT,  KC_DEL,  KC_NO
  //                            └────────┴────────┴───────┘  └────────┴────────┴────────┘
    ),

    [_NUM] = LAYOUT_split_3x5_3(
  //┌────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┐
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      LALT_T(KC_1), LCTL_T(KC_2), LGUI_T(KC_3), LSFT_T(KC_4), KC_5,    KC_6,    RSFT_T(KC_7), RGUI_T(KC_8), RCTL_T(KC_9), RALT_T(KC_0),
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      KC_F11,  KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F12,
  //└────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┘
                                 KC_ESC,  KC_TAB,  KC_SPC,    KC_NO,   KC_NO,   KC_NO
  //                            └────────┴────────┴───────┘  └────────┴────────┴────────┘
    ),

    [_FUN] = LAYOUT_split_3x5_3(
  //┌────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_MRWD, KC_MFFD, KC_MPLY,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_RBT,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_VOLD, KC_VOLU, KC_MUTE,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT,
  //├────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_BRID, KC_BRIU, KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   DB_TOGG,
  //└────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┘
                                 KC_ESC,  KC_TAB,  KC_SPC,    KC_NO,   KC_NO,   KC_NO
  //                            └────────┴────────┴───────┘  └────────┴────────┴────────┘
    )
};

// Per-key tapping term - longer for home row mods
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Home row mods - longer time for more reliable holds
        case LALT_T(KC_A):
        case LCTL_T(KC_S):
        case LGUI_T(KC_D):
        case LSFT_T(KC_F):
        case RSFT_T(KC_J):
        case RGUI_T(KC_K):
        case RCTL_T(KC_L):
        case RALT_T(KC_SCLN):
            return 220;
        // Other mod-taps - faster
        default:
            return 200;
    }
}

// Permissive hold only for home row mods
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LALT_T(KC_A):
        case LCTL_T(KC_S):
        case LGUI_T(KC_D):
        case LSFT_T(KC_F):
        case RSFT_T(KC_J):
        case RGUI_T(KC_K):
        case RCTL_T(KC_L):
        case RALT_T(KC_SCLN):
            return true;
        default:
            return false;
    }
}
