#ifndef PNG_DECODING_H
#define PNG_DECODING_H

// Function to overwrite ChunkyPNG::Canvas::PNGDecoding.decode_png_image_pass
VALUE oily_png_decode_png_image_pass(VALUE self, VALUE stream, VALUE width, VALUE height, VALUE color_mode, VALUE start_pos);

#endif
