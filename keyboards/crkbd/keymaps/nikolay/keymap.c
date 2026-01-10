#include QMK_KEYBOARD_H

// Custom keycodes for Globe key on hold
enum custom_keycodes {
    CAPS_G = SAFE_RANGE,  // tap=G, hold=Globe
    CAPS_H,               // tap=H, hold=Globe
    CAPS_PIPE,            // tap=|, hold=Globe
    CAPS_MINUS,           // tap=-, hold=Globe
    CAPS_5,               // tap=5, hold=Globe
    CAPS_6,               // tap=6, hold=Globe
    CAPS_NAV_G            // hold=Globe (no tap on nav layer)
};

#define ALT_DQUOT LALT_T(KC_QUOT)  // tap = ', hold = Alt
#define CTRL_LCBR RCTL_T(KC_LBRC)   // tap = {, hold = RCtrl
#define SHIFT_RCBR RSFT_T(KC_RBRC)  // tap = }, hold = RShift

static uint32_t keypress_counter = 0;
static bool caps_lock_active = false;  // Track if any Globe key is held

#ifdef OLED_ENABLE

// Поворачиваем дисплей для offhand (split keyboards)
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180; // Переворачиваем правую/левую половину
    }
    return rotation;
}

// Рендерим имя текущего слоя
void render_layer(void) {
    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case 0:
            oled_write_ln_P(PSTR("Layer: BASE"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("Layer: SYM "), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("Layer: NAV "), false);
            break;
        case 3:
            oled_write_ln_P(PSTR("Layer: NUM "), false);
            break;
        case 4:
            oled_write_ln_P(PSTR("Layer: FUN "), false);
            break;
        default:
            oled_write_ln_P(PSTR("Layer: ??? "), false);
    }
}

// OLED task — вызывается регулярно и рисует содержимое
bool oled_task_user(void) {
    // Можно рисовать разные вещи для master/non-master
    if (is_keyboard_master()) {
        render_layer();
        oled_write_P(PSTR("\n"), false);
        // Выводим счётчик нажатий
        char buf[32];
        snprintf(buf, sizeof(buf), "Keys: %lu", keypress_counter);
        oled_write_ln(buf, false);
    } else {
        // На второй половине можно рисовать логотип или дату
        oled_write_P(PSTR("Right half\n"), false);
        oled_write_P(PSTR("FFKB build"), false);
    }
    return false;
}

#endif // OLED_ENABLE

// Считаем нажатия клавиш (лог)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        keypress_counter++;
    }

    switch (keycode) {
        case ALT_DQUOT:
            if (record->event.pressed && record->tap.count && !record->tap.interrupted) {
                // Tap detected → send Shift + '
                tap_code16(LSFT(KC_QUOT));  // sends "
                return false;  // skip default processing
            }
            break;
        case CTRL_LCBR:
            if (record->event.pressed && record->tap.count && !record->tap.interrupted) {
                tap_code16(LSFT(KC_LBRC));  // sends {
                return false;               // skip default QMK processing
            }
            break;

        case SHIFT_RCBR:
            if (record->event.pressed && record->tap.count && !record->tap.interrupted) {
                tap_code16(LSFT(KC_RBRC));  // sends }
                return false;
            }
            break;

        // Globe key on hold keys
        case CAPS_G:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    // If tapped or any modifier is active (including another CAPS key), send G
                    tap_code(KC_G);
                    return false;
                } else {
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count && !get_mods()) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;

        case CAPS_H:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    // If tapped or any modifier is active (including another CAPS key), send H
                    tap_code(KC_H);
                    return false;
                } else {
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count && !get_mods()) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;

        case CAPS_PIPE:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    tap_code16(KC_PIPE);
                    return false;
                } else {
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count && !get_mods()) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;

        case CAPS_MINUS:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    tap_code(KC_PMNS);
                    return false;
                } else {
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count && !get_mods()) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;

        case CAPS_5:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    tap_code(KC_5);
                    return false;
                } else {
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count && !get_mods()) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;

        case CAPS_6:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    tap_code(KC_6);
                    return false;
                } else {
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count && !get_mods()) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;

        case CAPS_NAV_G:
            if (record->event.pressed) {
                if (record->tap.count) {
                    // If tapped, do nothing (placeholder for navigation layer)
                    return false;
                } else {
                    // Hold: activate Globe key even if modifiers are active
                    caps_lock_active = true;
                    register_code(KC_CAPS);
                }
            } else {
                if (!record->tap.count) {
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                }
            }
            return false;
    }
    return true;
}

