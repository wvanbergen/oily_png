require 'spec_helper'

describe OilyPNG::PNGDecoding do

  it "should call ChunkyPNG::Color.pixel_bytesize in the pure ruby version" do
    expect(ChunkyPNG::Color).to receive(:pixel_bytesize).and_return(3)
    ChunkyPNG::Canvas.from_file(resource_file('square.png'))
  end

  it "should not call ChunkyPNG::Color.pixel_bytesize in the native version" do
    expect(ChunkyPNG::Color).to receive(:pixel_bytesize).never
    OilyPNG::Canvas.from_file(resource_file('square.png'))
  end
  
  context 'decoding different filtering methods' do
    before(:all) { @reference = ChunkyPNG::Canvas.from_file(resource_file('nonsquare.png'))}
    
    it "should decode NONE filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_NONE)
      expect(ChunkyPNG::Canvas.from_blob(filtered_data)).to eq(OilyPNG::Canvas.from_blob(filtered_data))
    end
    
    it "should decode SUB filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_SUB)
      expect(ChunkyPNG::Canvas.from_blob(filtered_data)).to eq(OilyPNG::Canvas.from_blob(filtered_data))
    end
    
    it "should decode UP filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_UP)
      expect(ChunkyPNG::Canvas.from_blob(filtered_data)).to eq(OilyPNG::Canvas.from_blob(filtered_data))
    end
    
    it "should decode AVERAGE filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_AVERAGE)
      expect(ChunkyPNG::Canvas.from_blob(filtered_data)).to eq(OilyPNG::Canvas.from_blob(filtered_data))
    end
    
    it "should decode PAETH filtering exactly the same as ChunkyPNG" do
      filtered_data = @reference.to_blob(:filtering => ChunkyPNG::FILTER_PAETH)
      expect(ChunkyPNG::Canvas.from_blob(filtered_data)).to eq(OilyPNG::Canvas.from_blob(filtered_data))
    end
  end
  
  context 'decoding compatibility with ChunkyPNG' do
    resource_files.each do |file|
      it "should #{File.basename(file)} the same as ChunkyPNG" do
        expect(OilyPNG::Canvas.from_file(file).pixels).to eq(ChunkyPNG::Canvas.from_file(file).pixels)
      end
    end
  end
end
