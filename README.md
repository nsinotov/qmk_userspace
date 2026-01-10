# QMK Userspace - Nikolay's Keymaps

This repository contains my custom QMK keyboard layouts using the QMK Userspace feature.

## Keyboards

- **Corne (CRKBD)** - Two layouts:
  - `nikolay` - Standard layout
  - `nikolay_rp2040` - Layout for RP2040-based Corne

## Building

1. Clone the QMK firmware repository:
   ```bash
   qmk setup
   ```

2. Clone this userspace repository:
   ```bash
   git clone https://github.com/YOUR_USERNAME/qmk_userspace.git ~/qmk_userspace
   ```

3. Configure QMK to use this userspace:
   ```bash
   qmk config user.overlay_dir=~/qmk_userspace
   ```

4. Initialize QMK submodules (run from QMK firmware directory):
   ```bash
   cd ~/qmk_firmware
   # or wherever your QMK firmware is located
   qmk git-submodule
   ```

5. Build your keyboard (run from QMK firmware directory):
   ```bash
   # Build specific keymap
   qmk compile -kb crkbd/rev1 -km nikolay
   qmk compile -kb crkbd/rev1 -km nikolay_rp2040

   # Alternative with /common variant
   qmk compile -kb crkbd/rev1/common -km nikolay

   # Build all targets defined in qmk.json
   qmk userspace-compile
   ```

## Flashing

Make sure you're in the QMK firmware directory, then:

```bash
# Flash nikolay keymap
qmk flash -kb crkbd/rev1 -km nikolay
# or with /common variant
qmk flash -kb crkbd/rev1/common -km nikolay

# Flash nikolay_rp2040 keymap
qmk flash -kb crkbd/rev1 -km nikolay_rp2040
# or with /common variant
qmk flash -kb crkbd/rev1/common -km nikolay_rp2040
```

## Repository Structure

```
qmk_userspace/
├── qmk.json                          # Userspace configuration
├── keyboards/
│   └── crkbd/
│       └── keymaps/
│           ├── nikolay/              # Main keymap
│           │   ├── keymap.c
│           │   ├── config.h
│           │   └── rules.mk
│           └── nikolay_rp2040/       # RP2040 variant
│               ├── keymap.c
│               ├── config.h
│               └── rules.mk
```

## More Information

- [QMK Userspace Documentation](https://docs.qmk.fm/#/newbs_external_userspace)
- [QMK Firmware](https://github.com/qmk/qmk_firmware)
