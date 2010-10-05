require 'chunky_png'

module OilyPNG
  
  VERSION = "0.0.3"

  def self.included(base)
    base::Canvas.extend OilyPNG::PNGDecoding
  end

end

require 'oily_png/oily_png_ext'

# Include mixin into ChunkyPNG
ChunkyPNG.send(:include, OilyPNG)
