require 'chunky_png'

module OilyPNG
  
  VERSION = "0.0.4"

  def self.included(base)
    base::Canvas.extend OilyPNG::PNGDecoding
    base::Canvas.include OilyPNG::PNGEncoding
  end

end

require 'oily_png/oily_png_ext'

# Include mixin into ChunkyPNG
ChunkyPNG.send(:include, OilyPNG)
