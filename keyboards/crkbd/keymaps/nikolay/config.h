#pragma once

#define SPLIT_USB_DETECT

// Home row mod timing - balanced for fast typing and reliable holds
#define TAPPING_TERM 200                // Base timing for tap vs hold (200ms)
#define TAPPING_TERM_PER_KEY            // Enable per-key tapping term
#define PERMISSIVE_HOLD_PER_KEY         // Enable per-key permissive hold (home row only)

#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"

#define COMBO_COUNT 1
#define COMBO_TERM 50
// #define OLED_DISPLAY_128X64
#define OLED_TIMEOUT 20000              // Turns off OLED after said amount of milliseconds
#define OLED_BRIGHTNESS 128