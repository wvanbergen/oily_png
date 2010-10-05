require 'rubygems'
require 'bundler'

Bundler.setup

require 'spec'
require 'chunky_png'
require 'oily_png/decoding'

class OilyCanvas < ChunkyPNG::Canvas
  extend OilyPNG::PNGDecoding
end


module ResourceHelper
  def resource_files
    Dir[File.join(File.dirname(__FILE__), 'resources', '*.png')]
  end
end

module CanvasHelper
  
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

Spec::Runner.configure do |config|
  config.extend ResourceHelper
  config.include CanvasHelper
end
