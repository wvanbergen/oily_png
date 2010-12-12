require 'chunky_png'

module OilyPNG
  
  VERSION = "0.2.0"

  def self.included(base)
    base::Canvas.send(:extend, OilyPNG::PNGDecoding)
    base::Canvas.send(:include, OilyPNG::PNGEncoding)
  end

end

require 'oily_png/oily_png'

# Include mixin into ChunkyPNG
ChunkyPNG.send(:include, OilyPNG)
