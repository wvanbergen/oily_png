Gem::Specification.new do |s|
  s.name = 'oily_png'
  s.rubyforge_project = s.name
  
  # Do not change the version and date fields by hand. This will be done
  # automatically by the gem release script.
  s.version = "0.3.0"
  s.date    = "2010-12-12"

  s.summary     = "Native mixin to speed up ChunkyPNG"
  s.description = <<-EOT
    This Ruby C extenstion defines a module that can be included into ChunkyPNG to improve its speed.
  EOT

  s.authors  = ['Willem van Bergen']
  s.email    = ['willem@railsdoctors.com']
  s.homepage = 'http://wiki.github.com/wvanbergen/oily_png'

  s.extensions    = ["ext/oily_png/extconf.rb"]
  s.require_paths = ["lib", "ext"]

  s.add_runtime_dependency('chunky_png', '~> 0.12')

  s.add_development_dependency('rake')
  s.add_development_dependency('rake-compiler')
  s.add_development_dependency('rspec', '~> 2')

  s.rdoc_options << '--title' << s.name << '--main' << 'README.rdoc' << '--line-numbers' << '--inline-source'
  s.extra_rdoc_files = ['README.rdoc']

  # Do not change the files and test_files fields by hand. This will be done
  # automatically by the gem release script.
  s.files      = %w(.gitignore .infinity_test Gemfile Gemfile.lock LICENSE README.rdoc Rakefile ext/oily_png/extconf.rb ext/oily_png/oily_png_ext.c ext/oily_png/oily_png_ext.h ext/oily_png/png_decoding.c ext/oily_png/png_decoding.h ext/oily_png/png_encoding.c ext/oily_png/png_encoding.h lib/oily_png.rb oily_png.gemspec spec/decoding_spec.rb spec/encoding_spec.rb spec/resources/basi0g01.png spec/resources/basi0g02.png spec/resources/basi0g04.png spec/resources/basi0g08.png spec/resources/basi0g16.png spec/resources/basi2c08.png spec/resources/basi2c16.png spec/resources/basi3p01.png spec/resources/basi3p02.png spec/resources/basi3p04.png spec/resources/basi3p08.png spec/resources/basi4a08.png spec/resources/basi4a16.png spec/resources/basi6a08.png spec/resources/basi6a16.png spec/resources/basn0g01.png spec/resources/basn0g02.png spec/resources/basn0g04.png spec/resources/basn0g08.png spec/resources/basn0g16.png spec/resources/basn2c08.png spec/resources/basn2c16.png spec/resources/basn3p01.png spec/resources/basn3p02.png spec/resources/basn3p04.png spec/resources/basn3p08.png spec/resources/basn4a08.png spec/resources/basn4a16.png spec/resources/basn6a08.png spec/resources/basn6a16.png spec/resources/gray.png spec/resources/interlaced.png spec/resources/nonsquare.png spec/resources/s01i3p01.png spec/resources/s01n3p01.png spec/resources/s02i3p01.png spec/resources/s02n3p01.png spec/resources/s03i3p01.png spec/resources/s03n3p01.png spec/resources/s04i3p01.png spec/resources/s04n3p01.png spec/resources/s05i3p02.png spec/resources/s05n3p02.png spec/resources/s06i3p02.png spec/resources/s06n3p02.png spec/resources/s07i3p02.png spec/resources/s07n3p02.png spec/resources/s08i3p02.png spec/resources/s08n3p02.png spec/resources/s09i3p02.png spec/resources/s09n3p02.png spec/resources/s32i3p04.png spec/resources/s32n3p04.png spec/resources/s33i3p04.png spec/resources/s33n3p04.png spec/resources/s34i3p04.png spec/resources/s34n3p04.png spec/resources/s35i3p04.png spec/resources/s35n3p04.png spec/resources/s36i3p04.png spec/resources/s36n3p04.png spec/resources/s37i3p04.png spec/resources/s37n3p04.png spec/resources/s38i3p04.png spec/resources/s38n3p04.png spec/resources/s39i3p04.png spec/resources/s39n3p04.png spec/resources/s40i3p04.png spec/resources/s40n3p04.png spec/resources/square.png spec/spec_helper.rb tasks/github-gem.rake tasks/testing.rake)
  s.test_files = %w(spec/decoding_spec.rb spec/encoding_spec.rb)
end
