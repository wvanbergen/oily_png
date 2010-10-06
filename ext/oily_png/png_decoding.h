#ifndef PNG_DECODING_H
#define PNG_DECODING_H

/*
  Decodes an image pass from the given byte stream at the given position.
  A normal PNG will only have one pass that consumes the entire stream, while an
  interlaced image requires 7 passes which are loaded from different starting positions.
  
  This function shouild replace ChunkyPNG::Canvas::PNGDecoding.decode_png_image_pass
*/
VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE start_pos);

#endif
