#include "oily_png_ext.h"

PIXEL oily_png_encode_get_pixel(VALUE self, long index) {
  VALUE pixels = rb_funcall(self, rb_intern("pixels"), 0);
  return NUM2UINT(rb_ary_entry(pixels, index));
}

void oily_png_encode_pixel(PIXEL pixel, int color_mode, BYTE* bytes, int pos, VALUE palette) {
  switch (color_mode) {
    case OILY_PNG_COLOR_GRAYSCALE:
      bytes[pos] = (BYTE) ((pixel & (PIXEL) 0xff000000) >> 24);
      break;
    case OILY_PNG_COLOR_TRUECOLOR:
      bytes[pos + 0] = (BYTE) ((pixel & (PIXEL) 0xff000000) >> 24);
      bytes[pos + 1] = (BYTE) ((pixel & (PIXEL) 0x00ff0000) >> 16);
      bytes[pos + 2] = (BYTE) ((pixel & (PIXEL) 0x0000ff00) >>  8);
      break;
    case OILY_PNG_COLOR_INDEXED:
      bytes[pos] = (BYTE) NUM2UINT(rb_funcall(palette, rb_intern("index"), 1, UINT2NUM(pixel)));
      break;
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA:
      bytes[pos + 0] = (BYTE) ((pixel & (PIXEL) 0xff000000) >> 24);
      bytes[pos + 1] = (BYTE) ((pixel & (PIXEL) 0x000000ff));
      break;
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA:
      bytes[pos + 0] = (BYTE) ((pixel & (PIXEL) 0xff000000) >> 24);
      bytes[pos + 1] = (BYTE) ((pixel & (PIXEL) 0x00ff0000) >> 16);
      bytes[pos + 2] = (BYTE) ((pixel & (PIXEL) 0x0000ff00) >>  8);
      bytes[pos + 3] = (BYTE) ((pixel & (PIXEL) 0x000000ff));
      break;
    default: 
      rb_raise(rb_eRuntimeError, "Unsupported color mode: %d", color_mode);
  }
}

void oily_png_encode_filter_sub(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x;
  for (x = line_size - 1; x > pixel_size; x--) {
    bytes[pos + x] -= bytes[pos + x - pixel_size];
  }
}

void oily_png_encode_filter_up(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x;
  if (pos >= line_size) {
    for (x = line_size - 1; x > 0; x--) {
      bytes[pos + x] -= bytes[pos + x - line_size];
    }
  }
}

void oily_png_encode_filter_average(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x; BYTE a, b;
  for (x = line_size - 1; x > 0; x--) {
    a = (x > pixel_size)   ? bytes[pos + x - pixel_size] : 0;
    b = (pos >= line_size) ? bytes[pos + x - line_size]  : 0;
    bytes[pos + x] -= ((a + b) >> 1);
  }
}

void oily_png_encode_filter_paeth(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x, p, pa, pb, pc; BYTE a, b, c, pr;
  for (x = line_size - 1; x > 0; x--) {
    a = (x > pixel_size) ? bytes[pos + x - pixel_size] : 0;
    b = (pos >= line_size) ? bytes[pos + x - line_size] : 0;
    c = (pos >= line_size && x > pixel_size) ? bytes[pos + x - line_size - pixel_size] : 0;
    p  = a + b - c;
    pa = abs(p - a);
    pb = abs(p - b);
    pc = abs(p - c);
    pr = (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c);
    bytes[pos + x] -= pr;
  }
  
}

VALUE oily_png_encode_png_image_pass_to_stream(VALUE self, VALUE stream, VALUE color_mode, VALUE filtering) {
  
  // Get the data
  int width      = FIX2INT(rb_funcall(self, rb_intern("width"), 0));
  int height     = FIX2INT(rb_funcall(self, rb_intern("height"), 0));
  VALUE pixels   = rb_funcall(self, rb_intern("pixels"), 0);
  
  if (RARRAY_LEN(pixels) != width * height) {
    rb_raise(rb_eRuntimeError, "The number of pixels does not match the canvas dimensions.");
  }
  
  VALUE palette = Qnil;
  if (FIX2INT(color_mode) == OILY_PNG_COLOR_INDEXED) {
    palette = rb_funcall(self, rb_intern("encoding_palette"), 0);
  }
  
  int pixel_size = oily_png_pixel_size(FIX2INT(color_mode));
  int line_size = 1 + pixel_size * width;
  int pass_size = line_size * height;

  // Allocate memory for the byte array.
  BYTE* bytes = ALLOCA_N(BYTE, pass_size);

  PIXEL pixel;
  int x, y, pos;
  for (y = 0; y < height; y++) {
    bytes[line_size * y] = FIX2INT(filtering);
    
    for (x = 0; x < width; x++) {
      pixel = NUM2UINT(rb_ary_entry(pixels, y * height + x));
      pos   = (line_size * y) + (pixel_size * x) + 1;
      oily_png_encode_pixel(pixel, FIX2INT(color_mode), bytes, pos, palette);
    }
  }
  
  if (FIX2INT(filtering) != OILY_PNG_FILTER_NONE) {
    for (y = height - 1; y >= 0; y--) {
      switch (FIX2INT(filtering)) {
        case OILY_PNG_FILTER_SUB:     oily_png_encode_filter_sub(     bytes, line_size * y, line_size, pixel_size); break;
        case OILY_PNG_FILTER_UP:      oily_png_encode_filter_up(      bytes, line_size * y, line_size, pixel_size); break;
        case OILY_PNG_FILTER_AVERAGE: oily_png_encode_filter_average( bytes, line_size * y, line_size, pixel_size); break;
        case OILY_PNG_FILTER_PAETH:   oily_png_encode_filter_paeth(   bytes, line_size * y, line_size, pixel_size); break;
        default: rb_raise(rb_eRuntimeError, "Unsupported filter type: %d", FIX2INT(filtering));
      }
    }
  }
  
  rb_str_cat(stream, bytes, pass_size);
  return Qnil;
}
