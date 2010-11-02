#include "oily_png_ext.h"


// Decodes a SUB filtered scanline at the given position in the byte array
void oily_png_decode_filter_sub(BYTE* bytes, long pos, long line_length, char pixel_size) {
  long i;
  for (i = 1 + pixel_size; i < line_length; i++) {
    UNFILTER_BYTE(bytes[pos + i], bytes[pos + i - pixel_size]);
  }
}

// Decodes an UP filtered scanline at the given position in the byte array
void oily_png_decode_filter_up(BYTE* bytes, long pos, long line_size, char pixel_size) {
  UNUSED_PARAMETER(pixel_size);
  long i;
  if (pos >= line_size) { // The first line is not filtered because there is no privous line
    for (i = 1; i < line_size; i++) {
      UNFILTER_BYTE(bytes[pos + i], bytes[pos + i - line_size]);
    }
  }
}

// Decodes an AVERAGE filtered scanline at the given position in the byte array
void oily_png_decode_filter_average(BYTE* bytes, long pos, long line_size, char pixel_size) {
  long i;
  BYTE a, b;
  for (i = 1; i < line_size; i++) {
    a = (i > pixel_size)     ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_size) ? bytes[pos + i - line_size] : 0;
    UNFILTER_BYTE(bytes[pos + i], (a + b) >> 1);
  }
}

// Decodes a PAETH filtered scanline at the given position in the byte array
void oily_png_decode_filter_paeth(BYTE* bytes, long pos, long line_size, char pixel_size) {
  BYTE a, b, c, pr;
  long i, p, pa, pb, pc;
  for (i = 1; i < line_size; i++) {
    a = (i > pixel_size) ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_size) ? bytes[pos + i - line_size] : 0;
    c = (pos >= line_size && i > pixel_size) ? bytes[pos + i - line_size - pixel_size] : 0;
    p = a + b - c;
    pa = (p > a) ? p - a : a - p;
    pb = (p > b) ? p - b : b - p;
    pc = (p > c) ? p - c : c - p;
    pr = (pa <= pb) ? (pa <= pc ? a : c) : (pb <= pc ? b : c);
    UNFILTER_BYTE(bytes[pos + i], pr);
  }
}

BYTE oily_png_extract_1bit_element(BYTE* bytes, long start, long x) {
  BYTE byte = bytes[start + 1 + (x >> 3)];
  char bitshift = 7 - (x & (BYTE) 0x07);
  return (byte & (0x01 << bitshift)) >> bitshift;
}


BYTE oily_png_extract_2bit_element(BYTE* bytes, long start, long x) {
  BYTE byte = bytes[start + 1 + (x >> 2)];
  char bitshift = (6 - ((x & (BYTE) 0x03) << 1));
  return (byte & (0x03 << bitshift)) >> bitshift;
}

BYTE oily_png_extract_4bit_element(BYTE* bytes, long start, long x) {
  return ((x & 0x01) == 0) ? ((bytes[(start) + 1 + ((x) >> 1)] & (BYTE) 0xf0) >> 4) : (bytes[(start) + 1 + ((x) >> 1)] & (BYTE) 0x0f);
}

BYTE oily_png_resample_1bit_element(BYTE* bytes, long start, long x) {
  BYTE value = oily_png_extract_1bit_element(bytes, start, x);
  return (value == 0) ? 0x00 : 0xff;
}

BYTE oily_png_resample_2bit_element(BYTE* bytes, long start, long x) {
  switch (oily_png_extract_2bit_element(bytes, start, x)) {
    case 0x00: return 0x00;
    case 0x01: return 0x55;
    case 0x02: return 0xaa;
    case 0x03: default: return 0xff;
  }
}

BYTE oily_png_resample_4bit_element(BYTE* bytes, long start, long x) {
  switch(oily_png_extract_4bit_element(bytes, start, x)) {
    case 0x00: return 0;
    case 0x01: return 17;
    case 0x02: return 34;
    case 0x03: return 51;
    case 0x04: return 68;
    case 0x05: return 85;
    case 0x06: return 102;
    case 0x07: return 119;
    case 0x08: return 137;
    case 0x09: return 154;
    case 0x0a: return 171;
    case 0x0b: return 188;
    case 0x0c: return 205;
    case 0x0d: return 222;
    case 0x0e: return 239;
    case 0x0f: default: return 255;
  }
}


