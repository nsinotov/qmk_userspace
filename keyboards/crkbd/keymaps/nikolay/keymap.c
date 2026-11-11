// Corne (crkbd/rev1) keymap — same layout as the Sweep / KB38 / Urchin ZMK keymaps,
// on the 3x5+2 core, plus Corne extras (outer columns, 3rd thumb keys, OLED).
//
// Requires QMK 0.29+ (Chordal Hold, Flow Tap, MS_* mouse keycodes).

#include QMK_KEYBOARD_H

enum layers { BASE, SYM, NAV, NUM, MOUSE, SCROLL, FUN };

enum custom_keycodes {
    OS_COPY = SAFE_RANGE, // Cmd+C on macOS, Ctrl+C in Windows/Linux mode
    OS_TOGG,              // toggle macOS / Windows-Linux mode (saved to EEPROM)
    UNSTICK,              // release all modifiers and the Globe key
};

// ---- Home row mods (order ⌥ ⇧ ⌃ ⌘, mirrored on the right) ----
#define HM_A    LALT_T(KC_A)
#define HM_S    LSFT_T(KC_S)
#define HM_D    LCTL_T(KC_D)
#define HM_F    LGUI_T(KC_F)
#define HM_G    HYPR_T(KC_G)
#define HM_H    HYPR_T(KC_H)
#define HM_J    RGUI_T(KC_J)
#define HM_K    RCTL_T(KC_K)
#define HM_L    RSFT_T(KC_L)
#define HM_SCLN RALT_T(KC_SCLN)

// SYM home row (< and > are shifted, so their taps are intercepted below)
#define HS_LT   LALT_T(KC_COMM)
#define HS_GT   LSFT_T(KC_DOT)
#define HS_GRV  LCTL_T(KC_GRV)
#define HS_QUOT LGUI_T(KC_QUOT)
#define HS_EQL  RGUI_T(KC_EQL)
#define HS_SCLN RCTL_T(KC_SCLN)
#define HS_LBRC RSFT_T(KC_LBRC)
#define HS_RBRC RALT_T(KC_RBRC)

// NUM home row
#define HN_1    LALT_T(KC_1)
#define HN_2    LSFT_T(KC_2)
#define HN_3    LCTL_T(KC_3)
#define HN_4    LGUI_T(KC_4)
#define HN_7    RGUI_T(KC_7)
#define HN_8    RCTL_T(KC_8)
#define HN_9    RSFT_T(KC_9)
#define HN_0    RALT_T(KC_0)

// ---- Thumb layer-taps (tap, then hold to auto-repeat the tap key) ----
#define TH_TAB  LT(SYM, KC_TAB)
#define TH_SPC  LT(NAV, KC_SPC)
#define TH_ENT  LT(MOUSE, KC_ENT)
#define TH_BSPC LT(NUM, KC_BSPC)

// ---- Globe on hold (macOS: Caps Lock is remapped to Globe in System Settings) ----
// LT(0, ...) is only a tap/hold container; both tap and hold are handled in process_record_user.
#define GL_V    LT(0, KC_V)    // tap V, hold Globe
#define GL_M    LT(0, KC_M)    // tap M, hold Globe
#define GL_PSTE LT(0, KC_PSTE) // tap OS paste, hold Globe (NAV layer)

typedef union {
    uint32_t raw;
    struct {
        bool win_mode : 1;
    };
} user_config_t;

static user_config_t user_config;
static uint16_t      globe_owner      = KC_NO; // Globe key currently holding Caps Lock
static uint32_t      keypress_counter = 0;

static uint16_t os_shortcut(uint16_t key) {
    return user_config.win_mode ? LCTL(key) : LGUI(key);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
}

void eeconfig_init_user(void) {
    user_config.raw = 0;
    eeconfig_update_user(user_config.raw);
}

