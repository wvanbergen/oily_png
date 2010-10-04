require 'chunky_png'
require 'oily_png/decoding'

module OilyPNG
  
  VERSION = "0.0.1"

  def self.included(base)
    base::Canvas.extend OilyPNG::Decoding
  end

end

ChunkyPNG.send(:include, OilyPNG)
