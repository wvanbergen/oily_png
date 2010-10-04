#include "ruby.h"

// Variable for the OilyPNG module
VALUE OilyPNG = Qnil;

void Init_decoding();

VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE start_pos);

// COLOR_GRAYSCALE       = 0
// COLOR_TRUECOLOR       = 2
// COLOR_INDEXED         = 3
// COLOR_GRAYSCALE_ALPHA = 4
// COLOR_TRUECOLOR_ALPHA = 6
// 
// FILTERING_DEFAULT     = 0
// 
// COMPRESSION_DEFAULT   = 0
// 
// INTERLACING_NONE      = 0
// INTERLACING_ADAM7     = 1
// 
// FILTER_NONE           = 0
// FILTER_SUB            = 1
// FILTER_UP             = 2
// FILTER_AVERAGE        = 3
// FILTER_PAETH          = 4

void Init_decoding() {
  VALUE OilyPNG = rb_define_module("OilyPNG");
  VALUE OilyPNGDecoding = rb_define_module_under(OilyPNG, "PNGDecoding");
  rb_define_method(OilyPNGDecoding, "decode_png_image_pass", oily_png_decode_png_image_pass, 5);
}

int oily_png_pixel_size(color_mode) {
  switch (color_mode) {
    case 0:  return 1;
    case 2:  return 3;
    case 3:  return 1;
    case 4:  return 2;
    case 6:  return 4;
    default: return -1;
  }
}

unsigned int oily_png_decode_pixel(int color_mode, unsigned char* bytes, int byte_index, VALUE decoding_palette) {
  switch (color_mode) {
    case 0:
      return (bytes[byte_index] << 24) + (bytes[byte_index] << 16) + (bytes[byte_index] << 8) + 0xff;
    case 2:
      return (bytes[byte_index] << 24) + (bytes[byte_index + 1] << 16) + (bytes[byte_index + 2] << 8) + 0xff;
    case 3:
      return NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(bytes[byte_index])));
    case 4:
      return (bytes[byte_index] << 24) + (bytes[byte_index] << 16) + (bytes[byte_index] << 8) + bytes[byte_index + 1];
    case 6:
      return (bytes[byte_index] << 24) + (bytes[byte_index + 1] << 16) + (bytes[byte_index + 2] << 8) + bytes[byte_index + 3];
    default: 
      exit(1);
  }
}

void oily_png_filter_sub(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  int i;
  for (i = 1 + pixel_size; i < line_length; i++) {
    bytes[pos + i] += bytes[pos + i - pixel_size]; // mod 256 ???
  }
}

void oily_png_filter_up(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  int i;
  if (pos >= line_length) {
    for (i = 1; i < line_length; i++) {
      bytes[pos + i] += bytes[pos + i - line_length]; // mod 256 ???
    }
  }
}

void oily_png_filter_average(unsigned char* bytes, int pos, int line_length, int pixel_size) {
  int i;
  unsigned char a, b;
  for (i = 1; i < line_length; i++) {
    a = (i > pixel_size)     ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_length) ? bytes[pos + i - line_length] : 0;
    bytes[pos + i] += (a + b) >> 1;
  }
}

void oily_png_filter_paeth(unsigned char* bytes, int pos, int line_length, int pixel_size) {
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
    bytes[pos + i] += pr;
  }
}


VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE start_pos) {
  
  int pixel_size = oily_png_pixel_size(FIX2INT(color_mode));
  int line_size = pixel_size * FIX2INT(width) + 1;
  int pass_size = line_size * FIX2INT(height);
  
  VALUE pixels = rb_ary_new();
  
  VALUE decoding_palette = Qnil;
  if (FIX2INT(color_mode) == 3) {
    decoding_palette = rb_funcall(self, rb_intern("decoding_palette"), 0);
  }
  
  unsigned char* pixelstream = RSTRING_PTR(stream);
  unsigned char* bytes = malloc(pass_size);
  memcpy(bytes, pixelstream + FIX2INT(start_pos), pass_size);

  int y, x, line_start, prev_line_start, byte_index, pixel_index;
  unsigned char filter;
  unsigned int pixel;
  
  for (y = 0; y < FIX2INT(height); y++) {
    line_start = y * line_size;
    filter = bytes[line_start];
    
    switch (filter) {
      case 0: break;
      case 1: oily_png_filter_sub(     bytes, line_start, line_size, pixel_size); break;
      case 2: oily_png_filter_up(      bytes, line_start, line_size, pixel_size); break;
      case 3: oily_png_filter_average( bytes, line_start, line_size, pixel_size); break;
      case 4: oily_png_filter_paeth(   bytes, line_start, line_size, pixel_size); break;
      default: exit(1);
    }
    
    for (x = 0; x < FIX2INT(width); x++) {
      pixel_index = FIX2INT(width) * y + x;
      byte_index  = line_start + 1 + (x * pixel_size);
      pixel = oily_png_decode_pixel(FIX2INT(color_mode), bytes, byte_index, decoding_palette); 
      rb_ary_store(pixels, pixel_index, INT2NUM(pixel));
    }
  }
  
  free(bytes);
  return rb_funcall(self, rb_intern("new"), 3, width, height, pixels);
}

