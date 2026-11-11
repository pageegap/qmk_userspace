#!/usr/bin/env bash
set -euo pipefail
userspace_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
export PATH="$userspace_dir/../qmk-build-env/bin:$PATH"
export QMK_HOME="${QMK_HOME:-$userspace_dir/../bastardkb-qmk-no-argos}"
export QMK_USERSPACE="$userspace_dir"
cd -- "$userspace_dir"
exec python3 scripts/draw_keymap.py "$@"
