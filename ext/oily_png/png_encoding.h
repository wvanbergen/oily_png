#ifndef PNG_ENCODING_H
#define PNG_ENCODING_H

#define R_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0xff000000) >> 24))
#define G_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x00ff0000) >> 16))
#define B_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x0000ff00) >> 8))
#define A_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x000000ff)))

#define FILTER_BYTE(byte, adjustment)  byte = (BYTE) (((byte) - (adjustment)) & 0x000000ff)
#define ENCODING_PALETTE_INDEX(encoding_palette, pixels, width, y, x) ((x < width) ? ((BYTE) NUM2UINT(rb_funcall(encoding_palette, rb_intern("index"), 1, rb_ary_entry(pixels, y * width + x)))) : 0)

typedef void(*scanline_encoder_func)(BYTE*, VALUE, long, long, VALUE);

/*
  Encodes an image and append it to the stream.
  A normal PNG will only have one pass and call this method once, while interlaced
  images are split up in 7 distinct images. This method will be called for every one
  of these images, reusing the stream.
  
  This function should replace ChunkyPNG::Canvas::PNGEncoding.encode_png_image_pass_to_stream
*/
VALUE oily_png_encode_png_image_pass_to_stream(VALUE self, VALUE stream, VALUE color_mode, VALUE bit_depth, VALUE filtering);

#endif
