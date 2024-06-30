#ifndef _BUS_H_
#define _BUS_H_

#include <stdint.h>
#include "driver/spi_master.h"

#define SPI_CHUNK_SIZE 2400

// TODO: get rid of system-specific defines
typedef struct
{
    spi_host_device_t host;  ///< SPI Host
    spi_device_handle_t spi; ///< SPI handle
    int mosi;                ///< Master Out Slave In
    int sclk;                ///< Serial Clock
    int cs;                  ///< Chip Select
    int dc;                  ///< Data/Command
    int rst;                 ///< Reset
    int busy;                ///< Busy In

} sd1860_bus_t;

/**
 * @brief Initialize the bus
 *
 * @param bus
 * @return int
 */
int bus_init(sd1860_bus_t *bus);

/**
 * @brief Reset the bus
 *
 * @param bus
 * @return int
 */
void bus_reset(sd1860_bus_t *bus);

/**
 * @brief Send a command
 *
 * @param bus
 * @param command
 * @return int
 */
int bus_cmd(sd1860_bus_t *bus, uint8_t command);

/**
 * @brief Send data
 *
 * @param data
 * @return int
 */
int bus_data_byte(sd1860_bus_t *bus, uint8_t data);

/**
 * @brief Send data stream
 *
 * @param bus
 * @param data
 * @param len
 * @return int
 */
int bus_data_stream(sd1860_bus_t *bus, uint8_t *data, uint16_t len);

/**
 * @brief Finish the bus
 *
 * @param bus
 * @return int
 */
void bus_finish(sd1860_bus_t *bus);

/**
 * @brief Wait for the bus to be busy
 *
 * @param bus
 */
void bus_waitbusy(sd1860_bus_t *bus);

#endif // _BUS_H_