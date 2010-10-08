require 'spec_helper'

describe OilyPNG::PNGDecoding do

  it "should call Color.bytesize in the pure ruby version" do
    ChunkyPNG::Color.should_receive(:bytesize).and_return(3)
    ChunkyPNG::Canvas.from_file(resource_file('square.png'))
  end

  it "should not call Color.bytesize in the native version" do
    ChunkyPNG::Color.should_not_receive(:bytesize)
    OilyCanvas.from_file(resource_file('square.png'))
  end
  
  it "should decode an interlaced image correctly" do
    c1 = OilyCanvas.from_file(resource_file('interlaced.png'))
    c2 = ChunkyPNG::Canvas.from_file(resource_file('interlaced.png'))
    c2.should == c1
  end
  
  context 'decoding different filtering methods' do
    before(:all) { @reference = ChunkyPNG::Canvas.from_file(resource_file('nonsquare.png'))}
    
    it "should decode NONE filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_NONE)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode SUB filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_SUB)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode UP filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_UP)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode AVERAGE filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_AVERAGE)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode PAETH filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_PAETH)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
  end
  
  context 'decoding different color modes' do
    before(:all) { @reference = ChunkyPNG::Canvas.from_file(resource_file('gray.png'))}
    
    it "should decode RGBA images the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:color_mode => ChunkyPNG::COLOR_TRUECOLOR_ALPHA)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode RGB images exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:color_mode => ChunkyPNG::COLOR_TRUECOLOR)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode indexed images exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:color_mode => ChunkyPNG::COLOR_INDEXED)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode grayscale images exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:color_mode => ChunkyPNG::COLOR_GRAYSCALE)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
    
    it "should decode grayscale images with transparency the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:color_mode => ChunkyPNG::COLOR_GRAYSCALE_ALPHA)
      ChunkyPNG::Canvas.from_blob(filtered_data).should == OilyCanvas.from_blob(filtered_data)
    end
  end
end
