#ifndef __DRAW_H__
#define __DRAW_H__
#include "ssd1680.h"

// Colors for drawing
#define SSD1680_BLACK 0
#define SSD1680_WHITE 1

typedef struct
{
    const uint8_t width;  ///< Width of the font
    const uint8_t height; ///< Height of the font
    const uint8_t *data;  ///< Font data
} FontDef;

/**
 * @brief Draw a character on the display
 *
 * @param disp Display control structure
 * @param x X coordinate
 * @param y Y coordinate
 * @param c Character to draw
 * @param font Font to use
 * @param color Color of the character
 * @param bgcolor Background color
 */
void ssd1680_draw_char(ssd1680_ctl_t *disp, int x, int y, char c, const FontDef *font, uint16_t color, uint16_t bgcolor);

/**
 * @brief Draw a string on the display
 *
 * @param disp Display control structure
 * @param x X coordinate
 * @param y Y coordinate
 * @param text String to draw
 * @param font Font to use
 * @param color Color of the string
 * @param bgcolor Background color
 */
void ssd1680_draw_string(ssd1680_ctl_t *disp, int x, int y, const char *text, const FontDef *font, uint16_t color, uint16_t bgcolor);

#endif // __DRAW_H__