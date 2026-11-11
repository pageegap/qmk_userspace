# Charybdis Mini: migrated keymap without Argos

This userspace builds `mykeymap` for `bastardkb/charybdis/3x6/splinktegrated_rev1` (RP2040).
The original source remains in `../qmk_userspaceBU`, including its old UF2.
Firmware source is pinned to `bc50f889c80773e96bfcb1249f137b676cf0eb5f` in the build workflow.

## Rebuild locally

The sibling `qmk-build-env`, `bastardkb-qmk-no-argos`, and Arm GNU 14.2.Rel1 directories contain the isolated build environment prepared for this migration. The Bash build script saves `user.qmk_home` and `user.overlay_dir` in your QMK configuration, changes into this userspace, and runs a clean compile (`qmk compile -c`).

```sh
cd ~/Downloads/qmk_userspace-no-argos
./build.sh
```

The UF2 is copied to this userspace directory. To use another QMK checkout, set `QMK_HOME` when running the script. For another machine, install QMK's build dependencies, clone the pinned firmware and its submodules, and configure this directory as QMK userspace. GitHub Actions is configured for this one target; pushing this userspace triggers the firmware build workflow.

## Keymap diagram

![Charybdis Mini keymap](keymap-drawer/mykeymap.svg)

Regenerate the diagram after editing `keymap.c`:

```sh
./draw-keymap.sh
```

The prepared QMK environment includes keymap-drawer. On another machine, install `keymap-drawer/requirements.txt` into your QMK Python environment and set `QMK_HOME` to the firmware checkout. Drawing does not require the Arm compiler or a firmware build.

The diagram is generated from the C keymap and its five combos. Named layers and custom legends are preserved, including home-row modifiers, trackball modes, DPI controls, and the Mission Control/Maximize tap dance. Combo markers are shown on Base; their actual scope follows QMK's combo handling. Encoder actions remain listed in `keymap.c` and are not shown in the key diagram.

`keymap-drawer/config.yaml` controls legends and appearance; `annotations.yaml` labels combo actions. Generated YAML and the local physical layout JSON are saved alongside the SVG. The drawing workflow generates downloadable artifacts and checks that the committed preview matches the current keymap. Run the drawing command and commit its outputs along with keymap edits.

## Preserved behavior

- Five layers, all home-row modifiers and layer taps, five combos, both tap-dance definitions, and the NEWLINE macro.
- Left encoder on GP20/GP23, direction flip and resolution 2, with the original per-layer actions.
- Caps Lock red; lower green, raise cyan, pointer purple; brightness limit 130 and split RGB synchronization.
- Manual pointer-layer activation. Automatic pointer-layer activation remains off.
- Sniping activates on the configuration layer (layer 4), matching the old source.
- Normal DPI starts at 1200 and cycles through 16 values in steps of 200 (1200–4200). Precision DPI starts at 900 and cycles through four values in steps of 100 (900–1200).

`LAYER_CONGIF` was renamed to `LAYER_CONFIG` without changing its layer number. The old undefined `DPI_UP`/`DPI_DOWN` positions on the right of the configuration layer now use `S_D_MOD`/`S_D_RMOD` to adjust precision DPI. `DPI_MOD`/`DPI_RMOD` still adjust normal DPI, even while sniping is active; Shift reverses their direction.

## Pointing module and Argos exclusion

The keymap selects only `local/bk_pointing_device`. Argos, VIA, and raw HID are not enabled in this build. The upstream submodule still contains unused vendor modules and example keymaps; these are not compiled into this target.

The local pointing module is copied from BastardKB qmk_modules commit `309202383aca26a62b0f5670c51833a59206d146`. Its GPL license and copyright headers are retained. Local changes:

- Remove an unconditional Argos RGB header include and guard Argos-only device-information code.
- Store the complete pointing configuration in QMK's versioned 256-byte user EEPROM block, with a compile-time size check. Upstream's non-Argos path stores only its first byte.
- Avoid resetting user EEPROM at each startup and avoid duplicate calls to QMK user hooks.
- Allow the keymap to set legacy DPI minima, increments, and ranges and initialize them at the minimum step.
- Retain legacy normal-DPI key behavior instead of adjusting whichever pointing mode is active.

Keep the local module when moving this userspace. Updating the upstream submodule does not update this local copy; review its fixes before replacing it.

## Flash and hardware verification

Flash the generated `bastardkb_charybdis_3x6_splinktegrated_rev1_mykeymap.uf2` onto **both halves**, entering the RP2040 bootloader for each. The board is configured as right-master; use USB on the right for normal operation. No keyboard has been flashed automatically.

After flashing, check typing and home-row holds, combos/tap dances, the encoder on each layer, trackball movement and drag-scroll, configuration-layer sniping, RGB indicators, and DPI retention after unplugging/reconnecting. Compilation and build inspection cannot verify these physical behaviors. Old firmware EEPROM settings are not imported into the new pointing layout; DPI starts with the defaults above.

## Validation

The RP2040 UF2 compiles and links successfully. Source comparisons preserve all five layouts, encoder mappings, combos, and tap dances except the documented names. The final ELF and compiler dependencies contain no Argos code or headers, and no VIA or raw-HID symbols. See `validation.txt` for the firmware hash and validation limits.

The host regression test uses the actual EEPROM storage source with a mock EEPROM API; it verifies invalid-storage handling and complete configuration save/reload. Run it from this userspace root:

```sh
clang -std=c11 -Wall -Wextra -Werror '-DQMK_KEYBOARD_H="fake_qmk.h"' \
  -I tests/pointing_storage -I modules/local/bk_pointing_device \
  tests/pointing_storage/test.c modules/local/bk_pointing_device/bk_pointing_storage.c \
  -o /tmp/charybdis-pointing-storage-test
/tmp/charybdis-pointing-storage-test
```
