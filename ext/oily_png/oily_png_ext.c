#include "oily_png_ext.h"

void Init_oily_png() {
  VALUE OilyPNG = rb_define_module("OilyPNG");

  // Setup decoding module
  VALUE OilyPNG_PNGDecoding = rb_define_module_under(OilyPNG, "PNGDecoding");
  rb_define_method(OilyPNG_PNGDecoding, "decode_png_image_pass", oily_png_decode_png_image_pass, 6);
  
  // Setup encoding module
  VALUE OilyPNG_PNGEncoding = rb_define_module_under(OilyPNG, "PNGEncoding");
  rb_define_method(OilyPNG_PNGEncoding, "encode_png_image_pass_to_stream", oily_png_encode_png_image_pass_to_stream, 4);
  
  // Setup Color module
  VALUE OilyPNG_Color = rb_define_module_under(OilyPNG, "Color");
  rb_define_method(OilyPNG_Color, "compose_quick", oily_png_color_compose_quick, 2);
}

char oily_png_samples_per_pixel(char color_mode) {
  switch (color_mode) {
    case OILY_PNG_COLOR_GRAYSCALE:       return 1;
    case OILY_PNG_COLOR_TRUECOLOR:       return 3;
    case OILY_PNG_COLOR_INDEXED:         return 1;
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA: return 2;
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA: return 4;
    default: rb_raise(rb_eRuntimeError, "Unsupported color mode: %d", color_mode);
  }
}

char oily_png_pixel_bitsize(char color_mode, char bit_depth) {
  return oily_png_samples_per_pixel(color_mode) * bit_depth;
}

char oily_png_pixel_bytesize(char color_mode, char bit_depth) {
  return (bit_depth < 8) ? 1 : (oily_png_pixel_bitsize(color_mode, bit_depth) + 7) >> 3;
}

long oily_png_scanline_bytesize(char color_mode, char bit_depth, long width) {
  return (8 + ((oily_png_pixel_bitsize(color_mode, bit_depth) * width) + 7)) >> 3;
}

long oily_png_pass_bytesize(char color_mode, char bit_depth, long width, long height) {
  return (width == 0 || height == 0) ? 0 : (oily_png_scanline_bytesize(color_mode, bit_depth, width)) * height;
}