// Tap sends `tap_keycode`; hold sends Globe, unless a modifier or another Globe key is active.
static bool process_globe(uint16_t keycode, uint16_t tap_keycode, keyrecord_t *record) {
    if (record->tap.count) {
        if (record->event.pressed) {
            tap_code16(tap_keycode);
        }
        return false;
    }
    if (record->event.pressed) {
        if (get_mods() || globe_owner != KC_NO) {
            tap_code16(tap_keycode);
        } else {
            globe_owner = keycode;
            register_code(KC_CAPS);
        }
    } else if (globe_owner == keycode) {
        globe_owner = KC_NO;
        unregister_code(KC_CAPS);
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        keypress_counter++;
    }

    switch (keycode) {
        case HS_LT:
        case HS_GT:
            if (record->tap.count && record->event.pressed) {
                tap_code16(keycode == HS_LT ? KC_LT : KC_GT);
                return false;
            }
            return true;

        case GL_V:
            return process_globe(keycode, KC_V, record);
        case GL_M:
            return process_globe(keycode, KC_M, record);
        case GL_PSTE:
            return process_globe(keycode, os_shortcut(KC_V), record);

        case OS_COPY:
            if (record->event.pressed) {
                tap_code16(os_shortcut(KC_C));
            }
            return false;

        case OS_TOGG:
            if (record->event.pressed) {
                user_config.win_mode = !user_config.win_mode;
                eeconfig_update_user(user_config.raw);
            }
            return false;

        case UNSTICK:
            if (record->event.pressed) {
                clear_mods();
                clear_weak_mods();
                clear_oneshot_mods();
                if (globe_owner != KC_NO) {
                    globe_owner = KC_NO;
                    unregister_code(KC_CAPS);
                }
                send_keyboard_report();
            }
            return false;
    }
    return true;
}

// ---- Tap-hold tuning (mirrors the ZMK hold-tap settings) ----

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_TAB:
        case TH_SPC:
        case TH_ENT:
        case TH_BSPC:
        case GL_V:
        case GL_M:
        case GL_PSTE:
            return 200;
        default:
            return TAPPING_TERM; // home row mods and hyper
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_TAB:
        case TH_SPC:
        case TH_ENT:
        case TH_BSPC:
            return 200; // tap, then hold within 200 ms to auto-repeat
        default:
            return QUICK_TAP_TERM;
    }
}

// Like ZMK require-prior-idle-ms: a mod-tap pressed soon after another key is a tap.
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    switch (keycode) {
        case HM_S:
        case HM_L:
        case HS_GT:
        case HS_LBRC:
        case HN_2:
        case HN_9:
            return 125; // Shift activates more eagerly
        case HM_A: case HM_D: case HM_F: case HM_G:
        case HM_H: case HM_J: case HM_K: case HM_SCLN:
        case HS_LT: case HS_GRV: case HS_QUOT:
        case HS_EQL: case HS_SCLN: case HS_RBRC:
        case HN_1: case HN_3: case HN_4:
        case HN_7: case HN_8: case HN_0:
            return FLOW_TAP_TERM;
        default:
            return 0; // thumbs and Globe keys
    }
}

// Bilateral home row mods: holds only trigger with a key from the other hand
// (thumbs count as both). Hyper triggers with any key.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_split_3x6_3(
    'L', 'L', 'L', 'L', 'L', 'L',            'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L',            'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L',            'R', 'R', 'R', 'R', 'R', 'R',
                        '*', '*', '*',  '*', '*', '*'
);

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    if (tap_hold_keycode == HM_G || tap_hold_keycode == HM_H) {
        return true;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}

// ---- Combos: S+D -> Esc (BASE, MOUSE), K+L -> Esc (BASE) ----

enum combo_events { COMBO_ESC_SD, COMBO_ESC_KL };

const uint16_t PROGMEM esc_sd_combo[] = {HM_S, HM_D, COMBO_END};
const uint16_t PROGMEM esc_kl_combo[] = {HM_K, HM_L, COMBO_END};

combo_t key_combos[] = {
    [COMBO_ESC_SD] = COMBO(esc_sd_combo, KC_ESC),
    [COMBO_ESC_KL] = COMBO(esc_kl_combo, KC_ESC),
};

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    uint8_t layer = get_highest_layer(layer_state);
    switch (combo_index) {
        case COMBO_ESC_SD:
            return layer == BASE || layer == MOUSE;
        case COMBO_ESC_KL:
            return layer == BASE;
    }
    return true;
}

