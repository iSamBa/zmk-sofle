/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/usb.h>
#include <zmk/ble.h>
#include <zmk/endpoints.h>

#include "output_widget.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct output_state {
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
};

static struct output_state get_state(const zmk_event_t *eh) {
    return (struct output_state){
        .selected_endpoint = zmk_endpoints_selected(),
        .active_profile_index = zmk_ble_active_profile_index(),
        .active_profile_connected = zmk_ble_active_profile_is_connected(),
        .active_profile_bonded = !zmk_ble_active_profile_is_open(),
    };
}

static void set_output_text(lv_obj_t *label, struct output_state state) {
    char text[32] = {};

    switch (state.selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        snprintf(text, sizeof(text), "USB");
        break;
    case ZMK_TRANSPORT_BLE:
        if (state.active_profile_connected) {
            snprintf(text, sizeof(text), "BT %d", state.active_profile_index + 1);
        } else if (state.active_profile_bonded) {
            snprintf(text, sizeof(text), "BT %d~", state.active_profile_index + 1);
        } else {
            snprintf(text, sizeof(text), "BT -");
        }
        break;
    }

    lv_label_set_text(label, text);
}

static void output_update_cb(struct output_state state) {
    struct zmk_widget_output *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_output_text(widget->obj, state);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output, struct output_state, output_update_cb, get_state)

ZMK_SUBSCRIPTION(widget_output, zmk_endpoint_changed);
#if IS_ENABLED(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output, zmk_ble_active_profile_changed);
#endif

int zmk_widget_output_init(struct zmk_widget_output *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);

    // Set initial text
    struct output_state state = get_state(NULL);
    set_output_text(widget->obj, state);

    // Add to widgets list
    sys_slist_append(&widgets, &widget->node);

    return 0;
}

lv_obj_t *zmk_widget_output_obj(struct zmk_widget_output *widget) {
    return widget->obj;
}
