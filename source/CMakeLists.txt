idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES IO RotaryEncoder AHT10 lvgl gui lvgl_esp32_drivers)

