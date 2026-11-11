import unittest

from scripts.draw_keymap import extract_combos, layer_names, split_args


class SourceExportTests(unittest.TestCase):
    def test_nested_keycode_arguments_remain_intact(self):
        self.assertEqual(split_args("lock, LCTL(LGUI(KC_Q))"),
                         ["lock", "LCTL(LGUI(KC_Q))"])

    def test_layer_enum_preserves_explicit_and_implicit_numbers(self):
        source = "enum charybdis_keymap_layers { LAYER_BASE = 0, LAYER_POINTER, };"
        self.assertEqual(layer_names(source), {"LAYER_BASE": 0, "LAYER_POINTER": 1})
        with self.assertRaises(ValueError):
            layer_names("enum charybdis_keymap_layers { LAYER_BASE = 0, LAYER_OTHER = 3 };")

    def test_combo_action_changes_do_not_retain_the_old_label(self):
        source = """
            const uint16_t PROGMEM pair[] = {KC_T, KC_Y, COMBO_END};
            combo_t key_combos[] = {COMBO(pair, LCTL(LGUI(KC_Q)))};
        """
        labels = {"LCTL(LGUI(KC_Q))": "Lock"}
        combos = extract_combos(source, ["KC_T", "KC_Y"], labels)
        self.assertEqual(combos[0], {"p": [0, 1], "k": "Lock", "l": ["Base"]})
        changed = extract_combos(source.replace("LCTL(LGUI(KC_Q))", "KC_ESC"),
                                 ["KC_T", "KC_Y"], labels)
        self.assertEqual(changed[0]["k"], "KC_ESC")

    def test_ambiguous_or_missing_combo_keys_fail(self):
        source = """
            const uint16_t PROGMEM pair[] = {KC_T, KC_Y, COMBO_END};
            combo_t key_combos[] = {COMBO(pair, KC_ESC)};
        """
        for base in (["KC_T"], ["KC_T", "KC_T", "KC_Y"]):
            with self.subTest(base=base), self.assertRaises(ValueError):
                extract_combos(source, base, {})


if __name__ == "__main__":
    unittest.main()
