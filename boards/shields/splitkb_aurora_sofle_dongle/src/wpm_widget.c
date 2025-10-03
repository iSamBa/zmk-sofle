/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/wpm.h>

#include "wpm_widget.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct wpm_state {
    uint8_t wpm;
};

static struct wpm_state get_state(const zmk_event_t *eh) {
    return (struct wpm_state){.wpm = zmk_wpm_get_state()};
}

static void set_wpm_text(lv_obj_t *label, struct wpm_state state) {
    char text[16];
    snprintf(text, sizeof(text), "WPM %d", state.wpm);
    lv_label_set_text(label, text);
}

static void wpm_update_cb(struct wpm_state state) {
    struct zmk_widget_wpm *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_wpm_text(widget->obj, state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_wpm, struct wpm_state, wpm_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_wpm, zmk_wpm_state_changed);

int zmk_widget_wpm_init(struct zmk_widget_wpm *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);

    // Set initial text
    struct wpm_state state = get_state(NULL);
    set_wpm_text(widget->obj, state);

    // Add to widgets list
    sys_slist_append(&widgets, &widget->node);

    return 0;
}

lv_obj_t *zmk_widget_wpm_obj(struct zmk_widget_wpm *widget) {
    return widget->obj;
}
