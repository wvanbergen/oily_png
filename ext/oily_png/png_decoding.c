#include "oily_png_ext.h"


// Decodes a SUB filtered scanline at the given position in the byte array
void oily_png_decode_filter_sub(BYTE* bytes, int pos, int line_length, int pixel_size) {
  int i;
  for (i = 1 + pixel_size; i < line_length; i++) {
    UNFILTER_BYTE(bytes[pos + i], bytes[pos + i - pixel_size]);
  }
}

// Decodes an UP filtered scanline at the given position in the byte array
void oily_png_decode_filter_up(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int i;
  // The first line is not filtered because there is no privous line
  if (pos >= line_size) {
    for (i = 1; i < line_size; i++) {
      UNFILTER_BYTE(bytes[pos + i], bytes[pos + i - line_size]);
    }
  }
}

// Decodes an AVERAGE filtered scanline at the given position in the byte array
void oily_png_decode_filter_average(BYTE* bytes, int pos, int line_size, int pixel_size) {
  int i;
  BYTE a, b;
  for (i = 1; i < line_size; i++) {
    a = (i > pixel_size)     ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_size) ? bytes[pos + i - line_size] : 0;
    UNFILTER_BYTE(bytes[pos + i], (a + b) >> 1);
  }
}

// Decodes a PAETH filtered scanline at the given position in the byte array
void oily_png_decode_filter_paeth(BYTE* bytes, int pos, int line_size, int pixel_size) {
  BYTE a, b, c, pr;
  int i, p, pa, pb, pc;
  for (i = 1; i < line_size; i++) {
    a = (i > pixel_size) ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_size) ? bytes[pos + i - line_size] : 0;
    c = (pos >= line_size && i > pixel_size) ? bytes[pos + i - line_size - pixel_size] : 0;
    p = a + b - c;
    pa = abs(p - a);
    pb = abs(p - b);
    pc = abs(p - c);
    pr = (pa <= pb) ? (pa <= pc ? a : c) : (pb <= pc ? b : c);
    UNFILTER_BYTE(bytes[pos + i], pr);
  }
}

PIXEL oily_png_decode_pixel_grayscale(int color_mode, BYTE* bytes, int byte_index, VALUE decoding_palette) {
  return BUILD_PIXEL(bytes[byte_index], bytes[byte_index], bytes[byte_index], 0xff);
}

PIXEL oily_png_decode_pixel_truecolor(int color_mode, BYTE* bytes, int byte_index, VALUE decoding_palette) {
  return BUILD_PIXEL(bytes[byte_index], bytes[byte_index + 1], bytes[byte_index + 2], 0xff);
}

PIXEL oily_png_decode_pixel_indexed(int color_mode, BYTE* bytes, int byte_index, VALUE decoding_palette) {
  return (PIXEL) NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(bytes[byte_index])));
}

PIXEL oily_png_decode_pixel_grayscale_alpha(int color_mode, BYTE* bytes, int byte_index, VALUE decoding_palette) {
  return BUILD_PIXEL(bytes[byte_index], bytes[byte_index], bytes[byte_index], bytes[byte_index + 1]);
}

PIXEL oily_png_decode_pixel_truecolor_alpha(int color_mode, BYTE* bytes, int byte_index, VALUE decoding_palette) {
  return BUILD_PIXEL(bytes[byte_index], bytes[byte_index + 1], bytes[byte_index + 2], bytes[byte_index + 3]);
}

VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE start_pos) {
  
  int pixel_size = oily_png_pixel_size(FIX2INT(color_mode));
  int line_size = pixel_size * FIX2INT(width) + 1;
  int pass_size = line_size * FIX2INT(height);
  
  VALUE pixels = rb_ary_new();
  
  // Make sure that the stream is large enough to contain out pass.
  if (RSTRING_LEN(stream) < pass_size + FIX2INT(start_pos)) {
    rb_raise(rb_eRuntimeError, "The length of the stream is too short to contain the image!");
  }

  // Copy the bytes for this pass from the stream to a separate location
  // so we can work on this byte array directly.
  BYTE* pixelstream = (BYTE*) RSTRING_PTR(stream);
  BYTE* bytes = ALLOCA_N(BYTE, pass_size);
  memcpy(bytes, pixelstream + FIX2INT(start_pos), pass_size);

  // Get the decoding palette for indexed images.
  VALUE decoding_palette = Qnil;
  if (FIX2INT(color_mode) == OILY_PNG_COLOR_INDEXED) {
    decoding_palette = rb_funcall(self, rb_intern("decoding_palette"), 0);
  }

  // Select the pixel decoder function for this color mode.
  PIXEL (*pixel_decoder)(int, BYTE*, int, VALUE);
  switch (FIX2INT(color_mode)) {
    case OILY_PNG_COLOR_GRAYSCALE:       pixel_decoder = &oily_png_decode_pixel_grayscale; break;
    case OILY_PNG_COLOR_TRUECOLOR:       pixel_decoder = &oily_png_decode_pixel_truecolor; break;
    case OILY_PNG_COLOR_INDEXED:         pixel_decoder = &oily_png_decode_pixel_indexed; break;
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA: pixel_decoder = &oily_png_decode_pixel_grayscale_alpha; break;
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA: pixel_decoder = &oily_png_decode_pixel_truecolor_alpha; break;
    default: rb_raise(rb_eRuntimeError, "Color mode not supported: %d", color_mode);
  }
  
  int y, x, line_start, byte_index, pixel_index;
  PIXEL pixel;
  
  for (y = 0; y < FIX2INT(height); y++) {
    line_start = y * line_size;
    
    // Apply filering to the line
    switch (bytes[line_start]) {
      case OILY_PNG_FILTER_NONE:    break;
      case OILY_PNG_FILTER_SUB:     oily_png_decode_filter_sub(     bytes, line_start, line_size, pixel_size); break;
      case OILY_PNG_FILTER_UP:      oily_png_decode_filter_up(      bytes, line_start, line_size, pixel_size); break;
      case OILY_PNG_FILTER_AVERAGE: oily_png_decode_filter_average( bytes, line_start, line_size, pixel_size); break;
      case OILY_PNG_FILTER_PAETH:   oily_png_decode_filter_paeth(   bytes, line_start, line_size, pixel_size); break;
      default: rb_raise(rb_eRuntimeError, "Filter type not supported: %d", bytes[line_start]);
    }
    
    // Set the filter byte to 0 because the bytearray is now unfiltered.
    bytes[line_start] = OILY_PNG_FILTER_NONE;
    
    // Now, iterate over all bytes in this line and combine them into pixels
    for (x = 0; x < FIX2INT(width); x++) {
      pixel_index = FIX2INT(width) * y + x;
      byte_index  = line_start + 1 + (x * pixel_size);
      pixel = pixel_decoder(FIX2INT(color_mode), bytes, byte_index, decoding_palette); 
      rb_ary_store(pixels, pixel_index, INT2NUM(pixel));
    }
  }
  
  // Now, return a new ChunkyPNG::Canvas instance with the decoded pixels.
  return rb_funcall(self, rb_intern("new"), 3, width, height, pixels);
}

