#include "oily_png_ext.h"

void Init_oily_png_ext() {
  VALUE OilyPNG = rb_define_module("OilyPNG");

  // Setup decoding
  VALUE OilyPNG_PNGDecoding = rb_define_module_under(OilyPNG, "PNGDecoding");
  rb_define_method(OilyPNG_PNGDecoding, "decode_png_image_pass", oily_png_decode_png_image_pass, 5);
  
  // Setup encoding
  VALUE OilyPNG_PNGEncoding = rb_define_module_under(OilyPNG, "PNGEncoding");
  rb_define_method(OilyPNG_PNGEncoding, "encode_png_image_pass_to_stream", oily_png_encode_png_image_pass_to_stream, 3);
}

int oily_png_pixel_size(int color_mode) {
  switch (color_mode) {
    case OILY_PNG_COLOR_GRAYSCALE:       return 1;
    case OILY_PNG_COLOR_TRUECOLOR:       return 3;
    case OILY_PNG_COLOR_INDEXED:         return 1;
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA: return 2;
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA: return 4;
    default: rb_raise(rb_eRuntimeError, "Unsupported color mode: %d", color_mode);
  }
}
