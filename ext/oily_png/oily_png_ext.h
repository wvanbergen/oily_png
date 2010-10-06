#ifndef OILY_PNG_EXT
#define OILY_PNG_EXT

#include "ruby.h"

#include "png_decoding.h"
#include "png_encoding.h"


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
#define PIXEL unsigned int  // Pixels use 32 bits unsigned integers
#define BYTE  unsigned char // Bytes use 8 bits unsigned integers

#define R_BYTE(pixel)  ((BYTE) ((pixel & (PIXEL) 0xff000000) >> 24))
#define G_BYTE(pixel)  ((BYTE) ((pixel & (PIXEL) 0x00ff0000) >> 16))
#define B_BYTE(pixel)  ((BYTE) ((pixel & (PIXEL) 0x0000ff00) >> 8))
#define A_BYTE(pixel)  ((BYTE) ((pixel & (PIXEL) 0x000000ff)))


/*
  Initialize the extension by creating the OilyPNG modules, and registering
  the encoding and decoding replacement functions.
  
  Note, this does not actually replace functionality in ChunkyPNG; you will need
  to extend the ChunkyPNG::Canvas class with the OilyPNG::PNGDecoding module to 
  speed up decoding, and include OilyPNG::PNGEncoding into the same class to speed
  up encoding. This is done in lib/oily_png.rb
*/
void Init_oily_png_ext();

/*
  Returns the number of bytes per pixel for a given color mode.
*/
int oily_png_pixel_size(int color_mode);

#endif
