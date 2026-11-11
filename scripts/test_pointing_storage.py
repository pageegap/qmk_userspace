#!/usr/bin/env python3
"""Run the EEPROM regression test against the patched upstream function bodies."""
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
MODULE = ROOT / "modules/bastardkb/bk_pointing_device"


def function_body(source, signature):
    start = source.index(signature)
    brace = source.index("{", start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


def main():
    subprocess.run(["bash", str(ROOT / "apply-module-patches.sh")], check=True)
    source = (MODULE / "bk_pointing_device.c").read_text()
    signatures = ["static void read_bkpd_config_from_eeprom(void)",
                  "void write_bkpd_config_to_eeprom(void)"]
    with tempfile.TemporaryDirectory(prefix="charybdis-storage-") as temporary:
        temp = Path(temporary)
        (temp / "storage_under_test.inc").write_text(
            "extern bkpd_config_t g_bkpd_config;\n" +
            "\n".join(function_body(source, name) for name in signatures))
        binary = temp / "storage_test"
        subprocess.run([
            os.environ.get("CC", "cc"), "-std=c11", "-Wall", "-Wextra", "-Werror",
            '-DQMK_KEYBOARD_H="fake_qmk.h"', "-I", str(ROOT / "tests/pointing_storage"),
            "-I", str(MODULE), "-I", str(temp),
            str(ROOT / "tests/pointing_storage/test.c"), "-o", str(binary),
        ], check=True)
        subprocess.run([str(binary)], check=True)


if __name__ == "__main__":
    main()
