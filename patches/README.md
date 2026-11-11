# Pointing module compatibility patch

`pointing-no-argos.patch` is applied to BastardKB/qmk_modules commit `309202383aca26a62b0f5670c51833a59206d146` by `../apply-module-patches.sh` before local and CI firmware builds. It replaces the former full module copy. The upstream submodule revision remains unchanged.

The patch modifies only three C files. The userspace root README explains each change. The keymap owns a 256-byte QMK user EEPROM block for the pointing settings; other consumers must coordinate that allocation.

For an upstream update, inspect the upstream changes and retire resolved hunks before changing the expected revision in the helper. The helper checks applicability and supports repeated runs without overwriting conflicting edits.
