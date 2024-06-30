#include <stdint.h>
#include "ssd1680.h"
#include "draw.h"

// TODO: refactor
#define EPD_WIDTH 128
#define EPD_HEIGHT 296

void ssd1680_draw_pixel(ssd1680_ctl_t *disp, int x, int y, uint16_t color)
{
    if (x < 0 || x >= EPD_WIDTH || y < 0 || y >= EPD_HEIGHT)
    {
        return; // Out of bounds
    }

    int byteIndex = (x + y * EPD_WIDTH) / 8;
    int bitIndex = x % 8;

    if (color == SSD1680_BLACK)
    {
        disp->buffer[byteIndex] &= ~(1 << bitIndex); // Set bit to 0 for black
    }
    else
    {
        disp->buffer[byteIndex] |= (1 << bitIndex); // Set bit to 1 for white
    }
}

void ssd1680_draw_char(ssd1680_ctl_t *disp, int x, int y, char c, const FontDef *font, uint16_t color, uint16_t bgcolor)
{
    uint32_t b;
    for (uint32_t i = 0; i < font->height; i++)
    {
        b = font->data[(c)*font->height + i];
        for (uint32_t j = 0; j < font->width; j++)
        {
            if ((b << j) & 0x80)
            {
                ssd1680_draw_pixel(disp, x + j, y + i, color);
            }
            else
            {
                ssd1680_draw_pixel(disp, x + j, y + i, bgcolor);
            }
        }
    }
}

// // Function to draw a string on the display
void ssd1680_draw_string(ssd1680_ctl_t *disp, int x, int y, const char *text, const FontDef *font, uint16_t color, uint16_t bgcolor)
{
    while (*text)
    {
        if (x + font->width > disp->width)
        {
            x = 0;
            y += font->height;
            if (y + font->height >= disp->height)
            {
                break;
            }
        }
        ssd1680_draw_char(disp, x, y, *text, font, color, bgcolor);
        x += font->width;
        text++;
    }
}
