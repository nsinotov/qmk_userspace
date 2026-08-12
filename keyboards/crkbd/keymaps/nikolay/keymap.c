#include QMK_KEYBOARD_H

// Custom keycodes for Globe key on hold
enum custom_keycodes {
    CAPS_V = SAFE_RANGE,  // tap=V, hold=Globe
    CAPS_M,               // tap=M, hold=Globe
    CAPS_NAV_V            // tap=⌘V, hold=Globe (nav layer)
};

#define ALT_LT   LALT_T(KC_COMM)    // tap = <, hold = LAlt (custom intercept)
#define CTL_GT   LCTL_T(KC_DOT)     // tap = >, hold = LCtrl (custom intercept)

static uint32_t keypress_counter = 0;
static bool caps_lock_active = false;  // Track if any Globe key is held
static uint16_t caps_nav_v_timer = 0;  // Timer for CAPS_NAV_V tap/hold detection
static bool caps_nav_v_is_hold = false;  // Track if CAPS_NAV_V was held long enough

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
            oled_write_ln_P(PSTR("Layer: MOUSE"), false);
            break;
        case 5:
            oled_write_ln_P(PSTR("Layer: SCRL"), false);
            break;
        case 6:
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
        case ALT_LT:
            if (record->event.pressed && record->tap.count && !record->tap.interrupted) {
                tap_code16(LSFT(KC_COMM));  // sends <
                return false;
            }
            break;
        case CTL_GT:
            if (record->event.pressed && record->tap.count && !record->tap.interrupted) {
                tap_code16(LSFT(KC_DOT));   // sends >
                return false;
            }
            break;

        // Globe key on hold keys
        case CAPS_V:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    // If tapped or any modifier is active (including another CAPS key), send V
                    tap_code(KC_V);
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

        case CAPS_M:
            if (record->event.pressed) {
                if (record->tap.count || get_mods() || caps_lock_active) {
                    // If tapped or any modifier is active (including another CAPS key), send M
                    tap_code(KC_M);
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

        case CAPS_NAV_V:
            if (record->event.pressed) {
                // Key pressed - start timer
                caps_nav_v_timer = timer_read();
                caps_nav_v_is_hold = false;
            } else {
                // Key released
                if (caps_nav_v_is_hold) {
                    // Was a hold - release Caps Lock
                    caps_lock_active = false;
                    unregister_code(KC_CAPS);
                    caps_nav_v_is_hold = false;
                } else {
                    // Was a tap - send ⌘V
                    tap_code16(LGUI(KC_V));
                }
                // Reset timer
                caps_nav_v_timer = 0;
            }
            return false;
    }
    return true;
}

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

// Matrix scan to handle CAPS_NAV_V hold detection
void matrix_scan_user(void) {
    if (caps_nav_v_timer && !caps_nav_v_is_hold) {
        // Check if TAPPING_TERM has elapsed
        if (timer_elapsed(caps_nav_v_timer) > TAPPING_TERM) {
            // Held long enough - activate Caps Lock
            caps_nav_v_is_hold = true;
            caps_lock_active = true;
            register_code(KC_CAPS);
        }
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, 2, 4, 6);
    return state;
}

/* Combos: S+D -> Esc, K+L -> Esc */
const uint16_t PROGMEM sd_combo[] = {LCTL_T(KC_S), LGUI_T(KC_D), COMBO_END};
const uint16_t PROGMEM kl_combo[] = {RGUI_T(KC_K), RCTL_T(KC_L), COMBO_END};
combo_t key_combos[] = {
    COMBO(sd_combo, KC_ESC),
    COMBO(kl_combo, KC_ESC),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   LALT_T(KC_A), LCTL_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F), HYPR_T(KC_G),  HYPR_T(KC_H), RSFT_T(KC_J), RGUI_T(KC_K), RCTL_T(KC_L), RALT_T(KC_SCLN), KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_Z,    KC_X,    KC_C,    CAPS_V,  KC_B,                         KC_N,    CAPS_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_CAPS, LT(1,KC_TAB), LT(2,KC_SPC),  LT(4,KC_ENT), LT(3,KC_BSPC), CW_TOGG
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [1] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   ALT_LT, CTL_GT, LGUI_T(KC_GRV), LSFT_T(KC_QUOT), KC_BSLS,       KC_PMNS, RSFT_T(KC_PEQL), RGUI_T(KC_SCLN), RCTL_T(KC_LBRC), RALT_T(KC_RBRC), KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_QUES, KC_SLSH, KC_TILD, KC_DQUO, KC_PIPE,                     KC_UNDS, KC_PPLS, KC_COLN, KC_LCBR, KC_RCBR, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_NO,   KC_NO,     KC_ENT,  KC_DEL,  KC_ESC
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [2] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_CAPS, KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_LALT, KC_LCTL, KC_LGUI, KC_LSFT, KC_NO,                        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   LGUI(KC_C), CAPS_NAV_V, KC_NO,                 KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_NO,   KC_NO,     KC_TRNS, KC_DEL,  KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [3] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   LALT_T(KC_1), LCTL_T(KC_2), LGUI_T(KC_3), LSFT_T(KC_4), KC_5,       KC_6,    RSFT_T(KC_7), RGUI_T(KC_8), RCTL_T(KC_9), RALT_T(KC_0), KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_F11,  KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F12,  KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_ESC,  KC_TAB,  KC_SPC,    KC_NO,   KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [4] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_MS_U, KC_NO,   KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_LALT, KC_LCTL, KC_LGUI, KC_LSFT, MO(5),                        KC_NO,   KC_MS_L, KC_MS_D, KC_MS_R, KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_BTN2, KC_BTN1,   KC_TRNS, KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [5] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, KC_TRNS, KC_WH_U, KC_TRNS, KC_TRNS, KC_TRNS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, KC_WH_L, KC_WH_D, KC_WH_R, KC_TRNS, KC_TRNS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [6] = LAYOUT_split_3x6_3(
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
