#ifndef __SSD1680_H__
#define __SSD1680_H__
#include "bus.h"

/** Driver Output Control */
#define SSD1680_DRIVER_OUTPUT_CTL 0x01 //!< Driver Output Control - This command sets the display size and the scan direction.
#define REG_DOC_TB 0b00000001          //!< Gate scan direction (0 = G1 to G295, 1 = G295 to G1)
#define REG_DOC_SM 0b00000010          //!< Source output scan direction (0 = S1 to G295, 1 = G0,G2,G4…G294, G1,G3,…G295)
#define REG_DOC_GD 0b00000100          //!< Gate driver direction (0 = normal(G0,G1,G2,G3,…), 1 = G1,G0,G3,G2,…)

/** Gate Driving Voltage Control */
#define SSD1680_GATE_DRIVING_VOLTAGE_CTL 0x03 //!< Gate Driving Voltage Control - This command controls the gate driving voltage.

/** Source Driving Voltage Control */
#define SSD1680_SOURCE_DRIVING_VOLTAGE_CTL 0x04 //!< Source Driving Voltage Control - This command controls the source driving voltage.

/** Booster Soft Start Control */
#define SSD1680_BOOSTER_SOFT_START_CTL 0x0C      //!< Booster Soft Start Control - This command configures the booster soft start.
#define REG_BSSC_PHASE_STRENGTH_1 0b10000000     //!< Phase drive strength 1 (Weakest)
#define REG_BSSC_PHASE_STRENGTH_2 0b10010000     //!< Phase drive strength 2
#define REG_BSSC_PHASE_STRENGTH_3 0b10100000     //!< Phase drive strength 3
#define REG_BSSC_PHASE_STRENGTH_4 0b10110000     //!< Phase drive strength 4
#define REG_BSSC_PHASE_STRENGTH_5 0b11000000     //!< Phase drive strength 5
#define REG_BSSC_PHASE_STRENGTH_6 0b11010000     //!< Phase drive strength 6
#define REG_BSSC_PHASE_STRENGTH_7 0b11100000     //!< Phase drive strength 7
#define REG_BSSC_PHASE_STRENGTH_8 0b11110000     //!< Phase drive strength 8 (Strongest)
#define REG_BSSC_PHASE_DELAY_2_6 0b00000100      //!< Min Off Time Setting of GDR 2.6
#define REG_BSSC_PHASE_DELAY_3_2 0b00000101      //!< Min Off Time Setting of GDR 3.2
#define REG_BSSC_PHASE_DELAY_3_9 0b00000110      //!< Min Off Time Setting of GDR 3.9
#define REG_BSSC_PHASE_DELAY_4_6 0b00000111      //!< Min Off Time Setting of GDR 4.6
#define REG_BSSC_PHASE_DELAY_5_4 0b00001000      //!< Min Off Time Setting of GDR 5.4
#define REG_BSSC_PHASE_DELAY_6_3 0b00001001      //!< Min Off Time Setting of GDR 6.3
#define REG_BSSC_PHASE_DELAY_7_3 0b00001010      //!< Min Off Time Setting of GDR 7.3
#define REG_BSSC_PHASE_DELAY_8_4 0b00001011      //!< Min Off Time Setting of GDR 8.4
#define REG_BSSC_PHASE_DELAY_9_8 0b00001100      //!< Min Off Time Setting of GDR 9.8
#define REG_BSSC_PHASE_DELAY_11_5 0b00001101     //!< Min Off Time Setting of GDR 11.5
#define REG_BSSC_PHASE_DELAY_13_8 0b00001110     //!< Min Off Time Setting of GDR 13.8
#define REG_BSSC_PHASE_DELAY_16_5 0b00001111     //!< Min Off Time Setting of GDR 16.5
#define REG_BSSC_PHASE1_DURATION_10MS 0b00000000 //!< Phase 1 duration
#define REG_BSSC_PHASE1_DURATION_20MS 0b00000001 //!< Phase 1 duration
#define REG_BSSC_PHASE1_DURATION_30MS 0b00000010 //!< Phase 1 duration
#define REG_BSSC_PHASE1_DURATION_40MS 0b00000011 //!< Phase 1 duration
#define REG_BSSC_PHASE2_DURATION_10MS 0b00000000 //!< Phase 2 duration
#define REG_BSSC_PHASE2_DURATION_20MS 0b00000100 //!< Phase 2 duration
#define REG_BSSC_PHASE2_DURATION_30MS 0b00001000 //!< Phase 2 duration
#define REG_BSSC_PHASE2_DURATION_40MS 0b00001100 //!< Phase 2 duration
#define REG_BSSC_PHASE3_DURATION_10MS 0b00000000 //!< Phase 3 duration
#define REG_BSSC_PHASE3_DURATION_20MS 0b00010000 //!< Phase 3 duration
#define REG_BSSC_PHASE3_DURATION_30MS 0b00100000 //!< Phase 3 duration
#define REG_BSSC_PHASE3_DURATION_40MS 0b00110000 //!< Phase 3 duration

