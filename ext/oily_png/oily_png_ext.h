#ifndef OILY_PNG_EXT
#define OILY_PNG_EXT

#include "ruby.h"

// PNG color mode constants
#define OILY_PNG_COLOR_GRAYSCALE        0
#define OILY_PNG_COLOR_TRUECOLOR        2
#define OILY_PNG_COLOR_INDEXED          3
#define OILY_PNG_COLOR_GRAYSCALE_ALPHA  4
#define OILY_PNG_COLOR_TRUECOLOR_ALPHA  6

// PNG filter constants
#define OILY_PNG_FILTER_NONE    0
#define OILY_PNG_FILTER_SUB     1
#define OILY_PNG_FILTER_UP      2
#define OILY_PNG_FILTER_AVERAGE 3
#define OILY_PNG_FILTER_PAETH   4

// Type definitions
#define PIXEL unsigned int

#include "png_decoding.h"
#include "png_encoding.h"

void Init_oily_png_ext();
int oily_png_pixel_size(color_mode);

#endif