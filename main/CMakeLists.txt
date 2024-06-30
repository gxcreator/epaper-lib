set(srcs "draw.c"
    "bus.c"
    "ssd1680"
    "test_main.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