/** Deep Sleep Mode */
#define SSD1680_DEEP_SLEEP_MODE 0x10 //!< Deep Sleep Mode - This command puts the device into deep sleep mode to save power.

/** Data Entry Mode Setting */
#define SSD1680_DATA_ENTRY_MODE_CTL 0x11 //!< Data Entry Mode Setting - This command sets the data entry mode, specifying the address counter direction.
#define REG_DEM_X_INC 0b00000001         //!< X increment
#define REG_DEM_Y_INC 0b00000010         //!< Y increment
#define REG_DEM_X_DEC 0b00000000         //!< X decrement
#define REG_DEM_Y_DEC 0b00000000         //!< Y decrement
#define REG_DEM_CNT_INC_X 0b00000000     //!< Address counter increment in X direction
#define REG_DEM_CNT_INC_Y 0b00000100     //!< Address counter increment in Y direction

/** Software Reset */
#define SSD1680_SW_RESET 0x12 //!< SW Reset - This command performs a software reset.

/** Temperature Sensor Control */
#define SSD1680_TEMPERATURE_SENSOR_CTL 0x18 //!< Temperature Sensor Control - This command configures the temperature sensor.

/** Master Activation */
#define SSD1680_MASTER_ACTIVATION 0x20 //!< Master Activation - This command activates the display update.

/** Display Update Control 1 */
#define SSD1680_DISPLAY_UPDATE_CTL 0x21 //!< Display Update Control - This command controls the display update sequence.
#define REG_DUC_RED_NORMAL 0b00000000   //!< Red Normal
#define REG_DUC_RED_BYPASS 0b01000000   //!< Red Bypass
#define REG_DUC_RED_INVESE 0b10000000   //!< Red Inverse
#define REG_DUC_BW_NORMAL 0b00000000    //!< B/W Normal
#define REG_DUC_BW_BYPASS 0b00000100    //!< B/W Bypass
#define REG_DUC_BW_INVESE 0b00001000    //!< B/W Inverse
#define REG_DUC_SRC_0_TO_175 0b00000000 //!< Source 0 to 175
#define REG_DUC_SRC_8_TO_167 0b10000000 //!< Source 175 to 0

/** Write Data to RAM */
#define SSD1680_WRITE_RAM 0x24 //!< Write RAM - This command writes data to the display RAM.

/** Write VCOM register */
#define SSD1680_WRITE_VCOM_REGISTER 0x2C //!< Write VCOM Register - This command writes to the VCOM register.

/** Read VCOM register */
#define SSD1680_READ_VCOM_REGISTER 0x2F //!< Read VCOM Register - This command reads the VCOM register.

/** Write LUT register */
#define SSD1680_WRITE_LUT_REGISTER 0x32 //!< Write LUT Register - This command writes to the LUT (Look-Up Table) register.

/** Set Dummy Line Period */
#define SSD1680_SET_DUMMY_LINE_PERIOD 0x3A //!< Set Dummy Line Period - This command sets the dummy line period.

/** Set Gate Line Width */
#define SSD1680_SET_GATE_LINE_WIDTH 0x3B //!< Set Gate Line Width - This command sets the gate line width.

