/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 100
#endif

// Perform initialisation
int pico_led_init(void)
{
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on)
{
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

int main()
{
    const uint led_red = 2;
    const uint led_blue = 3;
    // GND
    const uint led_green = 4;
    // Initialize LED pin
    gpio_init(led_red);
    gpio_set_dir(led_red, GPIO_OUT);
    gpio_init(led_blue);
    gpio_set_dir(led_blue, GPIO_OUT);
    gpio_init(led_green);
    gpio_set_dir(led_green, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    while (true)
    {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
        // Blink LED
        gpio_put(led_red, false);
        gpio_put(led_blue, true);
        gpio_put(led_green, false);
        sleep_ms(4000);
        gpio_put(led_red, true);
        sleep_ms(1000);
        gpio_put(led_red, false);
        gpio_put(led_green, true);
        sleep_ms(1000);
        gpio_put(led_green, false);
        gpio_put(led_blue, false);
        sleep_ms(1000);
        gpio_put(led_blue, true);
        sleep_ms(1000);
        gpio_put(led_red, true);
        gpio_put(led_blue, false);
        gpio_put(led_green, true);
        sleep_ms(2000);
    }
}
