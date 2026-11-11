#!/usr/bin/env bash
set -euo pipefail

# Resolve paths relative to this script so it works from any directory.
userspace_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
firmware_dir="$(cd -- "${QMK_HOME:-$userspace_dir/../bastardkb-qmk-no-argos}" && pwd)"

# Use the isolated QMK environment and Arm compiler prepared for this keymap.
export PATH="$userspace_dir/../qmk-build-env/bin:$userspace_dir/../arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi/bin:$PATH"
export QMK_HOME="$firmware_dir"
export QMK_USERSPACE="$userspace_dir"

# Save these paths for subsequent QMK commands as well.
qmk config "user.qmk_home=$firmware_dir"
qmk config "user.overlay_dir=$userspace_dir"

cd -- "$userspace_dir"
"$userspace_dir/apply-module-patches.sh"
exec qmk compile -c \
    -kb bastardkb/charybdis/3x6/splinktegrated_rev1 \
    -km mykeymap -j 1 -e SKIP_GIT=yes "$@"
