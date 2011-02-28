require 'spec_helper'

describe OilyPNG::Operations do
  describe '#compose' do
    before do
      @canvas       = reference_canvas('operations')
      @oily_canvas  = oily_reference_canvas('operations')
    end
    
    it "should compose two images without offset exactly the same way as ChunkyPNG" do
      subcanvas = ChunkyPNG::Canvas.new(4, 8, ChunkyPNG::Color.rgba(0, 0, 0, 75))
      oily_subcanvas = OilyPNG::Canvas.new(4, 8, ChunkyPNG::Color.rgba(0, 0, 0, 75))
      @oily_canvas.compose(oily_subcanvas).pixels.should == @canvas.compose(subcanvas).pixels
    end
    
    it "should compose two images with offset exactly the same way as ChunkyPNG" do
      subcanvas = ChunkyPNG::Canvas.new(4, 8, ChunkyPNG::Color.rgba(0, 0, 0, 75))
      oily_subcanvas = OilyPNG::Canvas.new(4, 8, ChunkyPNG::Color.rgba(0, 0, 0, 75))
      @oily_canvas.compose(oily_subcanvas, 8, 4).pixels.should == @canvas.compose(subcanvas, 8, 4).pixels
    end
    
    it "should leave the original intact" do
      result = @oily_canvas.compose(OilyPNG::Canvas.new(1,1))
      @oily_canvas.should == oily_reference_canvas('operations')
    end
    
    it "should not return itself" do
      @oily_canvas.compose(OilyPNG::Canvas.new(1,1)).should_not equal(@oily_canvas)
    end
    
    it "should raise an exception when the pixels to compose fall outside the image" do
      # For now this raises a runtime error, but it should probably raise a ChunkyPNG::OutOfBounds error
      lambda { @oily_canvas.compose(OilyPNG::Canvas.new(1,1), 16, 16) }.should raise_error(RuntimeError)
    end
    
    
    it "should not change the image if the image is composed onto itself" do
      @oily_canvas.compose(@oily_canvas).should == @oily_canvas
    end
    
  end
end
