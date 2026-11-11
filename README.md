# QMK Userspace — Corne Keymap

QMK [userspace](https://docs.qmk.fm/newbs_external_userspace) with my keymap for the [Corne (crkbd)](https://github.com/foostan/crkbd) 42-key split keyboard.

The keymap follows the same layout as my ZMK keyboards ([Sweep](https://github.com/nsinotov/sweep-split-keyboard), [KB38](https://github.com/nsinotov/kb38-cirque-zmk), [Urchin](https://github.com/nsinotov/urchin-zmk-firmware)) on the 3x5+2 core. The Corne's outer columns, third thumb keys and OLED add a few extras.

## Hardware

| Component | Specification |
|-----------|---------------|
| Keyboard | Corne (crkbd) rev1 — `crkbd/rev1` |
| Layout | `LAYOUT_split_3x6_3` (6 columns x 3 rows + 3 thumb keys per side) |
| Connection | USB-C, wired split (TRRS) |
| Display | SSD1306 128x32 OLED on each half (I2C) |
| Keys | 42 (the outer columns are unused) |

## Firmware

### Keymap

7 layers with home row mods, mouse keys, and a function layer, plus a macOS / Windows-Linux toggle (see [OS Mode](#os-mode)).

```
BASE (QWERTY + home row mods)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │  Q  │  W  │  E  │  R  │  T  │   │  Y  │  U  │  I  │  O  │  P  │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │ ⌥/A │ ⇧/S │ ⌃/D │ ⌘/F │HYP/G│   │HYP/H│ ⌘/J │ ⌃/K │ ⇧/L │ ⌥/; │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │  Z  │  X  │  C  │GL/V │  B  │   │  N  │GL/M │  ,  │  .  │  /  │     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │ CW  │S/TAB│N/SPC│   │M/RET│#/BSP│ CW  │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘
  S=SYM  N=NAV  M=MOUSE  #=NUM   (hold = layer, tap = key)
  GL = Globe on hold (macOS)  CW = Caps Word

SYM (symbols)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │  !  │  @  │  #  │  $  │  %  │   │  ^  │  &  │  *  │  (  │  )  │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │ ⌥/< │ ⇧/> │ ⌃/` │ ⌘/' │  \  │   │  -  │ ⌘/= │ ⌃/; │ ⇧/[ │ ⌥/] │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │  ?  │  /  │  ~  │  "  │  |  │   │  _  │  +  │  :  │  {  │  }  │     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │     │     │     │   │     │     │ ESC │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘

NAV (navigation)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │     │     │CAPS │CAPWD│     │   │HOME │PG_DN│PG_UP│ END │     │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │  ⌥  │  ⇧  │  ⌃  │  ⌘  │     │   │  ←  │  ↓  │  ↑  │  →  │     │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │     │     │COPY │PASTE│     │   │     │     │     │     │     │     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │     │     │     │   │ FUN │ DEL │     │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘
  PASTE: hold for Globe (macOS)

NUM (numbers + function keys)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │ F1  │ F2  │ F3  │ F4  │ F5  │   │ F6  │ F7  │ F8  │ F9  │ F10 │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │ ⌥/1 │ ⇧/2 │ ⌃/3 │ ⌘/4 │  5  │   │  6  │ ⌘/7 │ ⌃/8 │ ⇧/9 │ ⌥/0 │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │ F11 │     │     │     │     │   │     │     │     │     │ F12 │     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │ ESC │     │     │   │     │     │     │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘

MOUSE (mouse movement — right hand IJKL style)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │     │     │     │     │     │   │     │     │  ↑  │     │     │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │  ⌥  │  ⇧  │  ⌃  │  ⌘  │SCRL │   │     │  ←  │  ↓  │  →  │     │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │     │     │     │     │     │   │     │     │     │     │     │     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │     │RCLK │LCLK │   │     │     │     │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘

SCROLL (hold SCRL on MOUSE layer — scroll with right hand IJKL)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │     │     │     │     │     │   │     │     │ S↑  │     │     │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │  ⌥  │  ⇧  │  ⌃  │  ⌘  │(hld)│   │     │ S←  │ S↓  │ S→  │     │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │     │     │     │     │     │   │     │     │     │     │     │     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │     │RCLK │LCLK │   │     │     │     │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘
  Unlisted keys fall through to MOUSE.

FUN (hold ENTER on NAV layer)
┌─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┬─────┬─────┬─────┐
│     │     │ WIN │PREV │NEXT │PLAY │   │     │     │     │USTCK│RESET│     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │     │     │VOL- │VOL+ │MUTE │   │     │     │     │     │BOOT │     │
├─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┼─────┼─────┤
│     │     │     │BRI- │BRI+ │     │   │     │     │     │     │DEBUG│     │
└─────┴─────┴─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │ ESC │     │     │   │     │     │     │
                  └─────┴─────┴─────┘   └─────┴─────┴─────┘
```

### OS Mode

COPY and PASTE on the NAV layer default to macOS shortcuts (Cmd+C / Cmd+V).
Press **WIN** on the FUN layer to switch them to Windows/Linux shortcuts (Ctrl+C / Ctrl+V); press it again to switch back.
The mode is saved to EEPROM, so it survives unplugging. The left OLED shows the current mode.

### Globe Key (macOS)

Holding **V**, **M**, or **PASTE** (NAV layer) sends Caps Lock.
In macOS System Settings → Keyboard → Modifier Keys, remap Caps Lock to 🌐 Globe to use these keys for Globe shortcuts (e.g. input language switching).

### Combos

| Keys | Action | Active Layers |
|------|--------|---------------|
| K + L | Escape | BASE |
| S + D | Escape | BASE, MOUSE |

### Features

- **Home row mods** with bilateral hold-trigger ([Chordal Hold](https://docs.qmk.fm/tap_hold#chordal-hold): left mods trigger on right-hand keys and vice versa)
- **Fast-typing protection** ([Flow Tap](https://docs.qmk.fm/tap_hold#flow-tap)): a home row key pressed within 200 ms of another key is a tap (125 ms for Shift)
- **Hyper key** (Ctrl+Shift+Alt+GUI) on G and H
- **Mouse keys** with movement and scroll layers
- **FUN layer** accessed by holding ENTER while on NAV layer (SPACE → ENTER)
- **Key repeat** — tap a thumb key, then press and hold it within 200 ms to auto-repeat its tap key (e.g. Backspace)
- **OS toggle** for copy/paste shortcuts (macOS ↔ Windows/Linux)
- **Caps Word** on the right outer thumb key and on the NAV layer
- **OLED** — left half shows the active layer, OS mode and key-press counter

## Building Firmware

Requires QMK 0.29 or newer (Chordal Hold, Flow Tap).

1. Set up QMK:
   ```bash
   qmk setup
   ```
2. Clone this userspace and point QMK at it:
   ```bash
   git clone https://github.com/nsinotov/qmk_userspace.git ~/qmk_userspace
   qmk config user.overlay_dir="$(realpath ~/qmk_userspace)"
   ```
3. Build:
   ```bash
   qmk compile -kb crkbd/rev1 -km nikolay   # single target
   qmk userspace-compile                    # all targets in qmk.json
   ```

## Flashing

```bash
qmk flash -kb crkbd/rev1 -km nikolay
```

1. Connect one half via USB
2. Run the command above, then press the reset button on the half (or BOOT on the FUN layer) when prompted
3. Repeat for the other half

## Repository Structure

```
qmk_userspace/
├── qmk.json                    # userspace build targets
└── keyboards/crkbd/keymaps/nikolay/
    ├── keymap.c                # layers, home row mods, combos, OLED
    ├── config.h                # tap-hold, combo and OLED settings
    └── rules.mk                # enabled QMK features
```

## References

- [QMK Userspace](https://docs.qmk.fm/newbs_external_userspace) — building keymaps outside the QMK repository
- [QMK Tap-Hold Configuration](https://docs.qmk.fm/tap_hold) — Chordal Hold, Flow Tap, Permissive Hold
- [Corne keyboard](https://github.com/foostan/crkbd) — original Corne design by foostan
- [QMK Firmware](https://github.com/qmk/qmk_firmware)

## License

[MIT](LICENSE)
