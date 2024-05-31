#pragma once
#include <pico/stdlib.h>
#include <pico/time.h>
#include <hardware/spi.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif
// Shorter types
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct {
	const u8 width;
	const u16 offset;	
} FONT_CHAR_INFO;	
typedef struct {
	const u8 height;
	const char startCh;
	const char endCh;
	const FONT_CHAR_INFO* charDesc;
	const u8* bitmap;
} FONT_INFO;
#define PROGMEM
#include "font.h"
// Small conversion layer between the Arudino SDK and Pi Pico SDK
typedef enum _ePinType {
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    OUTPUT,
    OUTPUT_2MA,
    OUTPUT_4MA,
    OUTPUT_8MA,
    OUTPUT_12MA,
} ePinType;
typedef enum _ePinStatus {
    LOW = 0,
    HIGH = 1,
} ePinStatus;
static inline void delay(u32 value) {
    sleep_ms(value);
}
static inline size_t millis() {
    return us_to_ms(to_us_since_boot(get_absolute_time()));
}
static inline void pinMode(u8 pin, ePinType pin_type) {
    gpio_init(pin);
    switch (pin_type) {
        case INPUT: {
            gpio_set_dir(pin, GPIO_IN);
            gpio_disable_pulls(pin);
        } break;
        case INPUT_PULLUP: {
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_up(pin);
        } break;
        case INPUT_PULLDOWN: {
            gpio_set_dir(pin, GPIO_IN);
            gpio_pull_down(pin);
        } break;
        case OUTPUT_2MA: {
            gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_2MA);
            gpio_set_dir(pin, true);
        } break;
        case OUTPUT:
        case OUTPUT_4MA: {
            gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_4MA);
            gpio_set_dir(pin, true);
        } break;
        case OUTPUT_8MA: {
            gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_8MA);
            gpio_set_dir(pin, true);
        } break;
        case OUTPUT_12MA: {
            gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_12MA);
            gpio_set_dir(pin, true);
        } break;
    }
}
static inline void digitalWrite(u8 pin, ePinStatus pin_status) {
    // No need for a switch case, the values of the enum *should* match it.
    gpio_put(pin, pin_status);
}
static inline u8 digitalRead(size_t pin) {
    return gpio_get(pin) ? HIGH : LOW;
}
static inline void Serial_begin(size_t speed) {
    stdio_usb_init();
}
static inline void Serial1_begin(size_t speed) {
    uart_init(uart0, speed);
}
static inline bool Serial1_available() {
    return uart_is_readable(uart0);
}
static inline char Serial1_read() {
    return uart_getc(uart0);
}
static inline void Serial1_write(const u8* data, size_t size) {
    // This is to avoid CR/LF
    uart_write_blocking(uart0, data, size);
}
static inline size_t Serial1_readBytes(u8* data, size_t size) {
    // This is to avoid CR/LF
    uart_read_blocking(uart0, data, size);
    return size;
}
// https://github.com/TinyCircuits/TinyCircuits-TinierScreen-Lib/blob/master/src/TinierScreen.h#L33-L62
// Screen commands
#define SSD1306_DEFAULT_ADDRESS 0x3C
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3
// ID Pin
#define THUMBY_ID_PIN 15
#define THUMBY_ID_PIN2 14
// Screen SPI pins
#define THUMBY_CS_PIN 16
#define THUMBY_DC_PIN 17
#define THUMBY_SCK_PIN 18
#define THUMBY_SDA_PIN 19 // Also known as TX/MOSI
#define THUMBY_RESET_PIN 20
// Link pins
#define THUMBY_LINK_TX_PIN 0
#define THUMBY_LINK_RX_PIN 1
#define THUMBY_LINK_PU_PIN 2
// Buttons
#define THUMBY_BTN_LDPAD_PIN 3
#define THUMBY_BTN_RDPAD_PIN 5
#define THUMBY_BTN_UDPAD_PIN 4
#define THUMBY_BTN_DDPAD_PIN 6
#define THUMBY_BTN_B_PIN 24
#define THUMBY_BTN_A_PIN 27
// PWM Piezo speaker pin
#define THUMBY_AUDIO_PIN 28
// Screen reset pin
#define THUMBY_SCREEN_RESET_PIN 20
// Screen size
#define THUMBY_SCREEN_WIDTH 72
#define THUMBY_SCREEN_HEIGHT 40
// Button bit masks
#define BUTTON_L 0b00000001
#define BUTTON_R 0b00000010
#define BUTTON_U 0b00000100
#define BUTTON_D 0b00001000
#define BUTTON_B 0b00010000
#define BUTTON_A 0b00100000
// Sections of code from https://github.com/daschr/pico-ssd1306/blob/main/ssd1306.h
// Rewritten for SPI
typedef struct spi_inst_c {
    u8 sclk_pin;
    u8 sda_pin;
    u8 cs_pin;
    u8 dc_pin;
    u8 rst_pin;
    spi_inst_t* spi;
} spi_inst_ct;
typedef struct _ssd1306_t {
    u8 width;
    u8 height;
    spi_inst_ct spi_i;
    u8* buffer;
    size_t bufsize;
} ssd1306_t;
static inline void ssd1306_write_multi(ssd1306_t* p, u8* data, size_t size, bool is_data) {
    digitalWrite(p->spi_i.dc_pin, is_data ? HIGH : LOW);
    spi_write_blocking(p->spi_i.spi, data, size);
}
static inline void ssd1306_write(ssd1306_t* p, u8 val, bool is_data) {
    ssd1306_write_multi(p, &val, 1, is_data);
}
// https://github.com/Aloshi/pico-ssd1306/tree/master
static inline void ssd1306_send_init_commands(ssd1306_t* p) {
    digitalWrite(p->spi_i.rst_pin, LOW);
    delay(10);
    digitalWrite(p->spi_i.rst_pin, HIGH);
    // Send init commands
    u8 cmds[] = {
        SSD1306_DISPLAYOFF,
        SSD1306_SETDISPLAYCLOCKDIV,
        0x80,
        SSD1306_SETDISPLAYOFFSET,
        0x00,
        SSD1306_SETSTARTLINE | 0x00,
        SSD1306_DISPLAYALLON_RESUME,
        SSD1306_NORMALDISPLAY,
        SSD1306_CHARGEPUMP,
        0x14,
        SSD1306_MEMORYMODE,
        0x00,
        SSD1306_SEGREMAP | 1,
        SSD1306_COMSCANDEC,
        SSD1306_SETCONTRAST,
        30,
        SSD1306_SETPRECHARGE,
        0xF1,
        SSD1306_SETVCOMDETECT,
        0x20,
        SSD1306_SETMULTIPLEX,
        40 - 1,
        SSD1306_SETCOMPINS,
        0x12,
        0xAD,
        0x30,
        SSD1306_DISPLAYON
    };
    for (size_t i = 0; i != sizeof(cmds); ++i) {
        ssd1306_write(p, cmds[i], false);
    }
}
static inline void ssd1306_send_window_commands(ssd1306_t* p) {
    u8 window_cmd[] = {
        SSD1306_PAGEADDR,
        0x00,
        0x05,
        SSD1306_COLUMNADDR,
        0x1C,
        0x64-1
    };
    for (size_t i = 0; i != sizeof(window_cmd); ++i) {
        ssd1306_write(p, window_cmd[i], false);
    }
}
static inline bool ssd1306_init(ssd1306_t* p, u8 sda_pin, u8 sclk_pin, u8 cs_pin, u8 dc_pin, u8 rst_pin, u16 width, u16 height, spi_inst_t* spi_instance) {
    p->width = width;
    p->height = height;
    p->spi_i.sda_pin = sda_pin;
    p->spi_i.sclk_pin = sclk_pin;
    p->spi_i.cs_pin = cs_pin;
    p->spi_i.dc_pin = dc_pin;
    p->spi_i.rst_pin = rst_pin;
    p->spi_i.spi = spi_instance;
    p->bufsize = width * ((height + 7) / 8);
    // Create buffer
    p->buffer = malloc(p->bufsize);
    if(!p->buffer) {
        p->bufsize = 0;
        return false;
    }
    // Reset display, leave it low
    pinMode(p->spi_i.rst_pin, OUTPUT);
    digitalWrite(p->spi_i.rst_pin, LOW);
    // Init GPIO
    spi_init(spi0, 10 * 1024 * 1024);
    gpio_set_function(p->spi_i.sclk_pin, GPIO_FUNC_SPI);
    gpio_set_function(p->spi_i.cs_pin, GPIO_FUNC_SIO);
    gpio_set_function(p->spi_i.sda_pin, GPIO_FUNC_SPI);
    // Select the display (leave it low, we will always use it regardless)
    pinMode(p->spi_i.cs_pin, OUTPUT);
    digitalWrite(p->spi_i.cs_pin, LOW);
    // Default to cmd mode
    pinMode(p->spi_i.dc_pin, OUTPUT);
    digitalWrite(p->spi_i.dc_pin, LOW); 
    // Init cmds
    delay(1);
    ssd1306_send_init_commands(p);
    ssd1306_send_window_commands(p);
    return true;
}
static inline void ssd1306_shutdown(ssd1306_t* p) {
    free(p->buffer - 1);
}
static inline void ssd1306_poweroff(ssd1306_t* p) {
    ssd1306_write(p, SSD1306_DISPLAYOFF, false);
}
static inline void ssd1306_poweron(ssd1306_t* p) {
    ssd1306_write(p, SSD1306_DISPLAYON, false);
}
static inline void ssd1306_contrast(ssd1306_t* p, u8 v) {
    ssd1306_write(p, SSD1306_SETCONTRAST, false);
    ssd1306_write(p, v, false);
}
static inline void ssd1306_invert(ssd1306_t* p, u8 v) {
    ssd1306_write(p, SSD1306_INVERTDISPLAY | (v & 1), false);
}
static inline void ssd1306_clear(ssd1306_t* p) {
    memset(p->buffer, 0, p->bufsize);
}
typedef enum _eSSD1306PixelColor {
    SSD1306_COLOR_OFF = 0,
    SSD1306_COLOR_ON = 1,
    SSD1306_COLOR_INVERTED = 2
} eSSD1306PixelColor;
static inline void ssd1306_draw_pixel(ssd1306_t* p, u32 x, u32 y, u16 color) {
    if (x >= p->width || y >= p->height)
        return;
    switch (color) {
        case 0xFFFF:
        case SSD1306_COLOR_ON: {
            p->buffer[x + p->width * (y >> 3)] |= (0x1 << (y & 0x07));
        } break;
        case 0xAAAA:
        case SSD1306_COLOR_OFF: {
            p->buffer[x + p->width * (y >> 3)] &= ~(0x1 << (y & 0x07));
        } break;
        case SSD1306_COLOR_INVERTED: {
            p->buffer[x + p->width * (y >> 3)] &= ~(0x1 << (y & 0x07));
        } break;
    }
}
// Modified for the Thumby.
static inline void ssd1306_transfer_buffer(ssd1306_t* p, u8* buffer, int bufferLen) {
    ssd1306_send_window_commands(p);
    ssd1306_write_multi(p, p->buffer, p->bufsize, true);
}
ssd1306_t display_instance;
// GraphicsBuffer reimpl with some things omitted
typedef struct _GraphicsBuffer {
    s32 cursorX, cursorY;
    u8 xMax, yMax, pixelXinc, pixelYinc, cursorXmin, cursorYmin, cursorXmax, cursorYmax;
    u8 fontHeight, fontFirstCh, fontLastCh;
    u16 fontColor, fontBGcolor;
    const FONT_CHAR_INFO* fontDescriptor;
    const u8* fontBitmap;
} GraphicsBuffer;
GraphicsBuffer graphics_buffer;
#define ALPHA_COLOR 0xAAAA
static inline void GraphicsBuffer_init(const u16 width, const u16 height) {
    graphics_buffer.xMax = width - 1;
    graphics_buffer.yMax = height - 1;
    graphics_buffer.fontColor = 0xFFFF;
    graphics_buffer.fontBGcolor = ALPHA_COLOR;
}
static inline void GraphicsBuffer_setX(u8 x, u8 end) {
    if (x > graphics_buffer.xMax)
      x = graphics_buffer.xMax;
    if (end > graphics_buffer.xMax)
        end = graphics_buffer.xMax;
    graphics_buffer.cursorX = x;
    graphics_buffer.pixelXinc = x;
    graphics_buffer.cursorXmin = x;
    graphics_buffer.cursorXmax = end;
}
static inline void GraphicsBuffer_setY(u8 y, u8 end) {
    if (y > graphics_buffer.yMax)
      y = graphics_buffer.yMax;
    if (end > graphics_buffer.yMax)
        end = graphics_buffer.yMax;
    graphics_buffer.cursorY = y;
    graphics_buffer.pixelYinc = y;
    graphics_buffer.cursorYmin = y;
    graphics_buffer.cursorYmax = end;
}
static inline void GraphicsBuffer_goTo(u8 x, u8 y) {
    if (x > graphics_buffer.xMax || y > graphics_buffer.yMax)
        return;
    GraphicsBuffer_setX(x, graphics_buffer.xMax);
    GraphicsBuffer_setY(y, graphics_buffer.yMax);
}
static inline void GraphicsBuffer_drawPixel(s16 x, s16 y, u16 color) {
    if (x > graphics_buffer.xMax || y > graphics_buffer.yMax)
        return;
    GraphicsBuffer_goTo(x, y);
	ssd1306_draw_pixel(&display_instance, x, y, color);
}
static inline void GraphicsBuffer_incrementPixel() {
    graphics_buffer.pixelXinc++;
    if (graphics_buffer.pixelXinc > graphics_buffer.cursorXmax) {
        graphics_buffer.pixelXinc = graphics_buffer.cursorXmin;
        graphics_buffer.pixelYinc++;
        if (graphics_buffer.pixelYinc >= graphics_buffer.cursorYmax) {
            graphics_buffer.pixelYinc = graphics_buffer.cursorYmin;
        }
    }
}
static inline void GraphicsBuffer_writePixel(u16 color) {
    u16 pos = graphics_buffer.pixelXinc;
    if (graphics_buffer.pixelYinc > 7) {
        pos += (graphics_buffer.pixelYinc/8) * (graphics_buffer.xMax+1);
    }
    u8 bitNum = (graphics_buffer.pixelYinc % 8);
    if (color) {
        display_instance.buffer[pos] |= (1 << (bitNum));
    }
    else {
        display_instance.buffer[pos] &= ~(1 << (bitNum));
    }
    GraphicsBuffer_incrementPixel();
}
static inline void GraphicsBuffer_clear() {
    ssd1306_clear(&display_instance);
}
#define swap_16(a, b) { s16 t = a; a = b; b = t; }
static inline void GraphicsBuffer_drawLine(s16 x0, s16 y0, s16 x1, s16 y1, u16 color) {
	s16 steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap_16(x0, y0);
		swap_16(x1, y1);
	}
	if (x0 > x1) {
		swap_16(x0, x1);
		swap_16(y0, y1);
	}
	s16 dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);
	s16 err = dx / 2;
	s16 ystep;
	if (y0 < y1) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}
	for (; x0 <= x1; x0++) {
		if (steep) {
			GraphicsBuffer_drawPixel(y0, x0, color);
		}
		else {
			GraphicsBuffer_drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}
static inline void GraphicsBuffer_drawCircle(s16 x0, s16 y0, s16 radius, u16 color) {
    s32 x = radius, y = 0;
    s32 radiusError = 1 - x;
    while (x >= y) {
        GraphicsBuffer_drawPixel(x + x0, y + y0, color);
        GraphicsBuffer_drawPixel(y + x0, x + y0, color);
        GraphicsBuffer_drawPixel(-x + x0, y + y0, color);
        GraphicsBuffer_drawPixel(-y + x0, x + y0, color);
        GraphicsBuffer_drawPixel(-x + x0, -y + y0, color);
        GraphicsBuffer_drawPixel(-y + x0, -x + y0, color);
        GraphicsBuffer_drawPixel(x + x0, -y + y0, color);
        GraphicsBuffer_drawPixel(y + x0, -x + y0, color);
        radiusError += 2 * (radiusError < 0 ? y++ : (y++ - x--)) + 1;
    }
}
static inline void GraphicsBuffer_setFont(FONT_INFO fontInfo) {
    graphics_buffer.fontHeight = fontInfo.height;
    graphics_buffer.fontFirstCh = fontInfo.startCh;
    graphics_buffer.fontLastCh = fontInfo.endCh;
    graphics_buffer.fontDescriptor = fontInfo.charDesc;
    graphics_buffer.fontBitmap = fontInfo.bitmap;
}
static inline void GraphicsBuffer_setCursor(u16 x, u16 y) {
    graphics_buffer.cursorX = x;
    graphics_buffer.cursorY = y;
}
static inline void GraphicsBuffer_fontColor(u16 f, u16 g) {
    graphics_buffer.fontBGcolor = g;
    graphics_buffer.fontColor = f;
}
static inline u8 GraphicsBuffer_getFontHeight() {
    return graphics_buffer.fontHeight;
}
static inline u8 GraphicsBuffer_getFontHeight2(FONT_INFO fontInfo) {
    return fontInfo.height;
}
static inline u8 GraphicsBuffer_getPrintWidth(char* st) {
    if (!graphics_buffer.fontFirstCh)
        return 0;
    u8 totalWidth = 0, amtCh = strlen(st);
    for (u8 i = 0; i != amtCh; ++i) {
        totalWidth += graphics_buffer.fontDescriptor[st[i] - graphics_buffer.fontFirstCh].width + 1;
    }
    return totalWidth;
}
static inline size_t GraphicsBuffer_write(u8 ch) {
    if (!graphics_buffer.fontFirstCh)
        return 1;
    if (ch < graphics_buffer.fontFirstCh || ch > graphics_buffer.fontLastCh)
        return 1;
    if (graphics_buffer.cursorX > graphics_buffer.xMax || graphics_buffer.cursorY > graphics_buffer.yMax)
        return 1;

    u8 chWidth = graphics_buffer.fontDescriptor[ch - graphics_buffer.fontFirstCh].width;
    u8 chHeight = graphics_buffer.fontHeight;
    u8 ySkip = 0;

    if (graphics_buffer.cursorY < 0) {
        if (graphics_buffer.cursorY + chHeight < 0)
            return 1;
        ySkip = -graphics_buffer.cursorY;
    }

    u8 xSkip = 0;
    if (graphics_buffer.cursorX < 0) {
        if (graphics_buffer.cursorX + chWidth < 0) {
            graphics_buffer.cursorX += (s32)(chWidth + 1);
            return 1;
        }
        xSkip = -graphics_buffer.cursorX;
    }

    u8 bytesPerRow = chWidth / 8;
    if (chWidth > bytesPerRow * 8)
        bytesPerRow++;

    u16 offset = graphics_buffer.fontDescriptor[ch - graphics_buffer.fontFirstCh].offset + (bytesPerRow * chHeight) - 1;

    GraphicsBuffer_setX(graphics_buffer.cursorX + xSkip, graphics_buffer.cursorX + chWidth);
    GraphicsBuffer_setY(graphics_buffer.cursorY + ySkip, graphics_buffer.cursorY + chHeight);

    graphics_buffer.cursorY -= ySkip;
    graphics_buffer.cursorX += (s32)((chWidth + 1) - xSkip);

    for (u8 y = ySkip; y < chHeight && ((y + graphics_buffer.cursorY) < graphics_buffer.yMax); ++y) {
        for (u8 byte = 0; byte < bytesPerRow; byte++) {
            u8 data = graphics_buffer.fontBitmap[offset - y - ((bytesPerRow - byte - 1) * chHeight)];
            u8 bits = byte * 8;
            for (u8 i = xSkip; i < 8 && (bits + i) < chWidth && (bits + i + graphics_buffer.cursorX) < (graphics_buffer.xMax + 1); i++) {
                if (data & (0x80 >> i)) {
                    GraphicsBuffer_writePixel(graphics_buffer.fontColor);
                } else {
                    if (graphics_buffer.fontBGcolor == ALPHA_COLOR) {
                        GraphicsBuffer_incrementPixel();
                    } else {
                        GraphicsBuffer_writePixel(graphics_buffer.fontBGcolor);
                    }
                }
            }
        }
        if ((graphics_buffer.cursorX + chWidth) <= graphics_buffer.xMax) {
            if (graphics_buffer.fontBGcolor == ALPHA_COLOR) {
                GraphicsBuffer_incrementPixel();
            } else {
                GraphicsBuffer_writePixel(graphics_buffer.fontBGcolor);
            }
        }
    }
    return 1;
}
static inline size_t GraphicsBuffer_print(const char* str) {
    size_t lenstr = strlen(str);
    for (size_t i = 0; i != lenstr; ++i) {
        GraphicsBuffer_write((u8)str[i]);
    }
    return lenstr;
}
static inline size_t GraphicsBuffer_printf(const char* format, ...) {
    char buffer[20];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    size_t n = GraphicsBuffer_print(buffer);
    va_end(args);
    return n;
}
// Thumby specific code
#define Thumby_initGrahpics() GraphicsBuffer_init(THUMBY_SCREEN_WIDTH, THUMBY_SCREEN_HEIGHT);
#define Thumby_setX GraphicsBuffer_setX
#define Thumby_setY GraphicsBuffer_setY
#define Thumby_goTo GraphicsBuffer_goTo
#define Thumby_drawPixel GraphicsBuffer_drawPixel
#define Thumby_incrementPixel GraphicsBuffer_incrementPixel
#define Thumby_writePixel GraphicsBuffer_writePixel
#define Thumby_clear GraphicsBuffer_clear
#define Thumby_drawLine GraphicsBuffer_drawLine
#define Thumby_drawCircle GraphicsBuffer_drawCircle
#define Thumby_setCursor GraphicsBuffer_setCursor
#define Thumby_setFont GraphicsBuffer_setFont
#define Thumby_fontColor GraphicsBuffer_fontColor
#define Thumby_getFontHeight GraphicsBuffer_getFontHeight
#define Thumby_getFontHeight2 GraphicsBuffer_getFontHeight2
#define Thumby_getPrintWidth GraphicsBuffer_getPrintWidth
#define Thumby_write GraphicsBuffer_write
// This will need to be rewritten and a wrapper will be needed, as I'd like int support in the future
#define Thumby_print GraphicsBuffer_print
#define Thumby_printf GraphicsBuffer_printf
#define Thumby_getBuffer() display_instance.buffer
#define Thumby_getBufferSize() display_instance.bufsize
static inline void Thumby_begin() {
    // Set sysclock to 125MHz, as per the Thumby Arduino spec
    set_sys_clock_khz(125000, true);
    // Setup link link pins
    pinMode(THUMBY_LINK_TX_PIN, OUTPUT);
    pinMode(THUMBY_LINK_RX_PIN, INPUT);
    pinMode(THUMBY_LINK_PU_PIN, OUTPUT);
    digitalWrite(THUMBY_LINK_PU_PIN, HIGH);
    // Setup button pins
    pinMode(THUMBY_BTN_LDPAD_PIN, INPUT_PULLUP);
    pinMode(THUMBY_BTN_RDPAD_PIN, INPUT_PULLUP);
    pinMode(THUMBY_BTN_UDPAD_PIN, INPUT_PULLUP);
    pinMode(THUMBY_BTN_DDPAD_PIN, INPUT_PULLUP);
    pinMode(THUMBY_BTN_B_PIN, INPUT_PULLUP);
    pinMode(THUMBY_BTN_A_PIN, INPUT_PULLUP);
    // Set audio pin as PWM
    gpio_set_function(THUMBY_AUDIO_PIN, GPIO_FUNC_PWM);
    // Setup UART (half-duplex)
    Serial1_begin(115200);
    gpio_set_function(THUMBY_LINK_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(THUMBY_LINK_RX_PIN, GPIO_FUNC_UART);
    // Setup display pins
    ssd1306_init(&display_instance, THUMBY_SDA_PIN, THUMBY_SCK_PIN, THUMBY_CS_PIN, THUMBY_DC_PIN, THUMBY_RESET_PIN, THUMBY_SCREEN_WIDTH, THUMBY_SCREEN_HEIGHT, spi0);
    // Init graphics
    Thumby_initGrahpics();
    Thumby_setFont(thinPixel7_10ptFontInfo);
}
// I'd rather *not* support this, but I have a very strong feeling someone used it.
static inline void Thumby_sendCommand(u8 command) {
    ssd1306_write(&display_instance, command, false);
}
static inline void Thumby_writeBuffer(u8* buffer, int bufferLength) {
    ssd1306_transfer_buffer(&display_instance, buffer, bufferLength);
}
static inline void Thumby_setBrightness(u8 brightness) {
    if (brightness > 127) {
        brightness = 127;
    }
    ssd1306_contrast(&display_instance, brightness);
}
// The first Thumby rev has a I2C display.
static inline u8 Thumby_getHWID() {
    u8 HWID = 0;
    pinMode(THUMBY_ID_PIN, INPUT_PULLUP);
    if (digitalRead(THUMBY_ID_PIN) == 0) {
        HWID += 1;
    }
    pinMode(THUMBY_ID_PIN, INPUT_PULLDOWN);
    pinMode(THUMBY_ID_PIN2, INPUT_PULLUP);
    if (digitalRead(THUMBY_ID_PIN2) == 0) {
        HWID += 2;
    }
    pinMode(THUMBY_ID_PIN, INPUT_PULLDOWN);
    return HWID;
}
static inline bool Thumby_isPressed(u8 mask) {
    if (mask & (1 << 0) && !digitalRead(THUMBY_BTN_LDPAD_PIN)) {
      return true;
    }
    else if (mask & (1 << 1) && !digitalRead(THUMBY_BTN_RDPAD_PIN)) {
      return true;
    }
    else if (mask & (1 << 2) && !digitalRead(THUMBY_BTN_UDPAD_PIN)) {
      return true;
    }
    else if (mask & (1 << 3) && !digitalRead(THUMBY_BTN_DDPAD_PIN)) {
      return true;
    }
    else if (mask & (1 << 4) && !digitalRead(THUMBY_BTN_B_PIN)) {
      return true;
    }
    else if (mask & (1 << 5) && !digitalRead(THUMBY_BTN_A_PIN)) {
      return true;
    }
    return false;
}
// Pack dataBuf into packedBuf (adds 2 size bytes, 
// 1 checksum, and returns false if size too large to
// fit in packet, or too large to fit in packedBuf)
static inline s8 Thumby_linkPack(u8* dataBuf, u16 dataBufLen, u8* packedBuf, u16 packedBufLen) {
    u16 packetLength = dataBufLen + 3;
    // Check that the data length can be indexed by two bytes and
    // that it will fit into the packed buffer with header bytes
    if (dataBufLen > 512 || packetLength > packedBufLen) {
        return -1;
    }
    // Prepare packet header
    packedBuf[0] = (dataBufLen >> 8) & 0xFF;
    packedBuf[1] = dataBufLen & 0xFF;
    packedBuf[2] = 0;
    // Generate checksum and copy data
    for (u16 b = 0; b != dataBufLen; ++b) {
        packedBuf[2] ^= dataBuf[b];
        packedBuf[b + 3] = dataBuf[b];
    }
    return packetLength;
}
// Unpack packedBuf to dataBuf (removes 1 checksum byte, and 2 size 
// bytes but returns false if checksum or size check fails, or if
// stripped packedBuf data cannot fit in dataBuf)
static inline s8 Thumby_linkUnpack(u8* packedBuf, u16 packedBufLen, u8* dataBuf, u16 dataBufLen) {
    u16 dataLength = (packedBuf[0] << 8) + packedBuf[1];
    // Check that the data length can be indexed by two bytes and
    // the received data length is the same as the actual received
    // packet length minus the 3 header bytes
    if ((packedBufLen - 3) > dataBufLen || dataLength != (packedBufLen - 3)) {
        return -1;
    }
    // Copy data and generate checksum off received data
    u8 checksum = 0;
    for (u16 b = 0; b != dataLength; ++b) {
        dataBuf[b] = packedBuf[b + 3];
        checksum ^= dataBuf[b];
    }
    // Return false if received and generated checksums are not the same
    if (checksum != packedBuf[2]) {
        return -1;
    }
    return dataLength;
}
// Start playing a sound through the buzzer using pwm (does not block)
static inline void Thumby_play2(u32 freq, u16 duty) {
    u32 wrap = clock_get_hz(clk_sys) / freq - 1;
    u32 level = (u32)(wrap * (duty / 65535.f));
    u8 slice_num = pwm_gpio_to_slice_num(THUMBY_AUDIO_PIN);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    // Set the PWM not running
    pwm_set_enabled(slice_num, true);
}
static inline void Thumby_play(u32 freq) {
    Thumby_play2(freq, 32768);
}
static inline void Thumby_stopPlay() {
    u8 slice_num = pwm_gpio_to_slice_num(THUMBY_AUDIO_PIN);
    // Set the PWM not running
    pwm_set_enabled(slice_num, false);
}
// Entry
#ifdef __cplusplus
};
#endif
extern void setup();
extern void loop();
int main() {
    setup();
    while (1) {      
        if (Thumby_getHWID() <= 1) {
            printf("Unsupported Thumby display!\n");
        }
        else {
            loop();
        }
    }
    return 0;
}