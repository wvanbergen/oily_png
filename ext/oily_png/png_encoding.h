#ifndef PNG_ENCODING_H
#define PNG_ENCODING_H

/*
  Encodes an image and append it to the stream.
  A normal PNG will only have one pass and call this method once, while interlaced
  images are split up in 7 distinct images. This method will be called for every one
  of these images, reusing the stream.
  
  This function should replace ChunkyPNG::Canvas::PNGEncoding.encode_png_image_pass_to_stream
*/
VALUE oily_png_encode_png_image_pass_to_stream(VALUE self, VALUE stream, VALUE color_mode, VALUE filtering);

#endif
