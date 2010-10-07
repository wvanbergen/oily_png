require 'chunky_png'

module OilyPNG
  
  VERSION = "0.0.7"

  def self.included(base)
    base::Canvas.send(:extend, OilyPNG::PNGDecoding)
    base::Canvas.send(:include, OilyPNG::PNGEncoding)
  end

end

require 'oily_png/oily_png_ext'

# Include mixin into ChunkyPNG
ChunkyPNG.send(:include, OilyPNG)
