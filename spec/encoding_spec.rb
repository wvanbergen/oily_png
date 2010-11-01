require 'spec_helper'

describe OilyPNG::PNGEncoding do

  context 'encoding different color modes' do
    before do
      @canvas      = ChunkyPNG::Canvas.from_file(resource_file('gray.png'))
      @oily_canvas = OilyPNG::Canvas.from_canvas(@canvas)
    end
    
    it "should encode an image using grayscale correctly" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_GRAYSCALE, ChunkyPNG::FILTER_NONE)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_GRAYSCALE, ChunkyPNG::FILTER_NONE)
      stream1.should == stream2
    end
    
    it "should encode an image using grayscale alpha correctly" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_GRAYSCALE_ALPHA, ChunkyPNG::FILTER_NONE)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_GRAYSCALE_ALPHA, ChunkyPNG::FILTER_NONE)
      stream1.should == stream2
    end
    
    it "should encode an image using truecolor correctly" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_NONE)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_NONE)
      stream1.should == stream2
    end
    
    it "should encode an image using truecolor alpha correctly" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR_ALPHA, ChunkyPNG::FILTER_NONE)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR_ALPHA, ChunkyPNG::FILTER_NONE)
      stream1.should == stream2
    end
    
    it "should encode an image using indexed colors correctly" do
      # Setup an encoding palette first
      mock_palette = mock('palette', :index => 0xab)
      @canvas.stub(:encoding_palette).and_return(mock_palette)
      @oily_canvas.stub(:encoding_palette).and_return(mock_palette)
      
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_INDEXED, ChunkyPNG::FILTER_NONE)
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_INDEXED, ChunkyPNG::FILTER_NONE)
      stream1.should == stream2
    end
  end
  
  context 'encoding different filters' do
    before do 
      @canvas      = ChunkyPNG::Canvas.from_file(resource_file('nonsquare.png'))
      @oily_canvas = OilyPNG::Canvas.from_canvas(@canvas)
    end
    
    it "should encode correctly with no filtering" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_NONE)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_NONE)
      stream1.should == stream2
    end
    
    it "should encode correctly with sub filtering" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_SUB)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_SUB)
      stream1.should == stream2
    end
    
    it "should encode correctly with up filtering" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_UP)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_UP)
      stream1.should == stream2
    end

    it "should encode correctly with average filtering" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_AVERAGE)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_AVERAGE)
      stream1.should == stream2
    end
    
    it "should encode correctly with paeth filtering" do
      @oily_canvas.send(:encode_png_image_pass_to_stream, stream1 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_PAETH)
      @canvas.send(:encode_png_image_pass_to_stream,      stream2 = ChunkyPNG::Datastream.empty_bytearray, ChunkyPNG::COLOR_TRUECOLOR, ChunkyPNG::FILTER_PAETH)
      stream1.should == stream2
    end
  end
  
  it "should encode an interlaced image correctly" do
    canvas = ChunkyPNG::Canvas.from_file(resource_file('interlaced.png'))
    data = OilyPNG::Canvas.from_canvas(canvas).to_blob(:interlace => true)
    ds = ChunkyPNG::Datastream.from_blob(data)
    ds.header_chunk.interlace.should == ChunkyPNG::INTERLACING_ADAM7
    ChunkyPNG::Canvas.from_datastream(ds).should == canvas
  end
end
