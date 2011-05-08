require 'chunky_png'

module OilyPNG
  
  VERSION = "1.0.1"

  def self.included(base)
    base::Canvas.send(:extend, OilyPNG::PNGDecoding)
    base::Canvas.send(:include, OilyPNG::PNGEncoding)

    base::Color.send(:include, OilyPNG::Color)
  end
end

require 'oily_png/oily_png'

# Include mixin into ChunkyPNG
ChunkyPNG.send(:include, OilyPNG)
