#pragma once

#define SPLIT_USB_DETECT

// ---- Tap-hold (mirrors the ZMK hold-tap settings) ----
#define TAPPING_TERM 220         // home row mods and hyper; thumbs use 200 (per key)
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 150       // thumbs use 200 for tap-then-hold repeat (per key)
#define QUICK_TAP_TERM_PER_KEY
#define PERMISSIVE_HOLD          // like ZMK "balanced" flavor
#define CHORDAL_HOLD             // bilateral home row mods (opposite-hand holds only)
#define FLOW_TAP_TERM 200        // like ZMK require-prior-idle-ms; Shift uses 125 (per key)

// ---- Combos ----
#define COMBO_TERM 50
#define COMBO_ONLY_FROM_LAYER 0  // combos use BASE keycodes on every layer
#define COMBO_SHOULD_TRIGGER     // per-layer filtering in combo_should_trigger()

// ---- OLED ----
#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"
#define OLED_TIMEOUT 20000       // turn the OLED off after 20 s of inactivity
#define OLED_BRIGHTNESS 128
