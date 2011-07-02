#include "oily_png_ext.h"


void oily_png_generate_steps(unsigned int width, unsigned int new_width, int *steps) {
  int base_step = width / new_width;
  int err = width % new_width;

  int index = base_step >> 2;
  int eps = err;

  if (base_step & 1) {
    index++;
    eps -= new_width >> 2;
    if (new_width & 1) {
      eps--;
    }
  }

  int i;
  for (i=0; i < new_width; i++) {
    steps[i] = index;

    index += base_step;
    eps += err;
    if (eps > new_width) {
      index++;
      eps -= new_width;
    }
  }
}

VALUE oily_png_canvas_steps(VALUE self, VALUE v_width, VALUE v_new_width) {
  unsigned int width = NUM2UINT(v_width);
  unsigned int new_width = NUM2UINT(v_new_width);

  
  VALUE ret = rb_ary_new2(new_width);

  int *steps = ALLOC_N(int, new_width);
  oily_png_generate_steps(width, new_width, steps);


  int i;
  for (i=0; i < new_width; i++) {
    rb_ary_store(ret, i, INT2FIX(steps[i]));
  }
  
  /* This is an unprotected allocation, will leak on exception.
   * However, rb_ary_store should not generate one.
   */
  xfree(steps);
  return ret;

}