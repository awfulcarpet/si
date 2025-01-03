#include <X11/Xlib.h>
#include <stdint.h>

struct Image {
	int width;
	int height;
	uint32_t *data;
};

XImage * img_to_ximg(struct Image *img);
struct Image * read_png(char *filename);
struct Image * read_bmp(char *filename);
