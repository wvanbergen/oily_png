require 'spec_helper'

describe OilyPNG::Operations do
  
  describe '#compose!' do
    subject { oily_reference_canvas('operations') } 

    it "should compose two images without offset exactly the same way as ChunkyPNG" do
      subcanvas = ChunkyPNG::Canvas.new(4, 8, ChunkyPNG::Color.rgba(0, 0, 0, 75))
      subject.compose!(subcanvas)
      expect(ChunkyPNG::Canvas.from_canvas(subject)).to eq(reference_canvas('operations').compose(subcanvas))
    end

    it "should compose two images with offset exactly the same way as ChunkyPNG" do
      subject.compose!(ChunkyPNG::Canvas.new(4, 8, ChunkyPNG::Color.rgba(0, 0, 0, 75)), 8, 4) 
      expect(subject).to eql(oily_reference_canvas('composited'))
    end

    it "should return itself" do
      expect(subject.compose!(OilyPNG::Canvas.new(1,1))).to be(subject)
    end

    it "should raise an exception when the pixels to compose fall outside the image" do
      # For now this raises a runtime error, but it should probably raise a ChunkyPNG::OutOfBounds error
      expect { subject.compose!(OilyPNG::Canvas.new(1,1), 16, 16) }.to raise_error
    end
  end
  
  describe '#replace!' do
    subject { oily_reference_canvas('operations') } 

    it "should compose two images without offset exactly the same way as ChunkyPNG" do
      subcanvas = ChunkyPNG::Canvas.new(3, 2, ChunkyPNG::Color.rgb(200, 255, 0))
      subject.replace!(subcanvas)
      expect(ChunkyPNG::Canvas.from_canvas(subject)).to eq(reference_canvas('operations').replace(subcanvas))
    end

    it "should compose two images with offset exactly the same way as ChunkyPNG" do
      subject.replace!(ChunkyPNG::Canvas.new(3, 2, ChunkyPNG::Color.rgb(200, 255, 0)), 5, 4) 
      expect(subject).to eq(oily_reference_canvas('replaced'))
    end

    it "should return itself" do
      expect(subject.replace!(OilyPNG::Canvas.new(1,1))).to be(subject)
    end

    it "should raise an exception when the pixels to compose fall outside the image" do
      # For now this raises a runtime error, but it should probably raise a ChunkyPNG::OutOfBounds error
      expect { subject.replace!(OilyPNG::Canvas.new(1,1), 16, 16) }.to raise_error
    end
  end

  describe '#rotate_left!' do
    subject { OilyPNG::Canvas.new(2, 3, [1, 2, 3, 4, 5, 6]) }

    it "should rotate the pixels 90 degrees clockwise" do
      subject.rotate_left!
      expect(subject).to eql OilyPNG::Canvas.new(3, 2, [2, 4, 6, 1, 3, 5] )
    end

    it "should return itself" do
      expect(subject.rotate_left!).to equal(subject)
    end

    it "should change the image dimensions" do
      expect { subject.rotate_left! }.to change { subject.dimension }.
          from(ChunkyPNG::Dimension('2x3')).to(ChunkyPNG::Dimension('3x2'))
    end

    it "it should rotate 180 degrees when applied twice" do
      subject_dup = subject.dup
      expect(subject.rotate_left!.rotate_left!).to eql subject_dup.rotate_180
    end

    it "it should rotate right when applied three times" do
      subject_dup = subject.dup
      expect(subject.rotate_left!.rotate_left!.rotate_left!).to eql subject_dup.rotate_right!
    end

    it "should return itself when applied four times" do
      subject_dup = subject.dup
      expect(subject.rotate_left!.rotate_left!.rotate_left!.rotate_left!).to eql subject_dup
    end
  end

  describe '#rotate_right!' do
    subject { OilyPNG::Canvas.new(2, 3, [1, 2, 3, 4, 5, 6]) }

    it "should rotate the pixels 90 degrees clockwise" do
      subject.rotate_right!
      expect(subject).to eql OilyPNG::Canvas.new(3, 2, [5, 3, 1, 6, 4, 2] )
    end

    it "should return itself" do
      expect(subject.rotate_right!).to equal(subject)
    end

    it "should change the image dimensions" do
      expect { subject.rotate_right! }.to change { subject.dimension }.
          from(ChunkyPNG::Dimension('2x3')).to(ChunkyPNG::Dimension('3x2'))
    end

    it "it should rotate 180 degrees when applied twice" do
      subject_dup = subject.dup
      expect(subject.rotate_right!.rotate_right!).to eql subject_dup.rotate_180
    end

    it "it should rotate left when applied three times" do
      subject_dup = subject.dup
      expect(subject.rotate_right!.rotate_right!.rotate_right!).to eql subject_dup.rotate_left
    end

    it "should return itself when applied four times" do
      subject_dup = subject.dup
      expect(subject.rotate_right!.rotate_right!.rotate_right!.rotate_right!).to eql subject_dup
    end
  end
end