PIXEL oily_png_decode_pixel_indexed_8bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  return (PIXEL) NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(bytes[start + 1 + x])));
}

PIXEL oily_png_decode_pixel_indexed_4bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  return (PIXEL) NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(oily_png_extract_4bit_element(bytes, start, x))));
}

PIXEL oily_png_decode_pixel_indexed_2bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  return (PIXEL) NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(oily_png_extract_2bit_element(bytes, start, x))));
}

PIXEL oily_png_decode_pixel_indexed_1bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  return (PIXEL) NUM2UINT(rb_funcall(decoding_palette, rb_intern("[]"), 1, INT2FIX(oily_png_extract_1bit_element(bytes, start, x))));
}


PIXEL oily_png_decode_pixel_grayscale_8bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + x], 
                      bytes[start + 1 + x], 
                      bytes[start + 1 + x], 
                      0xff);
}

PIXEL oily_png_decode_pixel_grayscale_1bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( oily_png_resample_1bit_element(bytes, start, x), 
                      oily_png_resample_1bit_element(bytes, start, x), 
                      oily_png_resample_1bit_element(bytes, start, x), 
                      0xff);
}

PIXEL oily_png_decode_pixel_grayscale_2bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( oily_png_resample_2bit_element(bytes, start, x), 
                      oily_png_resample_2bit_element(bytes, start, x), 
                      oily_png_resample_2bit_element(bytes, start, x), 
                      0xff);
}

PIXEL oily_png_decode_pixel_grayscale_4bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( oily_png_resample_4bit_element(bytes, start, x), 
                      oily_png_resample_4bit_element(bytes, start, x), 
                      oily_png_resample_4bit_element(bytes, start, x), 
                      0xff);
}

PIXEL oily_png_decode_pixel_truecolor_8bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 3) + 0], 
                      bytes[start + 1 + (x * 3) + 1], 
                      bytes[start + 1 + (x * 3) + 2], 
                      0xff);
}

PIXEL oily_png_decode_pixel_grayscale_alpha_8bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 2) + 0], 
                      bytes[start + 1 + (x * 2) + 0], 
                      bytes[start + 1 + (x * 2) + 0], 
                      bytes[start + 1 + (x * 2) + 1]);
}

PIXEL oily_png_decode_pixel_truecolor_alpha_8bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 4) + 0], 
                      bytes[start + 1 + (x * 4) + 1], 
                      bytes[start + 1 + (x * 4) + 2], 
                      bytes[start + 1 + (x * 4) + 3]);
}

PIXEL oily_png_decode_pixel_grayscale_16bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 2)], 
                      bytes[start + 1 + (x * 2)], 
                      bytes[start + 1 + (x * 2)], 
                      0xff);
}

PIXEL oily_png_decode_pixel_truecolor_16bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 6) + 0], 
                      bytes[start + 1 + (x * 6) + 2], 
                      bytes[start + 1 + (x * 6) + 4], 
                      0xff);
}

PIXEL oily_png_decode_pixel_grayscale_alpha_16bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 4) + 0], 
                      bytes[start + 1 + (x * 4) + 0], 
                      bytes[start + 1 + (x * 4) + 0], 
                      bytes[start + 1 + (x * 4) + 2]);
}

PIXEL oily_png_decode_pixel_truecolor_alpha_16bit(BYTE* bytes, long start, long x, VALUE decoding_palette) {
  UNUSED_PARAMETER(decoding_palette);
  return BUILD_PIXEL( bytes[start + 1 + (x * 8) + 0], 
                      bytes[start + 1 + (x * 8) + 2], 
                      bytes[start + 1 + (x * 8) + 4], 
                      bytes[start + 1 + (x * 8) + 6]);
}


