# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_DISPLAY)
  # Remove prospector's custom_status_screen.c from the build
  get_target_property(app_sources app SOURCES)
  list(FILTER app_sources EXCLUDE REGEX "prospector.*custom_status_screen\\.c$")
  set_target_properties(app PROPERTIES SOURCES "${app_sources}")

  # Add our custom widgets and status screen
  target_sources(app PRIVATE src/wpm_widget.c)
  target_sources(app PRIVATE src/output_widget.c)
  target_sources(app PRIVATE src/custom_status_screen.c)
endif()
