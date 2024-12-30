#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "img.h"

struct BMP {
	uint32_t size;
	uint32_t offset;

	uint32_t width;
	uint32_t height;

	uint32_t *data;
};

static int
is_bmp(uint8_t header[14])
{
	return header[0] == 'B' && header[1] == 'M';
}

struct BMP *
parse_header(FILE *f)
{
	uint8_t header[14] = {0};
	fread(header, 1, 14, f);
	assert(is_bmp(header));

	struct BMP *bmp = calloc(1, sizeof(struct BMP));


	for (int i = 0; i < 4; i++) {
		bmp->size |= header[i + 2] << (8 * i);
	}

	for (int i = 0; i < 4; i++) {
		bmp->offset |= header[i + 0x0A] << (8 * i);
	}

	bmp->size -= bmp->offset; /* len of pixel data */

	uint8_t info_header[40] = {0};
	fread(info_header, 1, 40, f);

	/* 40 len info header */
	assert(info_header[0] == 0x28);

	for (int i = 0; i < 4; i++) {
		bmp->width |= info_header[4 + i] << (8 * i);
	}

	for (int i = 0; i < 4; i++) {
		bmp->height |= info_header[8 + i] << (8 * i);
	}

	/* no compression */
	assert(info_header[16] == 0);

	return bmp;
}

void
read_pixels(FILE *f, struct BMP *bmp)
{
	fseek(f, bmp->offset, SEEK_SET);
	uint8_t *buf = calloc(bmp->size, sizeof(uint8_t));
	fread(buf, 1, bmp->size, f);
	bmp->data = calloc(bmp->width * bmp->height, sizeof(uint32_t));

	uint32_t width = 0;
	uint32_t idx = 0;
	for (uint32_t i = 0; i < bmp->size; i += 3) {
		bmp->data[(4 - idx / bmp->width) * bmp->width + idx % bmp->width] = buf[i + 2] << 16 | buf[i + 1] << 8 | buf[i];
		idx++;

		if (++width % bmp->width == 0) {
			i += bmp->width % 4;
		}
	}
}

struct Image *
read_bmp(char *filename)
{
	FILE *f = fopen(filename, "rb");

	if (f == NULL) {
		fprintf(stderr, "unable to open file: %s\n", filename);
		exit(1);
	}

	struct BMP *bmp = parse_header(f);
	read_pixels(f, bmp);


	struct Image *img = calloc(1, sizeof(struct Image));
	img->data = bmp->data;
	img->height = bmp->height;
	img->width = bmp->width;

	fclose(f);
	return img;
}