// Per-key tapping term - longer for home row mods
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Home row mods - longer time for more reliable holds
        case LSFT_T(KC_A):
        case LCTL_T(KC_S):
        case LALT_T(KC_D):
        case LGUI_T(KC_F):
        case RGUI_T(KC_J):
        case RALT_T(KC_K):
        case RCTL_T(KC_L):
        case RSFT_T(KC_SCLN):
            return 220;
        // Other mod-taps - faster
        default:
            return 200;
    }
}

// Permissive hold only for home row mods
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_A):
        case LCTL_T(KC_S):
        case LALT_T(KC_D):
        case LGUI_T(KC_F):
        case RGUI_T(KC_J):
        case RALT_T(KC_K):
        case RCTL_T(KC_L):
        case RSFT_T(KC_SCLN):
            return true;
        default:
            return false;
    }
}

/* Combo K+L -> Esc */
const uint16_t PROGMEM kl_combo[] = {RALT_T(KC_K), RCTL_T(KC_L), COMBO_END};
combo_t key_combos[] = {
    COMBO(kl_combo, KC_ESC),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   LSFT_T(KC_A), LCTL_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), CAPS_G,    CAPS_H,    RGUI_T(KC_J), RALT_T(KC_K), RCTL_T(KC_L), RSFT_T(KC_SCLN), KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   HYPR_T(KC_Z), KC_X,    KC_C,    KC_V,    KC_B,                    KC_N,    KC_M,    KC_COMM, KC_DOT,  HYPR_T(KC_SLSH), KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_ESC,  LT(1,KC_TAB), LT(2,KC_SPC),  LT(4,KC_ENT), LT(3,KC_BSPC), KC_ESC
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [1] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   LSFT_T(KC_DOT), LCTL_T(KC_COMM), ALT_DQUOT, LGUI_T(KC_QUOT), CAPS_PIPE, CAPS_MINUS, RGUI_T(KC_PEQL), RALT_T(KC_GRV), CTRL_LCBR, SHIFT_RCBR, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_QUES, KC_SLSH, KC_LT,   KC_GT,   KC_BSLS,                     KC_UNDS, KC_PPLS, KC_TILD, KC_LBRC, KC_RBRC, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_NO,   KC_NO,     KC_ENT,  KC_DEL,  KC_ESC
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [2] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, CAPS_NAV_G,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   LGUI(KC_C), LGUI(KC_V), KC_NO,                 KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_NO,   KC_NO,     KC_ENT,  KC_DEL,  KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [3] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   LSFT_T(KC_1), LCTL_T(KC_2), LALT_T(KC_3), LGUI_T(KC_4), CAPS_5,    CAPS_6,    RGUI_T(KC_7), RALT_T(KC_8), RCTL_T(KC_9), RSFT_T(KC_0), KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_F11,  KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F12,  KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_ESC,  KC_TAB,  KC_SPC,    KC_NO,   KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [4] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_MPRV, KC_MNXT, KC_MPLY,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_RBT,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   KC_VOLD, KC_VOLU, KC_MUTE,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   KC_BRID, KC_BRIU, KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   DB_TOGG, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_ESC,  KC_TAB,  KC_SPC,    KC_NO,   KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    )
};
