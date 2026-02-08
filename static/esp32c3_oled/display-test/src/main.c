/*
 * Bouncing ball on SSD1306 72x40 OLED using raw I2C.
 * Bypasses Zephyr display driver — proven reliable on ESP32-C3.
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(display_test, LOG_LEVEL_INF);

#define I2C_NODE       DT_NODELABEL(i2c0)
#define SSD1306_ADDR   0x3C

#define SCREEN_W       72
#define SCREEN_H       40
#define X_OFF          30  /* (132 - 72) / 2 */
#define Y_OFF          12  /* (64 - 40) / 2 */
#define PAGES          8   /* 64 / 8 */

#define BALL_RADIUS    4
#define FRAME_DELAY_MS 30

static const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
static uint8_t fb[SCREEN_W * PAGES];

/* Send command bytes: prepend 0x00 control byte, single i2c_write */
static int ssd1306_cmd(const uint8_t *cmd, size_t len)
{
	uint8_t buf[28]; /* 1 ctrl + up to 27 cmd bytes */

	if (len + 1 > sizeof(buf)) {
		return -ENOMEM;
	}
	buf[0] = 0x00;
	memcpy(&buf[1], cmd, len);
	return i2c_write(i2c_dev, buf, len + 1, SSD1306_ADDR);
}

/* U8g2-matching init sequence */
static void ssd1306_init_display(void)
{
	static const uint8_t init[] = {
		0xAE,       /* Display OFF */
		0x2E,       /* Deactivate scroll */
		0xD5, 0x80, /* Clock div ratio / osc freq */
		0xA8, 0x3F, /* MUX ratio = 63 (all 64 COM lines) */
		0xD3, 0x00, /* Display offset = 0 */
		0x40,       /* Start line = 0 */
		0x8D, 0x14, /* Charge pump ON */
		0x20, 0x02, /* Page addressing mode */
		0xA1,       /* Segment remap */
		0xC8,       /* COM scan remapped */
		0xDA, 0x12, /* COM pins: alternative, no LR remap */
		0x81, 0xCF, /* Contrast */
		0xD9, 0xF1, /* Pre-charge period */
		0xDB, 0x20, /* VCOMH deselect */
		0xA4,       /* Display follows RAM */
		0xA6,       /* Normal (not inverted) */
		0xAF,       /* Display ON */
	};

	ssd1306_cmd(init, sizeof(init));
}

/* Write one page (72 bytes) via raw I2C — single message, ≤73 bytes */
static void flush_display(void)
{
	uint8_t buf[1 + SCREEN_W]; /* 0x40 ctrl + 72 data */

	buf[0] = 0x40;

	for (int page = 0; page < PAGES; page++) {
		uint8_t addr[] = {
			(uint8_t)(0xB0 | page),
			(uint8_t)(0x00 | (X_OFF & 0x0F)),
			(uint8_t)(0x10 | ((X_OFF >> 4) & 0x0F)),
		};
		ssd1306_cmd(addr, sizeof(addr));

		memcpy(&buf[1], &fb[page * SCREEN_W], SCREEN_W);
		i2c_write(i2c_dev, buf, sizeof(buf), SSD1306_ADDR);
	}
}

static inline void set_pixel(int x, int y)
{
	if (x >= 0 && x < SCREEN_W && y >= 0 && y < PAGES * 8) {
		fb[(y / 8) * SCREEN_W + x] |= 1 << (y & 7);
	}
}

static void draw_rect(int x0, int y0, int x1, int y1)
{
	for (int x = x0; x <= x1; x++) {
		set_pixel(x, y0);
		set_pixel(x, y1);
	}
	for (int y = y0; y <= y1; y++) {
		set_pixel(x0, y);
		set_pixel(x1, y);
	}
}

static void draw_ball(int cx, int cy, int r)
{
	for (int dy = -r; dy <= r; dy++) {
		for (int dx = -r; dx <= r; dx++) {
			if (dx * dx + dy * dy <= r * r) {
				set_pixel(cx + dx, cy + dy);
			}
		}
	}
}

int main(void)
{
	if (!device_is_ready(i2c_dev)) {
		LOG_ERR("I2C not ready");
		return -1;
	}

	ssd1306_init_display();

	memset(fb, 0, sizeof(fb));
	flush_display();
	LOG_INF("Display ready");

	int16_t bx = SCREEN_W / 2;
	int16_t by = SCREEN_H / 2;
	int16_t dx = 1, dy = 1;

	while (1) {
		memset(fb, 0, sizeof(fb));

		draw_rect(0, Y_OFF, SCREEN_W - 1, Y_OFF + SCREEN_H - 1);
		draw_ball(bx, Y_OFF + by, BALL_RADIUS);

		flush_display();

		bx += dx;
		by += dy;
		if (bx - BALL_RADIUS <= 1 || bx + BALL_RADIUS >= SCREEN_W - 2) {
			dx = -dx;
			bx += dx;
		}
		if (by - BALL_RADIUS <= 1 || by + BALL_RADIUS >= SCREEN_H - 2) {
			dy = -dy;
			by += dy;
		}

		k_msleep(FRAME_DELAY_MS);
	}
	return 0;
}
