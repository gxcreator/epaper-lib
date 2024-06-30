#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "bus.h"

static const char *TAG = "BUS";

/**
 * @brief Initialize the GPIOs
 *
 * @param bus
 */
static void bus_gpio_init(sd1860_bus_t *bus)
{
    gpio_set_direction(bus->dc, GPIO_MODE_OUTPUT);
    gpio_set_direction(bus->rst, GPIO_MODE_OUTPUT);
    gpio_set_direction(bus->cs, GPIO_MODE_OUTPUT);
    gpio_set_direction(bus->busy, GPIO_MODE_INPUT);
}

/**
 * @brief Select the bus
 *
 * @param bus
 */
static void bus_select(sd1860_bus_t *bus)
{
    gpio_set_level(bus->cs, 0);
}

/**
 * @brief Deselect the bus
 *
 * @param bus
 */
static void bus_deselect(sd1860_bus_t *bus)
{
    gpio_set_level(bus->cs, 1);
}

/**
 * @brief Start a command
 *
 * @param bus
 */
static void bus_start_cmd(sd1860_bus_t *bus)
{
    gpio_set_level(bus->dc, 0);
    bus_select(bus);
}

/**
 * @brief Start data
 *
 * @param bus
 */
static void bus_start_data(sd1860_bus_t *bus)
{
    gpio_set_level(bus->dc, 1);
    bus_select(bus);
}

/**
 * @brief End the transaction
 *
 * @param bus
 */
static void bus_tx_done(sd1860_bus_t *bus)
{
    bus_deselect(bus);
}

void bus_reset(sd1860_bus_t *bus)
{
    gpio_set_level(bus->rst, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(bus->rst, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void bus_waitbusy(sd1860_bus_t *bus)
{
    while (gpio_get_level(bus->busy) == 1)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    vTaskDelay(pdMS_TO_TICKS(10));
}

/**
 * @brief Transmit a byte
 *
 * @param bus
 * @param byte
 * @return int
 */
static int bus_tx_byte(sd1860_bus_t *bus, uint8_t byte)
{
    spi_transaction_t t =
        {
            .length = 8,
            .tx_buffer = &byte,
        };
    return spi_device_transmit(bus->spi, &t);
}

static int bus_tx_chunk(sd1860_bus_t *bus, uint8_t *data, uint16_t len)
{
    ESP_LOGI(TAG, "bus_tx_chunk: %p %d", data, len);
    spi_transaction_t t =
        {
            .length = len * 8,
            .tx_buffer = data,
        };
    return spi_device_transmit(bus->spi, &t);
}

static int bus_tx_stream(sd1860_bus_t *bus, uint8_t *data, uint16_t len)
{
    int res = ESP_ERR_NOT_FOUND;
    while (len > 0)
    {
        uint16_t chunk = len > SPI_CHUNK_SIZE ? SPI_CHUNK_SIZE : len;
        res = bus_tx_chunk(bus, data, chunk);
        if (res != ESP_OK)
        {
            break;
        }
        len -= chunk;
        data += chunk;
    }
    return res;
}

int bus_init(sd1860_bus_t *bus)
{
    bus_gpio_init(bus);

    spi_bus_config_t buscfg =
        {
            .miso_io_num = -1,
            .mosi_io_num = bus->mosi,
            .sclk_io_num = bus->sclk,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
        };
    int res = spi_bus_initialize(bus->host, &buscfg, SPI_DMA_CH_AUTO);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %d", res);
        return res;
    }

    ESP_LOGD(TAG, "SPI init done");

    spi_device_interface_config_t devcfg =
        {
            .clock_speed_hz = 8 * 1000 * 1000, // 4 MHz
            .mode = 0,
            .spics_io_num = -1,
            .queue_size = 7,
        };

    res = spi_bus_add_device(SPI2_HOST, &devcfg, &bus->spi);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %d", res);
        return res;
    }

    ESP_LOGD(TAG, "SPI dev added");

    return res;
}

// Function to send a command to the SSD1680
int bus_cmd(sd1860_bus_t *bus, uint8_t command)
{
    bus_start_cmd(bus);
    return bus_tx_byte(bus, command);
}

// Function to send data to the SSD1680
int bus_data_byte(sd1860_bus_t *bus, uint8_t data)
{
    bus_start_data(bus);
    return bus_tx_byte(bus, data);
}

int bus_data_stream(sd1860_bus_t *bus, uint8_t *data, uint16_t len)
{
    bus_start_data(bus);
    return bus_tx_stream(bus, data, len);
}

void bus_finish(sd1860_bus_t *bus)
{
    bus_tx_done(bus);
}
