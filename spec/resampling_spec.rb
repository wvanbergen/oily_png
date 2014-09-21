require 'spec_helper'

describe OilyPNG::Resampling do

  include OilyPNG::Resampling

  describe '#steps' do
    it "should generate the steps from 4 to 8 as [0,0,1,1,2,2,3,3]" do
      expect(steps(4, 8)).to eq([0, 0, 1, 1, 2, 2, 3, 3])
    end

    it "should generate the steps the same as ChunkyPNG" do
      image = ChunkyPNG::Image.new(1,1)
      expect(steps( 2,  8)).to eq(image.send(:steps, 2, 8))
      expect(steps( 2, 11)).to eq(image.send(:steps, 2,11))
      expect(steps(11,  5)).to eq(image.send(:steps,11, 5))
    end
  end

  describe '#resample_nearest_neighbor!' do
    before(:all) { @reference = ChunkyPNG::Canvas.from_file(resource_file('nonsquare.png'))}

    it "should resample [0,1,2,3] to 4x4 properly" do
      expect(OilyPNG::Canvas.new(2,2,[0,1,2,3]).resample_nearest_neighbor(4,4)).to eq(OilyPNG::Canvas.new(4,4,[0,0,1,1,0,0,1,1,2,2,3,3,2,2,3,3]))
    end

    it "should resample [0,1,2,3] to 99x45 as ChunkyPNG does" do
      expect(ChunkyPNG::Canvas.new(2,2,[0,1,2,3]).resample_nearest_neighbor(99,45)).to eq(OilyPNG::Canvas.new(2,2,[0,1,2,3]).resample_nearest_neighbor(99,45))
    end

    it "should resample an image to 10x20 as ChunkyPNG does" do
      expect(@reference.resample_nearest_neighbor(10,20)).to eq(OilyPNG::Canvas.from_canvas(@reference).resample_nearest_neighbor(10,20))
    end

    it "should resample an image to 11x19 as ChunkyPNG does" do
      expect(@reference.resample_nearest_neighbor(11,19)).to eq(OilyPNG::Canvas.from_canvas(@reference).resample_nearest_neighbor(11,19))
    end
  end

  describe '#resample_bilinear!' do
    before(:all) { @reference = ChunkyPNG::Canvas.from_file(resource_file('nonsquare.png'))}

    it "should resample an image to 10x20 as ChunkyPNG does" do
      expect(@reference.resample_bilinear(10,20)).to eq(OilyPNG::Canvas.from_canvas(@reference).resample_bilinear(10,20))
    end

    it "should resample an image to 11x19 as ChunkyPNG does" do
      expect(@reference.resample_bilinear(11,19)).to eq(OilyPNG::Canvas.from_canvas(@reference).resample_bilinear(11,19))
    end

    it "should upsample an image to 88x44 as ChunkyPNG does" do
      expect(@reference.resample_bilinear(88,44)).to eq(OilyPNG::Canvas.from_canvas(@reference).resample_bilinear(88,44))
    end

    it "should not crash upsampling tall image" do
      @reference = ChunkyPNG::Canvas.from_file(resource_file('nonsquaretall.png'))
      expect { OilyPNG::Canvas.from_canvas(@reference).resample_bilinear(44,88) }.to_not raise_error
    end
  end
end
