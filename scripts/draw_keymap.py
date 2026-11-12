#!/usr/bin/env python3
"""Export this C keymap through QMK and render it with keymap-drawer."""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile

import yaml
from keymap_drawer.config import ParseConfig

ROOT = Path(__file__).resolve().parents[1]
KEYBOARD = "bastardkb/charybdis/3x6/splinktegrated_rev1"
KEYMAP = ROOT / "keyboards/bastardkb/charybdis/3x6/keymaps/mykeymap/keymap.c"
OUTPUT = ROOT / "keymap-drawer"


def split_args(text):
    """Split C keycode arguments while keeping nested modifier calls intact."""
    parts, start, depth = [], 0, 0
    for i, char in enumerate(text):
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        elif char == "," and depth == 0:
            parts.append(text[start:i].strip())
            start = i + 1
    if depth:
        raise ValueError("Unbalanced keycode expression")
    parts.append(text[start:].strip())
    return [part for part in parts if part]


def layer_names(source):
    match = re.search(r"enum charybdis_keymap_layers\s*\{([^}]+)\}", source)
    if not match:
        raise ValueError("Cannot find the keymap layer enum")
    result, index = {}, 0
    for entry in split_args(match[1]):
        name, *value = entry.split("=")
        if value:
            index = int(value[0].strip(), 0)
        result[name.strip()] = index
        index += 1
    if sorted(result.values()) != list(range(len(result))):
        raise ValueError("Expected contiguous layers starting at zero")
    return result


def extract_combos(source, base_keys, labels, layers=None):
    arrays = dict(re.findall(
        r"const uint16_t PROGMEM (\w+)\[\]\s*=\s*\{([^}]+)\}", source
    ))
    match = re.search(r"combo_t key_combos\[\]\s*=\s*\{(.*?)\};", source, re.S)
    if not match:
        raise ValueError("Cannot find key_combos; update the diagram exporter if its declaration changed")
    combos = []
    for entry in split_args(match[1]):
        initializer = re.fullmatch(
            r"(?:\[[^\[\]]+\]\s*=\s*)?COMBO\s*\((.*)\)", entry, re.S
        )
        if not initializer:
            raise ValueError(f"Unsupported combo declaration: {entry}")
        name, action = split_args(initializer[1])
        keys = split_args(arrays[name])
        if keys.pop() != "COMBO_END":
            raise ValueError(f"Missing COMBO_END for {name}")
        keys = [re.sub(r"\s+", "", key) for key in keys]
        candidates = {"Base": base_keys, **(layers or {})}
        matches = {}
        for layer, bindings in candidates.items():
            if all(bindings.count(key) == 1 for key in keys):
                positions = tuple(bindings.index(key) for key in keys)
                matches.setdefault(positions, []).append(layer)
                # Show Base combos once; other layers may inherit them through transparency.
                if layer == "Base":
                    break
        if not matches:
            raise ValueError(
                f"Combo {name} needs unique trigger keys together in a layer: {', '.join(keys)}"
            )
        action = re.sub(r"\s+", "", action)
        for positions, combo_layers in matches.items():
            combos.append({"p": list(positions), "k": labels.get(action, action), "l": combo_layers})
    return combos


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--qmk-home", type=Path,
                        default=os.environ.get("QMK_HOME", ROOT.parent / "bastardkb-qmk-no-argos"))
    args = parser.parse_args()
    firmware = args.qmk_home.expanduser().resolve()
    layout_file = firmware / "keyboards/bastardkb/charybdis/3x6/info.json"
    if not layout_file.is_file():
        parser.error(f"Cannot find Charybdis layout in {firmware}; set QMK_HOME or --qmk-home")
    env = dict(os.environ, QMK_HOME=str(firmware), QMK_USERSPACE=str(ROOT))
    exported = json.loads(subprocess.check_output([
        "qmk", "c2json", "--no-cpp", "-kb", KEYBOARD, "-km", "mykeymap", str(KEYMAP)
    ], cwd=ROOT, env=env, text=True))
    source = re.sub(r"/\*.*?\*/|//[^\n]*", "", KEYMAP.read_text(), flags=re.S)
    names = layer_names(source)
    if len(exported["layers"]) != len(names):
        raise ValueError("QMK export and source layer counts differ")
    titles = [name.removeprefix("LAYER_").title() for name in names]
    if titles[0] != "Base":
        raise ValueError("Combo visualization expects layer zero to be Base")
    def normalize(key):
        key = re.sub(r"\s+", "", key)
        for name, number in names.items():
            key = re.sub(r"\b" + re.escape(name) + r"\b", str(number), key)
        return key
    exported["layers"] = [[normalize(key) for key in layer] for layer in exported["layers"]]
    physical = json.loads(layout_file.read_text())["layouts"]
    count = len(physical[exported["layout"]]["layout"])
    if any(len(layer) != count for layer in exported["layers"]):
        raise ValueError("Layer key counts do not match the physical layout")
    annotations = yaml.safe_load((OUTPUT / "annotations.yaml").read_text())
    combos = extract_combos(
        source, exported["layers"][0], annotations["combo_labels"],
        dict(zip(titles, exported["layers"])),
    )
    with tempfile.TemporaryDirectory(prefix="charybdis-keymap-") as temporary:
        qmk_json = Path(temporary) / "keymap.json"
        qmk_json.write_text(json.dumps(exported))
        # Custom key legends extend keymap-drawer's defaults rather than replacing them.
        config = yaml.safe_load((OUTPUT / "config.yaml").read_text())
        custom = config["parse_config"].get("qmk_keycode_map", {})
        config["parse_config"]["qmk_keycode_map"] = ParseConfig().qmk_keycode_map | custom
        config_file = Path(temporary) / "config.yaml"
        config_file.write_text(yaml.safe_dump(config, allow_unicode=True))
        parsed = subprocess.check_output([
            "keymap", "-c", str(config_file), "parse", "-q", str(qmk_json),
            "--layer-names", *titles,
        ], cwd=ROOT, env=env, text=True)
    drawing = yaml.safe_load(parsed)
    drawing["layout"] = {"qmk_info_json": "keymap-drawer/layout.json", "layout_name": exported["layout"]}
    drawing["combos"] = combos
    (OUTPUT / "layout.json").write_text(json.dumps({"layouts": physical}, indent=2) + "\n")
    diagram = OUTPUT / "mykeymap.yaml"
    diagram.write_text(yaml.safe_dump(drawing, sort_keys=False, allow_unicode=True))
    subprocess.run([
        "keymap", "-c", str(OUTPUT / "config.yaml"), "draw", str(diagram),
        "-o", str(OUTPUT / "mykeymap.svg"),
    ], cwd=ROOT, env=env, check=True)
    print(f"Rendered {len(titles)} layers, {count} keys per layer, and {len(combos)} combos")
    print(OUTPUT / "mykeymap.svg")


if __name__ == "__main__":
    main()
