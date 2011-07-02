require 'spec_helper'

describe OilyPNG::Native do

  include OilyPNG::Native

  describe '#steps' do
    it "should generate the steps from 4 to 8 as [0,0,1,1,2,2,3,3]" do
      steps(4,8).should == [0,0,1,1,2,2,3,3]
    end

    it "should generate the steps the same as ChunkyPNG" do
      image = ChunkyPNG::Image.new(1,1)
      steps(2,8).should == image.steps(2,8)
      steps(2,11).should == image.steps(2,11)
      steps(11,5).should == image.steps(11,5)
    end
  end
end
