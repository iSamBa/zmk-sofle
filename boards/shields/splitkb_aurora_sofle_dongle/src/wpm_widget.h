/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/sys/slist.h>

struct zmk_widget_wpm {
    sys_snode_t node;
    lv_obj_t *obj;
};

int zmk_widget_wpm_init(struct zmk_widget_wpm *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_wpm_obj(struct zmk_widget_wpm *widget);
