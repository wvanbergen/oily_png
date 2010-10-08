#include "oily_png_ext.h"

///// Pixel encoding functions //////////////////////////////////////////

void oily_png_encode_pixel_grayscale(PIXEL pixel, BYTE* bytes, int pos, VALUE palette) {
  bytes[pos] = R_BYTE(pixel);
}

void oily_png_encode_pixel_grayscale_alpha(PIXEL pixel, BYTE* bytes, int pos, VALUE palette) {
  bytes[pos + 0] = R_BYTE(pixel);
  bytes[pos + 1] = A_BYTE(pixel);
}

void oily_png_encode_pixel_truecolor(PIXEL pixel, BYTE* bytes, int pos, VALUE palette) {
  bytes[pos + 0] = R_BYTE(pixel);
  bytes[pos + 1] = G_BYTE(pixel);
  bytes[pos + 2] = B_BYTE(pixel);
}

void oily_png_encode_pixel_truecolor_alpha(PIXEL pixel, BYTE* bytes, int pos, VALUE palette) {
  bytes[pos + 0] = R_BYTE(pixel);
  bytes[pos + 1] = G_BYTE(pixel);
  bytes[pos + 2] = B_BYTE(pixel);
  bytes[pos + 3] = A_BYTE(pixel);
}

void oily_png_encode_pixel_indexed(PIXEL pixel, BYTE* bytes, int pos, VALUE palette) {
  bytes[pos] = (BYTE) NUM2UINT(rb_funcall(palette, rb_intern("index"), 1, UINT2NUM(pixel)));
}

///// Scanline filtering functions //////////////////////////////////////////

void oily_png_encode_filter_sub(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x;
  for (x = line_size - 1; x > pixel_size; x--) {
    FILTER_BYTE(bytes[pos + x], bytes[pos + x - pixel_size]);
  }
}

void oily_png_encode_filter_up(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x;
  if (pos >= line_size) {
    for (x = line_size - 1; x > 0; x--) {
      FILTER_BYTE(bytes[pos + x], bytes[pos + x - line_size]);
    }
  }
}

void oily_png_encode_filter_average(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int x; BYTE a, b;
  for (x = line_size - 1; x > 0; x--) {
    a = (x > pixel_size)   ? bytes[pos + x - pixel_size] : 0;
    b = (pos >= line_size) ? bytes[pos + x - line_size]  : 0;
    FILTER_BYTE(bytes[pos + x], (a + b) >> 1);
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
    FILTER_BYTE(bytes[pos + x], pr);
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

  // Get the encoding palette if we're encoding to an indexed bytestream.
  VALUE palette = Qnil;
  if (FIX2INT(color_mode) == OILY_PNG_COLOR_INDEXED) {
    palette = rb_funcall(self, rb_intern("encoding_palette"), 0);
  }
  
  int pixel_size = oily_png_pixel_size(FIX2INT(color_mode));
  int line_size = 1 + pixel_size * width;
  int pass_size = line_size * height;

  // Allocate memory for the byte array.
  BYTE* bytes = ALLOCA_N(BYTE, pass_size);

  // Select out pixel encoder function based on the color mode.
  void (*pixel_encoder)(PIXEL, BYTE*, int, VALUE) = NULL;
  switch (FIX2INT(color_mode)) {
    case OILY_PNG_COLOR_GRAYSCALE:       pixel_encoder = &oily_png_encode_pixel_grayscale; break;
    case OILY_PNG_COLOR_TRUECOLOR:       pixel_encoder = &oily_png_encode_pixel_truecolor; break;
    case OILY_PNG_COLOR_INDEXED:         pixel_encoder = &oily_png_encode_pixel_indexed; break;
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA: pixel_encoder = &oily_png_encode_pixel_grayscale_alpha; break;
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA: pixel_encoder = &oily_png_encode_pixel_truecolor_alpha; break;
    default: rb_raise(rb_eRuntimeError, "Unsupported color mode: %d", color_mode);
  }

  // Loop over all the pixels to encode them into the byte array.
  PIXEL pixel;
  int x, y, pos;
  for (y = 0; y < height; y++) {
    bytes[line_size * y] = (BYTE) FIX2INT(filtering);
    
    for (x = 0; x < width; x++) {
      pixel = NUM2UINT(rb_ary_entry(pixels, y * width + x));
      pos   = (line_size * y) + (pixel_size * x) + 1;
      pixel_encoder(pixel, bytes, pos, palette);
    }
  }
  
  // Check if we are going to apply any filtering
  if (FIX2INT(filtering) != OILY_PNG_FILTER_NONE) {
    
    // Assign the chosen filter function to the scanline_filter variable.
    void (*scanline_filter)(BYTE*, int, int, int) = NULL;
    switch (FIX2INT(filtering)) {
      case OILY_PNG_FILTER_SUB:     scanline_filter = &oily_png_encode_filter_sub; break;
      case OILY_PNG_FILTER_UP:      scanline_filter = &oily_png_encode_filter_up; break;
      case OILY_PNG_FILTER_AVERAGE: scanline_filter = &oily_png_encode_filter_average; break;
      case OILY_PNG_FILTER_PAETH:   scanline_filter = &oily_png_encode_filter_paeth; break;
      default: rb_raise(rb_eRuntimeError, "Unsupported filter type: %d", FIX2INT(filtering));
    }
    
    // Now, apply the scanline_filter function to every line, backwards.
    for (y = height - 1; y >= 0; y--) {
      scanline_filter(bytes, line_size * y, line_size, pixel_size);
    }
  }
  
  // Append to encoded image pass to the output stream.
  rb_str_cat(stream, (char*) bytes, pass_size);
  return Qnil;
}