// ---- OLED ----

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return is_keyboard_master() ? rotation : OLED_ROTATION_180;
}

static void render_layer(void) {
    static const char *const names[] = {
        [BASE] = "BASE", [SYM] = "SYM", [NAV] = "NAV", [NUM] = "NUM",
        [MOUSE] = "MOUSE", [SCROLL] = "SCROLL", [FUN] = "FUN",
    };
    uint8_t layer = get_highest_layer(layer_state);
    oled_write_P(PSTR("Layer: "), false);
    oled_write_ln(layer <= FUN ? names[layer] : "???", false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        char buf[22];
        render_layer();
        oled_write_ln_P(user_config.win_mode ? PSTR("OS:    Win/Linux") : PSTR("OS:    macOS"), false);
        snprintf(buf, sizeof(buf), "Keys:  %lu", (unsigned long)keypress_counter);
        oled_write_ln(buf, false);
    } else {
        oled_write_ln_P(PSTR("Corne"), false);
    }
    return false;
}

#endif // OLED_ENABLE

// ---- Keymap ----

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   HM_A,    HM_S,    HM_D,    HM_F,    HM_G,                         HM_H,    HM_J,    HM_K,    HM_L,    HM_SCLN, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_Z,    KC_X,    KC_C,    GL_V,    KC_B,                         KC_N,    GL_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          CW_TOGG, TH_TAB,  TH_SPC,    TH_ENT,  TH_BSPC, CW_TOGG
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [SYM] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   HS_LT,   HS_GT,   HS_GRV,  HS_QUOT, KC_BSLS,                      KC_MINS, HS_EQL,  HS_SCLN, HS_LBRC, HS_RBRC, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_QUES, KC_SLSH, KC_TILD, KC_DQUO, KC_PIPE,                      KC_UNDS, KC_PLUS, KC_COLN, KC_LCBR, KC_RCBR, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,   KC_ESC
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [NAV] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_CAPS, CW_TOGG, KC_NO,                        KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_TRNS, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_LALT, KC_LSFT, KC_LCTL, KC_LGUI, KC_NO,                        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TRNS, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   OS_COPY, GL_PSTE, KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   KC_NO,   KC_NO,     MO(FUN), KC_DEL,  KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [NUM] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   HN_1,    HN_2,    HN_3,    HN_4,    KC_5,                         KC_6,    HN_7,    HN_8,    HN_9,    HN_0,    KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_F11,  KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F12,  KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_ESC,  KC_NO,   KC_NO,     KC_NO,   KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [MOUSE] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   MS_UP,   KC_NO,   KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_LALT, KC_LSFT, KC_LCTL, KC_LGUI, MO(SCROLL),                   KC_NO,   MS_LEFT, MS_DOWN, MS_RGHT, KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_NO,   MS_BTN2, MS_BTN1,   KC_TRNS, KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [SCROLL] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, KC_TRNS, MS_WHLU, KC_TRNS, KC_TRNS, KC_TRNS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, MS_WHLL, MS_WHLD, MS_WHLR, KC_TRNS, KC_TRNS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    ),

    [FUN] = LAYOUT_split_3x6_3(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_NO,   KC_NO,   OS_TOGG, KC_MPRV, KC_MNXT, KC_MPLY,                      KC_NO,   KC_NO,   KC_NO,   UNSTICK, QK_RBT,  KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   KC_VOLD, KC_VOLU, KC_MUTE,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT, KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                    ├────────┼────────┼────────┼────────┼────────┼────────┤
      KC_NO,   KC_NO,   KC_NO,   KC_BRID, KC_BRIU, KC_NO,                        KC_NO,   KC_NO,   KC_NO,   KC_NO,   DB_TOGG, KC_NO,
  //└────────┴────────┴────────┴────────┼────────┼────────┼────────┐  ┌────────┼────────┼────────┼────────┴────────┴────────┴────────┘
                                          KC_ESC,  KC_NO,   KC_NO,     KC_NO,   KC_NO,   KC_NO
  //                                    └────────┴────────┴────────┘  └────────┴────────┴────────┘
    )
};
