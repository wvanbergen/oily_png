require 'rubygems'
require 'bundler/setup'
require 'chunky_png'
require 'oily_png/oily_png'


module OilyPNG
  class Canvas < ChunkyPNG::Canvas
    extend OilyPNG::PNGDecoding
    include OilyPNG::PNGEncoding
  end
end

module ResourceHelper
  def resource_files
    Dir[File.join(File.dirname(__FILE__), 'resources', '*.png')]
  end
end

module CanvasHelper
  
  def resource_files(pattern = "*.png")
    Dir[File.join(File.dirname(__FILE__), 'resources', pattern)]
  end
  
  def resource_file(name)
    File.join(File.dirname(__FILE__), 'resources', name)
  end
  
  def display(canvas)
    filename = resource_file('_tmp.png')
    canvas.to_datastream.save(filename)
    `open #{filename}`
  end
  
  def reference_canvas(name)
    ChunkyPNG::Canvas.from_file(resource_file("#{name}.png"))
  end
end

RSpec.configure do |config|
  config.extend ResourceHelper
  config.include CanvasHelper
end
