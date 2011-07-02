#ifndef OILY_PNG_RESAMPLING_H
#define OILY_PNG_RESAMPLING_H

/*
 * Generates the interpolation steps (nearest neighbour) through two values.
 */
void oily_png_generate_steps(unsigned int width, unsigned int new_width, int *steps);

/*
 * Generates the interpolation steps through two values.
 * 
 * Returns a Ruby Array
 */
VALUE oily_png_canvas_steps(VALUE self, VALUE width, VALUE new_width);

#endif
