/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/peripheral_status.h>

#include "wpm_widget.h"
#include "output_widget.h"

static struct zmk_widget_layer_status layer_status_widget;
static struct zmk_widget_peripheral_status peripheral_status_widget;
static struct zmk_widget_wpm wpm_widget;
static struct zmk_widget_output output_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;

    screen = lv_obj_create(NULL);

    // Layer status - center of screen
    zmk_widget_layer_status_init(&layer_status_widget, screen);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_CENTER, 0, 0);

    // Peripheral status (split halves' batteries) - bottom
    zmk_widget_peripheral_status_init(&peripheral_status_widget, screen);
    lv_obj_align(zmk_widget_peripheral_status_obj(&peripheral_status_widget), LV_ALIGN_BOTTOM_MID, 0, 0);

    // Output/BT profile widget - top right
    zmk_widget_output_init(&output_widget, screen);
    lv_obj_align(zmk_widget_output_obj(&output_widget), LV_ALIGN_TOP_RIGHT, 0, 0);

    // WPM widget - below Output/BT widget, aligned to right
    zmk_widget_wpm_init(&wpm_widget, screen);
    lv_obj_align_to(zmk_widget_wpm_obj(&wpm_widget), zmk_widget_output_obj(&output_widget), LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 2);

    return screen;
}
