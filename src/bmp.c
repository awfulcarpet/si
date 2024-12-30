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
		bmp->data[idx++] = buf[i] << 16 | buf[i + 1] << 8 | buf[i + 2];
		printf("%02x%02x%02x ", buf[i], buf[i+1], buf[i+2]);

		if (++width % bmp->width == 0) {
			i += bmp->width % 4;
			printf("\n");
		}
	}
	printf("\n");
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
	printf("%d\n", bmp->size);
	printf("%02x\n", bmp->offset);
	printf("%d %d\n", bmp->width, bmp->height);
	read_pixels(f, bmp);
	for (uint32_t i = 0; i < bmp->width * bmp->height; i++) {
		printf("%06x ", bmp->data[i]);
		if ((i + 1)% 5 == 0)
			printf("\n");
	}


	struct Image *img = NULL;
	/*img->data = calloc(bmp->width * bmp->height, sizeof(uint32_t));*/
	/*img->height = bmp->height;*/
	/*img->width = bmp->width;*/

	fclose(f);
	return img;
}
