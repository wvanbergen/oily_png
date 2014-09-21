require 'spec_helper'

describe OilyPNG::Color do
  include OilyPNG::Color

  before(:each) do
    @white             = 0xffffffff
    @black             = 0x000000ff
    @opaque            = 0x0a6496ff
    @non_opaque        = 0x0a649664
    @fully_transparent = 0x0a649600
  end

  describe '#compose_quick' do
    it 'should use the foregorund color as is when the background color is fully transparent' do
      expect(compose_quick(@non_opaque, @fully_transparent)).to be(@non_opaque)
    end

    it 'should use the foregorund color as is when an opaque color is given as foreground color' do
      expect(compose_quick(@opaque, @white)).to be(@opaque)
    end

    it 'should use the background color as is when a fully transparent pixel is given as foreground color' do
      expect(compose_quick(@fully_transparent, @white)).to be(@white)
    end

    it 'should compose pixels correctly' do
      expect(compose_quick(@non_opaque, @white)).to be(0x9fc2d6ff)
    end

    it 'should compose colors exactly the same as ChunkyPNG' do
      fg, bg = rand(0xffffffff), rand(0xffffffff)
      expect(compose_quick(fg, bg)).to be(ChunkyPNG::Color.compose_quick(fg, bg))
    end
  end

  describe '#euclidean_distance_rgba' do
    let(:color_a) { rand(0xffffffff) }
    let(:color_b) { rand(0xffffffff) }
    subject { euclidean_distance_rgba(color_a, color_b) }

    it { should == ChunkyPNG::Color.euclidean_distance_rgba(color_a, color_b) }

    context 'when both colors are the same' do
      let(:color_b) { color_a }
      it { should == 0 }
    end
  end
end
