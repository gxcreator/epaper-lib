#include "esp_system.h"
#include "driver/spi_master.h"

#include "bus.h"
#include "ssd1680.h"

#include "esp_log.h"

static const char *TAG = "SSD1680";

static void ssd1680_doc_set(ssd1680_ctl_t *disp, uint8_t doc)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, SSD1680_DRIVER_OUTPUT_CTL);
    // bus_data(bus, disp->height & 0xff);  // 296 [0x127]
    // bus_data(bus, (disp->height >> 8) & 0x01);  // 296 [0x127]
    bus_data_byte(bus, 0x27); // 0x127-->(295+1)=296
    bus_data_byte(bus, 0x01);
    bus_data_byte(bus, doc); //
    bus_finish(bus);
}

static void ssd1680_data_entry_mode_set(ssd1680_ctl_t *disp, uint8_t data_mode)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, SSD1680_DATA_ENTRY_MODE_CTL); // data entry mode
    bus_data_byte(bus, data_mode);
    bus_finish(bus);
}

static void ssd1680_set_ram_start_pos(ssd1680_ctl_t *disp)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, SSD1680_SET_RAM_X_ADDRESS_START_END); // set Ram-X address start/end position
    bus_data_byte(bus, 0x00);
    bus_data_byte(bus, 0x0F); // 0x0F-->(15+1)*8=128
    bus_finish(bus);

    bus_cmd(bus, SSD1680_SET_RAM_Y_ADDRESS_START_END); // set Ram-Y address start/end position
    bus_data_byte(bus, 0x00);
    bus_data_byte(bus, 0x00);
    bus_data_byte(bus, 0x27); // 0x127-->(295+1)=296
    bus_data_byte(bus, 0x01);
    bus_finish(bus);
}

static void ssd1680_border_waveform_set(ssd1680_ctl_t *disp, uint8_t border_waveform)
{
    sd1860_bus_t *bus = &disp->bus;

    bus_cmd(bus, SSD1680_BORDER_WAVEFORM_CTL); // BorderWavefrom
    bus_data_byte(bus, border_waveform);
    bus_finish(bus);
}

static void ssd1680_display_update_ctl_set(ssd1680_ctl_t *disp, uint8_t ram_option, uint8_t output_mode)
{
    sd1860_bus_t *bus = &disp->bus;

    bus_cmd(bus, SSD1680_DISPLAY_UPDATE_CTL); // BorderWavefrom
    bus_data_byte(bus, ram_option);
    bus_data_byte(bus, output_mode);
    bus_finish(bus);
}

static void ssd1680_temp_sensor_set(ssd1680_ctl_t *disp)
{
    sd1860_bus_t *bus = &disp->bus;

    bus_cmd(bus, SSD1680_TEMPERATURE_SENSOR_CTL);
    // bus_data(bus, 0x48); //external temperatrure sensor
    bus_data_byte(bus, 0x80); // internal temperatrure sensor
    bus_finish(bus);
}

static void ssd1680_soft_start(ssd1680_ctl_t *disp, uint8_t phase1_ss, uint8_t phase2_ss, uint8_t phase3_ss, uint8_t duration)
{
    sd1860_bus_t *bus = &disp->bus;

    bus_cmd(bus, SSD1680_BOOSTER_SOFT_START_CTL);
    bus_data_byte(bus, phase1_ss);
    bus_data_byte(bus, phase2_ss);
    bus_data_byte(bus, phase3_ss);
    bus_data_byte(bus, duration);
    bus_finish(bus);
}

static void ssd1680_display_update_ctl2_set(ssd1680_ctl_t *disp)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, SSD1680_DISPLAY_UPDATE_CTL2);
    // bus_data(bus, 0x48);
    bus_data_byte(bus, 0xF7);
    bus_finish(bus);
}

static void ssd1680_activate(ssd1680_ctl_t *disp)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, 0x20); // Activate Display Update Sequence
    bus_finish(bus);
    bus_waitbusy(bus);
}

static void ssd1680_counter_x_set(ssd1680_ctl_t *disp, uint8_t x_pos)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, SSD1680_SET_RAM_X_ADDRESS_COUNTER);
    bus_data_byte(bus, x_pos);
    bus_finish(bus);
}

static void ssd1680_counter_y_set(ssd1680_ctl_t *disp, uint16_t y_pos)
{
    sd1860_bus_t *bus = &disp->bus;
    bus_cmd(bus, SSD1680_SET_RAM_Y_ADDRESS_COUNTER);
    bus_data_byte(bus, y_pos & 0xff);
    bus_data_byte(bus, (y_pos >> 8) & 0x01);
    bus_finish(bus);
}

static void ssd1680_write_data_color(ssd1680_ctl_t *disp, uint8_t *data, uint16_t len, uint8_t color)
{
    sd1860_bus_t *bus = &disp->bus;
    if (color == SSD1680_COLOR_BW)
    {
        bus_cmd(bus, 0x24);
    }
    else
    {
        bus_cmd(bus, 0x26);
    }
    if (data == NULL)
    {
        for (int i = 0; i < (disp->width * disp->height) / 8; i++)
        {
            bus_data_byte(bus, 0x00);
        }
    }
    else
    {
        ESP_LOGI(TAG, "bus_data_stream: %p %d", data, (disp->width * disp->height) / 8);
        bus_data_stream(bus, data, (disp->width * disp->height) / 8);
        // for (int i = 0; i < (disp->width * disp->height) / 8; i++)
        // {
        //     bus_data_byte(bus, data[i]);
        // }
    }

    bus_finish(bus);
}

