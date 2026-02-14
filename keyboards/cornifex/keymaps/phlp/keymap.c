/* Copyright 2020 foostan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */
#include QMK_KEYBOARD_H

// ===============================
// Layers
// ===============================
enum layers {
    L_DFLT_WIN = 0,
    L_DFLT_MAC,
    L_SYM,
    L_SET,
};

// ===============================
// Custom Keycodes
// ===============================
enum custom_keycodes {
    OS_TOG = SAFE_RANGE,
};

// ===============================
// Aliases
// ===============================
#define ALT_TAB    LALT_T(KC_TAB)
#define LT_SYM_SPC LT(L_SYM, KC_SPC)
#define LT_SYM_ENT LT(L_SYM, KC_ENT)

#define CTL_DEL    LCTL_T(KC_DEL)
#define GUI_DEL    LGUI_T(KC_DEL)
#define SFT_BSPC   LSFT_T(KC_BSPC)

#define ___ KC_TRNS
#define XXX KC_NO

// ===============================
// Umlaut Combos (US-International)
// ===============================
enum combo_events {
    UML_AE,
    UML_OE,
    UML_UE,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM uml_ae_combo[] = {KC_A, KC_E, COMBO_END};
const uint16_t PROGMEM uml_oe_combo[] = {KC_O, KC_E, COMBO_END};
const uint16_t PROGMEM uml_ue_combo[] = {KC_U, KC_E, COMBO_END};

combo_t key_combos[] = {
    [UML_AE] = COMBO(uml_ae_combo, KC_NO),
    [UML_OE] = COMBO(uml_oe_combo, KC_NO),
    [UML_UE] = COMBO(uml_ue_combo, KC_NO),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) return;

    switch (combo_index) {
        case UML_AE:
            tap_code16(RALT(KC_A));
            break;
        case UML_OE:
            tap_code16(RALT(KC_O));
            break;
        case UML_UE:
            tap_code16(RALT(KC_U));
            break;
    }
}

// ===============================
// OS Toggle with Feedback
// ===============================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {

        case OS_TOG: {
            uint8_t cur = get_highest_layer(default_layer_state);
            uint8_t next = (cur == L_DFLT_MAC) ? L_DFLT_WIN : L_DFLT_MAC;

            set_single_persistent_default_layer(next);

            if (next == L_DFLT_MAC) {
                SEND_STRING("[OSX]");
            } else {
                SEND_STRING("[WIN]");
            }

            return false;
        }
    }

    return true;
}

// ===============================
// Keymaps
// ===============================
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // ===============================
    // Windows Base Layer
    // ===============================
    [L_DFLT_WIN] = LAYOUT(
        ALT_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,     KC_MINS,
        KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,     KC_K,    KC_L,    KC_SCLN,  KC_GRV,
        KC_ESC,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M,     KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,

        XXX,      XXX,     XXX,     MO(L_SET), LT_SYM_SPC, CTL_DEL,
                  SFT_BSPC, LT_SYM_ENT, MO(L_SET), XXX,    XXX,     OS_TOG
    ),

    // ===============================
    // Mac Base Layer
    // ===============================
    [L_DFLT_MAC] = LAYOUT(
        ALT_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,     KC_MINS,
        KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,     KC_K,    KC_L,    KC_SCLN,  KC_GRV,
        KC_ESC,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M,     KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,

        XXX,      XXX,     XXX,     MO(L_SET), LT_SYM_SPC, GUI_DEL,
                  SFT_BSPC, LT_SYM_ENT, MO(L_SET), XXX,    XXX,     OS_TOG
    ),

    // ===============================
    // Symbol Layer
    // ===============================
    [L_SYM] = LAYOUT(
        ___,      KC_QUOT, KC_LT,   KC_GT,   KC_DQUO, KC_COMM,    KC_AMPR, KC_SCLN,  KC_LBRC, KC_RBRC, KC_PERC, XXX,
        ___,      KC_EXLM, KC_MINS, KC_PLUS, KC_EQL,  KC_DOT,     KC_PIPE, KC_COLN,  KC_LPRN, KC_RPRN, KC_QUES, XXX,
        ___,      KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, KC_HASH,    KC_TILD, KC_DLR,   KC_LCBR, KC_RCBR, KC_AT,   XXX,

        ___,      ___,     ___,     ___,     ___,     ___,
                  ___,     ___,     ___,     ___,     ___,     ___
    ),

    // ===============================
    // Settings Layer
    // ===============================
    [L_SET] = LAYOUT(
        XXX,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,       KC_F1,   KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,
        XXX,      KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_LEFT, KC_DOWN,  KC_UP,   KC_RGHT, XXX,     XXX,
        XXX,      XXX,     XXX,     XXX,     XXX,     XXX,        KC_F7,   KC_F8,    KC_F9,   KC_F10,  KC_F11,  KC_F12,

        ___,      ___,     ___,     ___,     ___,     ___,
                  ___,     ___,     ___,     ___,     ___,     QK_BOOT
    ),
};
