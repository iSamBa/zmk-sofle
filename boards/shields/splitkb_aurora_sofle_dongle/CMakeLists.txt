# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_DISPLAY)
  target_sources(app PRIVATE src/wpm_widget.c)
  target_sources(app PRIVATE src/output_widget.c)
  target_sources(app PRIVATE src/custom_status_screen.c)
endif()
