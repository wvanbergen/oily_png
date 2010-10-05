Gem::Specification.new do |s|
  s.name = 'oily_png'
  s.rubyforge_project = s.name
  
  # Do not change the version and date fields by hand. This will be done
  # automatically by the gem release script.
  s.version = "0.0.4"
  s.date    = "2010-10-05"

  s.summary     = "Native mixin to speed up ChunkyPNG"
  s.description = <<-EOT
    This Ruby C extenstion defines a module that can be included into ChunkyPNG to improve its speed.
  EOT

  s.authors  = ['Willem van Bergen']
  s.email    = ['willem@railsdoctors.com']
  s.homepage = 'http://wiki.github.com/wvanbergen/oily_png'

  s.extensions    = ["ext/oily_png/extconf.rb"]
  s.require_paths = ["lib", "ext"]

  s.required_rubygems_version = '1.3.7'
  s.add_runtime_dependency('chunky_png', '~> 0.10.2')

  s.add_development_dependency('rake')
  s.add_development_dependency('rspec', '>= 1.3')

  s.rdoc_options << '--title' << s.name << '--main' << 'README.rdoc' << '--line-numbers' << '--inline-source'
  s.extra_rdoc_files = ['README.rdoc']

  # Do not change the files and test_files fields by hand. This will be done
  # automatically by the gem release script.
  s.files      = %w(.gitignore Gemfile Gemfile.lock LICENSE README.rdoc Rakefile ext/oily_png/extconf.rb ext/oily_png/oily_png_ext.c ext/oily_png/oily_png_ext.h ext/oily_png/png_decoding.c ext/oily_png/png_decoding.h ext/oily_png/png_encoding.c ext/oily_png/png_encoding.h lib/oily_png.rb oily_png.gemspec spec/decoding_spec.rb spec/encoding_spec.rb spec/resources/gray.png spec/resources/operations.png spec/spec_helper.rb tasks/github-gem.rake)
  s.test_files = %w(spec/decoding_spec.rb spec/encoding_spec.rb)
end
