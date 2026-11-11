#!/usr/bin/env bash
set -euo pipefail
userspace_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
module_dir="$userspace_dir/modules/bastardkb"
patch_file="$userspace_dir/patches/pointing-no-argos.patch"
expected_revision=309202383aca26a62b0f5670c51833a59206d146

if [[ ! -e "$module_dir/.git" ]]; then
    git -C "$userspace_dir" submodule update --init -- modules/bastardkb
fi
if [[ "$(git -C "$module_dir" rev-parse HEAD)" != "$expected_revision" ]]; then
    echo "Pointing module revision changed; review the patch before building." >&2
    exit 1
fi
if git -C "$module_dir" apply --check "$patch_file" 2>/dev/null; then
    git -C "$module_dir" apply "$patch_file"
    echo "Applied non-Argos pointing fixes."
elif git -C "$module_dir" apply --reverse --check "$patch_file" 2>/dev/null; then
    echo "Non-Argos pointing fixes already applied."
else
    echo "Pointing patch does not apply cleanly; existing changes were left untouched." >&2
    exit 1
fi
