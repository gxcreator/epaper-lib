#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "font_8x8.h"
#include "draw.h"
#include "bus.h"
#include "ssd1680.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////// Please update the following configuration according to your HardWare spec /////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK 19
#define PIN_NUM_CS 9

#define PIN_NUM_DC 11
#define PIN_NUM_RST 1
#define PIN_NUM_BUSY 12

#define EPD_WIDTH 128
#define EPD_HEIGHT 296

// Static variables for SPI device and configuration
static const char *TAG = "MAIN";
static const FontDef Font8x8Def = {8, 8, Font8x8};

static uint8_t epd_buffer[EPD_WIDTH * EPD_HEIGHT / 8]; // Memory buffer to hold pixel data
static ssd1680_ctl_t disp;                             // Display control structure

// Main application entry point
void app_main(void)
{
    ESP_LOGI(TAG, "Initializing EPD...");
    // Initialize the EPD
    disp.bus.host = SPI2_HOST;
    disp.bus.sclk = PIN_NUM_CLK;
    disp.bus.mosi = PIN_NUM_MOSI;
    disp.bus.cs = PIN_NUM_CS;
    disp.bus.dc = PIN_NUM_DC;
    disp.bus.rst = PIN_NUM_RST;
    disp.bus.busy = PIN_NUM_BUSY;

    bus_init(&disp.bus);

    disp.height = EPD_HEIGHT;
    disp.width = EPD_WIDTH;
    disp.buffer = epd_buffer;

    ssd1680_init(&disp);

    vTaskDelay(pdMS_TO_TICKS(100));
    // ssd1680_clear();
    ssd1680_clear(&disp);
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_LOGI(TAG, "EPD initialized");
    while (1)
    {
        // vTaskDelay(pdMS_TO_TICKS(100));
        ESP_LOGI(TAG, "Displaying text...");

        ssd1680_draw_string(&disp, 0, 0, "What you guys are referring to as "
                                         "Linux, is in fact, GNU/Linux, or as I've recently taken to calling "
                                         "it, GNU plus Linux. Linux is not an operating system unto itself,"
                                         "but rather another free component of a fully functioning GNU "
                                         "system made useful by the GNU corelibs, shell utilities and vital "
                                         "system components comprising a full OS as defined by POSIX."
                                         "Many computer users run a modified version of the GNU system every "
                                         "day, without realizing it. Through a peculiar turn of events, the "
                                         "version of GNU which is widely used today is often called Linux, and "
                                         "many of its users are not aware that it is basically the GNU system, "
                                         "developed by the GNU Project.",
                            &Font8x8Def, SSD1680_BLACK, SSD1680_WHITE);
        // ssd1680_draw_string(0, 10, "ZXCVBNM", &Font8x8Def, SSD1680_BLACK, SSD1680_WHITE);
        ssd1680_upload(&disp);
        ssd1680_update(&disp);
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
