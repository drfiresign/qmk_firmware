/* Copyright 2021 Joe Maples <joe@maples.dev>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// layers
#define BASE 0
#define TAB 1
#define SPACE 2
#define DISP 3
#define SLACK 4
#define BROWSE 5

// keycodes
#define ___ KC_TRNS
#define xxx KC_NO
#define PGDN KC_PAGE_DOWN
#define PGUP KC_PAGE_UP
#define PLAYPAUSE KC_MEDIA_PLAY_PAUSE
#define ENTER KC_ENTER
#define ESCAPE KC_ESCAPE

#define TAB_PREV RSFT(RALT(KC_LBRC))
#define TAB_NEXT RSFT(RALT(KC_RBRC))
#define TAB_NEW RALT(KC_T)
#define TAB_REOPEN RSFT(TAB_NEW)

#define SPACE_PREV RGUI(RALT(RSFT(KC_LBRC)))
#define SPACE_NEXT RGUI(RALT(RSFT(KC_RBRC)))

#define CYCLE_WIN LCTL(LSFT(KC_N))
#define CYCLE_DISP LCTL(LSFT(KC_O))

#define WIN_KILL RALT(KC_W)
#define _ALL RSFT(RALT(KC_A))

// layer helpers
#define L_TAB LT(1, xxx)
#define L_SPACE LT(2, xxx)
#define L_DISPLAY LT(3, PLAYPAUSE)
#define L_SLACK TG(SLACK)
#define L_WIN TG(BROWSE)
#define M_TAB MO(TAB)

// clang-format off
const uint16_t PROGMEM combo_pgdn[] =           {  L_TAB,     L_SPACE, COMBO_END};
const uint16_t PROGMEM combo_pgup[] =           {L_SPACE,   L_DISPLAY, COMBO_END};

// slack layer combos
const uint16_t PROGMEM combo_slack[] =          {  L_TAB,   L_DISPLAY, COMBO_END};
const uint16_t PROGMEM combo_slack_to_base[] =  { ESCAPE,        _ALL, COMBO_END};

// slack layer paging combos
const uint16_t PROGMEM combo_slack_pgdn[] =     { ESCAPE,       ENTER, COMBO_END};
const uint16_t PROGMEM combo_slack_pgup[] =     { ENTER,         _ALL, COMBO_END};

// browser layer combos
const uint16_t PROGMEM combo_browse[] =         {  L_TAB,     L_SPACE,  L_DISPLAY, COMBO_END};
const uint16_t PROGMEM combo_browse_to_base[] = {TAB_NEW,    WIN_KILL, TAB_REOPEN, COMBO_END};

// browser layer tab combos
const uint16_t PROGMEM combo_prev_tab[] =       {TAB_NEW,    WIN_KILL, COMBO_END};
const uint16_t PROGMEM combo_next_tab[] =       {WIN_KILL, TAB_REOPEN, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo_pgup, PGUP),
    COMBO(combo_pgdn, PGDN),
    COMBO(combo_slack, L_SLACK), /* COMBO_ACTION(x) == COMBO(x, KC_NO) */
    COMBO(combo_browse, L_WIN),
    COMBO(combo_slack_to_base, L_SLACK),
    COMBO(combo_slack_pgdn, PGDN),
    COMBO(combo_slack_pgup, PGUP),
    COMBO(combo_browse_to_base, L_WIN),
    COMBO(combo_prev_tab, TAB_PREV),
    COMBO(combo_next_tab, TAB_NEXT)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] =   LAYOUT(     L_TAB,    L_SPACE,  L_DISPLAY),
    [TAB] =    LAYOUT(       ___,   TAB_PREV,   TAB_NEXT),
    [SPACE] =  LAYOUT(SPACE_PREV,        ___, SPACE_NEXT),
    [DISP] =   LAYOUT( CYCLE_WIN, CYCLE_DISP,        ___),
    [SLACK] =  LAYOUT(    ESCAPE,      ENTER,       _ALL),
    [BROWSE] = LAYOUT(   TAB_NEW,   WIN_KILL, TAB_REOPEN)
};
// clang-format on

/* combo_slack and combo_browse should be layers instead of keys. These
   layers should be able to close windows and tabs, navigate slack messages,
   etc. However, we need to get the RGB lighting working properly so we can
   indicate which mode it is we're currently in.

   Basically, switching to one of these two toggleable layers should give you an
   entirely separate set of layer toggle options and combos.
*/

const rgblight_segment_t PROGMEM base_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_RED});
const rgblight_segment_t PROGMEM tab_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_GREEN});
const rgblight_segment_t PROGMEM space_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_GOLD});
const rgblight_segment_t PROGMEM disp_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_PURPLE});
const rgblight_segment_t PROGMEM slack_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_YELLOW});
const rgblight_segment_t PROGMEM win_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_TEAL});

// clang-format off
const rgblight_segment_t* const PROGMEM rgb_layers[] =
    RGBLIGHT_LAYERS_LIST(base_layer, tab_layer, space_layer, disp_layer, slack_layer, win_layer);
// clang-format on

void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, BASE));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, TAB));
    rgblight_set_layer_state(2, layer_state_cmp(state, SPACE));
    rgblight_set_layer_state(3, layer_state_cmp(state, DISP));
    rgblight_set_layer_state(4, layer_state_cmp(state, SLACK));
    rgblight_set_layer_state(5, layer_state_cmp(state, BROWSE));
    return state;
}
