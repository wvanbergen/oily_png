#include "oily_png_ext.h"

////////////////////////////////////////////////////////////////////////////

// Decodes a pixel at the given position in the bytearray
PIXEL oily_png_decode_pixel(int color_mode, unsigned char* bytes, int byte_index, VALUE decoding_palette) {
  switch (color_mode) {
    case OILY_PNG_COLOR_GRAYSCALE:
      return (bytes[byte_index] << 24) + (bytes[byte_index] << 16) + (bytes[byte_index] << 8) + 0xff;
    case OILY_PNG_COLOR_TRUECOLOR:
      return (bytes[byte_index] << 24) + (bytes[byte_index + 1] << 16) + (bytes[byte_index + 2] << 8) + 0xff;
    case OILY_PNG_COLOR_INDEXED:
      return NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(bytes[byte_index])));
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA:
      return (bytes[byte_index] << 24) + (bytes[byte_index] << 16) + (bytes[byte_index] << 8) + bytes[byte_index + 1];
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA:
      return (bytes[byte_index] << 24) + (bytes[byte_index + 1] << 16) + (bytes[byte_index + 2] << 8) + bytes[byte_index + 3];
    default: 
      exit(1);
  }
}

// Decodes a SUB filtered scanline at the given position in the byte array
void oily_png_decode_filter_sub(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  int i;
  for (i = 1 + pixel_size; i < line_length; i++) {
    bytes[pos + i] += bytes[pos + i - pixel_size]; // mod 256 ???
  }
}

// Decodes an UP filtered scanline at the given position in the byte array
void oily_png_decode_filter_up(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  int i;
  // The first line is not filtered because there is no privous line
  if (pos >= line_length) {
    for (i = 1; i < line_length; i++) {
      bytes[pos + i] += bytes[pos + i - line_length]; // mod 256 ???
    }
  }
}

// Decodes an AVERAGE filtered scanline at the given position in the byte array
void oily_png_decode_filter_average(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  int i;
  unsigned char a, b;
  for (i = 1; i < line_length; i++) {
    a = (i > pixel_size)     ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_length) ? bytes[pos + i - line_length] : 0;
    bytes[pos + i] += (a + b) >> 1; // mod 256 ???
  }
}

// Decodes a PAETH filtered scanline at the given position in the byte array
void oily_png_decode_filter_paeth(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  unsigned char a, b, c, pr;
  int i, p, pa, pb, pc;
  for (i = 1; i < line_length; i++) {
    a = (i > pixel_size) ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_length) ? bytes[pos + i - line_length] : 0;
    c = (pos >= line_length && i > pixel_size) ? bytes[pos + i - line_length - pixel_size] : 0;
    p = a + b - c;
    pa = abs(p - a);
    pb = abs(p - b);
    pc = abs(p - c);
    pr = (pa <= pb) ? (pa <= pc ? a : c) : (pb <= pc ? b : c);
    bytes[pos + i] += pr; // mod 256 ???
  }
}

// Decodes an image pass from the given byte stream at the given position.
// A normal PNG will only have one pass that consumes the entire stream, while an
// interlaced image requires 7 passes which are loaded from different starting positions.
VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE start_pos) {
  
  int pixel_size = oily_png_pixel_size(FIX2INT(color_mode));
  int line_size = pixel_size * FIX2INT(width) + 1;
  int pass_size = line_size * FIX2INT(height);
  
  VALUE pixels = rb_ary_new();
  
  VALUE decoding_palette = Qnil;
  if (FIX2INT(color_mode) == OILY_PNG_COLOR_INDEXED) {
    decoding_palette = rb_funcall(self, rb_intern("decoding_palette"), 0);
  }
  
  // Copy the bytes for this pass from the stream to a separate location
  // so we can work on this byte array directly.
  unsigned char* pixelstream = RSTRING_PTR(stream);
  unsigned char* bytes = malloc(pass_size);
  memcpy(bytes, pixelstream + FIX2INT(start_pos), pass_size);

  int y, x, line_start, prev_line_start, byte_index, pixel_index;
  unsigned char filter;
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
      default: exit(1);
    }
    
    // Set the filter byte to 0 because the bytearray is now unfiltered.
    bytes[line_start] = OILY_PNG_FILTER_NONE;
    
    // Now, iterate over all bytes in this line and combine them into pixels
    for (x = 0; x < FIX2INT(width); x++) {
      pixel_index = FIX2INT(width) * y + x;
      byte_index  = line_start + 1 + (x * pixel_size);
      pixel = oily_png_decode_pixel(FIX2INT(color_mode), bytes, byte_index, decoding_palette); 
      rb_ary_store(pixels, pixel_index, INT2NUM(pixel));
    }
  }
  
  // Get rid of the byte array.
  free(bytes);
  
  // Now, return a new ChunkyPNG::Canvas instance with the decoded pixels.
  return rb_funcall(self, rb_intern("new"), 3, width, height, pixels);
}