pixel_decoder_func oily_png_decode_pixel_func(int color_mode, int bit_depth) {
  switch (color_mode) {
    case OILY_PNG_COLOR_GRAYSCALE:
      switch (bit_depth) {
        case  1: return &oily_png_decode_pixel_grayscale_1bit;
        case  2: return &oily_png_decode_pixel_grayscale_2bit;
        case  4: return &oily_png_decode_pixel_grayscale_4bit;
        case  8: return &oily_png_decode_pixel_grayscale_8bit;
        case 16: return &oily_png_decode_pixel_grayscale_16bit;
        default: return NULL;
      }

    case OILY_PNG_COLOR_TRUECOLOR:
      switch (bit_depth) {
        case  8: return &oily_png_decode_pixel_truecolor_8bit;
        case 16: return &oily_png_decode_pixel_truecolor_16bit;
        default: return NULL;
      }
      
    case OILY_PNG_COLOR_INDEXED:
      switch (bit_depth) {
        case 1: return &oily_png_decode_pixel_indexed_1bit;
        case 2: return &oily_png_decode_pixel_indexed_2bit;
        case 4: return &oily_png_decode_pixel_indexed_4bit;
        case 8: return &oily_png_decode_pixel_indexed_8bit;
        default: return NULL;
      }
      
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA: 
      switch (bit_depth) {
        case  8: return &oily_png_decode_pixel_grayscale_alpha_8bit;
        case 16: return &oily_png_decode_pixel_grayscale_alpha_16bit;
        default: return NULL;
      }

    case OILY_PNG_COLOR_TRUECOLOR_ALPHA:
      switch (bit_depth) {
        case  8: return &oily_png_decode_pixel_truecolor_alpha_8bit;
        case 16: return &oily_png_decode_pixel_truecolor_alpha_16bit;
        default: return NULL;
      }
    
    default: return NULL;
  }
}

VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE depth, VALUE start_pos) {
  
  VALUE pixels = rb_ary_new();
  
  if ((FIX2LONG(height) > 0) && (FIX2LONG(width) > 0)) {

    char pixel_size = oily_png_pixel_bytesize(FIX2INT(color_mode), FIX2INT(depth));
    long line_size  = oily_png_scanline_bytesize(FIX2INT(color_mode), FIX2INT(depth), FIX2LONG(width));
    long pass_size  = oily_png_pass_bytesize(FIX2INT(color_mode), FIX2INT(depth), FIX2LONG(width), FIX2LONG(height));
      
    // Make sure that the stream is large enough to contain our pass.
    if (RSTRING_LEN(stream) < pass_size + FIX2LONG(start_pos)) {
      rb_raise(rb_eRuntimeError, "The length of the stream is too short to contain the image!");
    }

    // Copy the bytes for this pass from the stream to a separate location
    // so we can work on this byte array directly.
    BYTE* bytes = ALLOCA_N(BYTE, pass_size);
    memcpy(bytes, RSTRING_PTR(stream) + FIX2LONG(start_pos), pass_size);

    // Get the decoding palette for indexed images.
    VALUE decoding_palette = Qnil;
    if (FIX2INT(color_mode) == OILY_PNG_COLOR_INDEXED) {
      decoding_palette = rb_funcall(self, rb_intern("decoding_palette"), 0);
    }

    // Select the pixel decoder function for this color mode.
    PIXEL (*pixel_decoder)(BYTE*, long, long, VALUE) = NULL;
    pixel_decoder = oily_png_decode_pixel_func(FIX2INT(color_mode), FIX2INT(depth));
  
    if (pixel_decoder == NULL) {
      rb_raise(rb_eRuntimeError, "No decoder for color mode %d and bit depth %d", FIX2INT(color_mode), FIX2INT(depth));
    }
  
    long y, x, line_start;
    PIXEL pixel;
  
    for (y = 0; y < FIX2LONG(height); y++) {
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
    
      // Now, iterate over all bytes in this line and convert them into pixels
      for (x = 0; x < FIX2LONG(width); x++) {
        pixel = pixel_decoder(bytes, line_start, x, decoding_palette); 
        rb_ary_store(pixels, FIX2LONG(width) * y + x, UINT2NUM(pixel));
      }
    }
  }
  
  // Now, return a new ChunkyPNG::Canvas instance with the decoded pixels.
  return rb_funcall(self, rb_intern("new"), 3, width, height, pixels);
}
