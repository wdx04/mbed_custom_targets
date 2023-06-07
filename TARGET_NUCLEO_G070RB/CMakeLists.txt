# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-g070rb INTERFACE)

target_sources(mbed-nucleo-g070rb
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-g070rb
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-g070rb INTERFACE mbed-stm32g070xb)
