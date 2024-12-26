#include <assert.h>
#include <png.h>
#include <pngconf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "img.h"

int
is_png(FILE *f) {
	assert(f);

	unsigned char header[9];
	fread(header, 1, 8, f);

	return !png_sig_cmp(header, 0, 8);
}

struct Image *
read_png(char *filename) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL) {
		fprintf(stderr, "unable to open file: %s\n", filename);
		return NULL;
	}

	assert(is_png(f));

	struct Image *img = calloc(1, sizeof(struct Image));

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	assert(png);

	png_infop info = png_create_info_struct(png);
	assert(info);

	png_init_io(png, f);
	png_set_sig_bytes(png, 8);


	png_read_png(png, info, PNG_TRANSFORM_STRIP_ALPHA, NULL);
	img->width = png_get_image_width(png, info);
	img->height = png_get_image_height(png, info);
	img->data = calloc(img->width * img->height, sizeof(uint32_t));

	png_bytepp rows = png_get_rows(png, info);
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			img->data[i * img->width + j] = rows[i][3 * j] << 16 | rows[i][3 * j + 1] << 8 | rows[i][3 * j + 2];
		}
	}
	fclose(f);

	return img;
}
