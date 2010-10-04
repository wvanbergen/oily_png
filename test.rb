require 'rubygems'
require 'chunky_png'

require 'oily_png'

color_mode = ChunkyPNG::COLOR_TRUECOLOR
filtering  = ChunkyPNG::FILTER_NONE

# Decode without speedup
test_image = ChunkyPNG::Canvas.from_file('test.png')
test_image.save('_tmp.png', :color_mode => color_mode, :filtering => filtering)

# Enable OilyPNG speedup
ChunkyPNG::Canvas.send(:extend, OilyPNG::PNGDecoding)
decoded_image = ChunkyPNG::Canvas.from_file('_tmp.png')

# Check whether compatible
p decoded_image == test_image
File.unlink('_tmp.png')
