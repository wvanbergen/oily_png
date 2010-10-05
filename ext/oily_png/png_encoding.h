#ifndef PNG_ENCODING_H
#define PNG_ENCODING_H

// Function to overwrite ChunkyPNG::Canvas::PNGEncoding.encode_png_image_pass_to_stream
VALUE oily_png_encode_png_image_pass_to_stream(VALUE self, VALUE stream, VALUE color_mode, VALUE filtering);

#endif