static void ssd1680_write_data_bw(ssd1680_ctl_t *disp, uint8_t *data, uint16_t len)
{
    ssd1680_write_data_color(disp, data, len, SSD1680_COLOR_BW);
}

static void ssd1680_write_data_red(ssd1680_ctl_t *disp, uint8_t *data, uint16_t len)
{
    ssd1680_write_data_color(disp, data, len, SSD1680_COLOR_RED);
}

const uint8_t lut[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static void ssd1680_set_lut(ssd1680_ctl_t *disp)
{
    sd1860_bus_t *bus = &disp->bus;
    // COMMAND BOOSTER SOFT START CONTROL
    bus_cmd(bus, 0x0C);
    bus_data_byte(bus, 0xD7);
    bus_data_byte(bus, 0xD6);
    bus_data_byte(bus, 0x9D);
    bus_finish(bus);

    // COMMAND WRITE VCOM REGISTER
    bus_cmd(bus, 0x2C);
    bus_data_byte(bus, 0x9B);
    bus_finish(bus);

    // COMMAND SET DUMMY LINE PERIOD
    bus_cmd(bus, 0x3A);
    bus_data_byte(bus, 0x1A);
    bus_finish(bus);

    // COMMAND SET GATE TIME
    bus_cmd(bus, 0x3B);
    bus_data_byte(bus, 0x08); // 2µs per row
    bus_finish(bus);

    bus_cmd(bus, 0x32);
    for (int i = 0; i < sizeof(lut); i++)
    {
        bus_data_byte(bus, lut[i]);
    }
    bus_finish(bus);
}

void ssd1680_cfg(ssd1680_ctl_t *disp)
{
    // 3. Send Initialization Code

    // • Set gate driver output by Command 0x01
    ssd1680_doc_set(disp, 0x00); // 0x00: G0, G1, G2, G3, … G295

    // • Set display RAM size by Command 0x11, 0x44, 0x45
    // 0x01: X increment, Y increment, address counter increment in X direction
    ssd1680_data_entry_mode_set(disp,
                                REG_DEM_X_INC | REG_DEM_Y_INC | REG_DEM_CNT_INC_X);

    ssd1680_set_ram_start_pos(disp);

    // • Set panel border by Command 0x3C
    ssd1680_border_waveform_set(disp,
                                REG_BWC_VBD_USE_GS_TRANS | REG_BWC_GS_TRANS_LUT | REG_BWC_LUT1);

    // 4. Load Waveform LUT
    // • Wait BUSY Low
    ssd1680_set_lut(disp);

    ssd1680_display_update_ctl_set(disp,
                                   REG_DUC_BW_NORMAL | REG_DUC_RED_NORMAL,
                                   REG_DUC_SRC_8_TO_167);
    // • Sense temperature by int/ext TS by Command 0x18
    ssd1680_temp_sensor_set(disp);

    // • Load waveform LUT from OTP by Command 0x22, 0x20
    // or by MCU
    ssd1680_display_update_ctl2_set(disp);
    ssd1680_activate(disp);
}

// Function to initialize the SSD1680
void ssd1680_init(ssd1680_ctl_t *disp)
{
    sd1860_bus_t *bus = &disp->bus;
    ESP_LOGD(TAG, "HW resetting bus...");
    bus_reset(bus);

    ESP_LOGD(TAG, "SW resetting bus...");
    bus_cmd(bus, SSD1680_SW_RESET);

    ESP_LOGI(TAG, "Wait for BUSY");
    bus_waitbusy(bus);

    ssd1680_cfg(disp);

    ESP_LOGI(TAG, "Init done");
}

// Function to clear the display
void ssd1680_clear(ssd1680_ctl_t *disp)
{
    // 5. Write Image and Drive Display Panel
    // • Write image data in RAM by Command 0x4E, 0x4F,
    // 0x24, 0x26

    ssd1680_counter_x_set(disp, 0);
    ssd1680_counter_y_set(disp, 0x127);

    ssd1680_write_data_bw(disp, NULL, disp->width * disp->height);
    // TODO: support red color
    // ssd1680_write_data_red(disp, NULL, disp->width * disp->height);
    // • Set softstart setting by Command 0x0C
    ssd1680_soft_start(disp,
                       REG_BSSC_PHASE_STRENGTH_1 | REG_BSSC_PHASE_DELAY_2_6,
                       REG_BSSC_PHASE_STRENGTH_1 | REG_BSSC_PHASE_DELAY_2_6,
                       REG_BSSC_PHASE_STRENGTH_1 | REG_BSSC_PHASE_DELAY_2_6,
                       REG_BSSC_PHASE1_DURATION_10MS | REG_BSSC_PHASE2_DURATION_10MS | REG_BSSC_PHASE3_DURATION_10MS);

    // • Drive display panel by Command 0x22, 0x20
    ssd1680_display_update_ctl2_set(disp);
    ssd1680_activate(disp);
}

// Function to update the display
void ssd1680_update(ssd1680_ctl_t *disp)
{
    ssd1680_display_update_ctl2_set(disp);
    ssd1680_activate(disp);
}

void ssd1680_upload(ssd1680_ctl_t *disp)
{
    ssd1680_counter_x_set(disp, 0);
    ssd1680_counter_y_set(disp, 0x127);

    ssd1680_write_data_bw(disp, disp->buffer, disp->width * disp->height);
    ssd1680_write_data_red(disp, disp->buffer, disp->width * disp->height);
}