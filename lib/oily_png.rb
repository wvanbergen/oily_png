require 'chunky_png'
require 'oily_png/png_decoding'

module OilyPNG
  
  VERSION = "0.0.3"

  def self.included(base)
    base::Canvas.extend OilyPNG::PNGDecoding
  end

end

ChunkyPNG.send(:include, OilyPNG)