/** Border Waveform Control */
#define SSD1680_BORDER_WAVEFORM_CTL 0x3C //!< Border Waveform Control - This command controls the border waveform.
#define REG_BWC_LUT0 0b00000000          //!< White border waveform
#define REG_BWC_LUT1 0b00000001          //!< Black and white border waveform
#define REG_BWC_LUT2 0b00000010          //!< Gray and white border waveform
#define REG_BWC_LUT3 0b00000011          //!< Gray and white border waveform with gray transition

#define REG_BWC_GS_TRANS_LUT 0b00000000     //!< Gray scale transition border waveform
#define REG_BWC_GS_TRANS_LUT_RED 0b00000100 //!< Red border waveform

#define REG_BWC_VBD_FIX_VSS 0b00000000  //!< VBD level 0
#define REG_BWC_VBD_FIX_VSH1 0b00010000 //!< VBD level 1
#define REG_BWC_VBD_FIX_VSL 0b00100000  //!< VBD level 2
#define REG_BWC_VBD_FIX_VSH2 0b00110000 //!< VBD level 3

#define REG_BWC_VBD_USE_GS_TRANS 0b00000000 //!< GS Transition, Defined in A[2] and A[1:0]
#define REG_BWC_VBD_USE_FIX 0b01000000      //!< Fix Level, Defined in A[5:4]
#define REG_BWC_VBD_USE_VCOM 0b10000000     //!< VCOM
#define REG_BWC_VBD_USE_HIZ 0b11000000      //!< HiZ

/** Display Update Control 2 */
#define SSD1680_DISPLAY_UPDATE_CTL2 0x22 //!< Display Update Control 2 - This command controls the display update sequence.
#define REG_DUC2_MODE_ENCLK_ANALOG 0xC0
#define REG_DUC2_MODE_DISCLK_ANALOG 0x03
#define REG_DUC2_MODE_ENCLK_TEMP_MODE1_DISANALOG_DISOSC 0xF7
#define REG_DUC2_MODE_ENCLK_TEMP_MODE2_DISANALOG_DISOSC 0xFF

/** Set RAM X Address Start/End */
#define SSD1680_SET_RAM_X_ADDRESS_START_END 0x44 //!< Set RAM X Address Start/End - This command sets the RAM X address start and end positions.

/** Set RAM Y Address Start/End */
#define SSD1680_SET_RAM_Y_ADDRESS_START_END 0x45 //!< Set RAM Y Address Start/End - This command sets the RAM Y address start and end positions.
#define SSD1680_SET_RAM_X_ADDRESS_COUNTER 0x4E   //!< Set RAM X Address Counter - This command sets the RAM X address counter.
#define SSD1680_SET_RAM_Y_ADDRESS_COUNTER 0x4F   //!< Set RAM Y Address Counter - This command sets the RAM Y address counter.

/** Dummy command */
#define SSD1680_NOP 0x7F //!< NOP - This command does nothing (No Operation).

#define SSD1680_COLOR_BW 0  //!< Black and white color
#define SSD1680_COLOR_RED 1 //!< Red color

typedef struct
{
    sd1860_bus_t bus; //< Bus configuration
    uint16_t width;   //< Width of the display
    uint16_t height;  //< Height of the display
    int orientation;  //< Orientation of the display
    uint8_t *buffer;  //< Buffer to hold the pixel data
} ssd1680_ctl_t;

/**
 * @brief Initialize the display
 *
 * @param disp Display control structure
 */
void ssd1680_init(ssd1680_ctl_t *disp);

/**
 * @brief Upload the buffer to the display
 *
 * @param disp Display control structure
 */
void ssd1680_upload(ssd1680_ctl_t *disp);

/**
 * @brief Update the display
 *
 * @param disp Display control structure
 */
void ssd1680_update(ssd1680_ctl_t *disp);

/**
 * @brief Clear the display
 *
 * @param disp Display control structure
 */
void ssd1680_clear(ssd1680_ctl_t *disp);

#endif // __SSD1680_H__