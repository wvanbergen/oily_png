Gem::Specification.new do |s|
  s.name = 'oily_png'
  s.rubyforge_project = s.name
  
  # Do not change the version and date fields by hand. This will be done
  # automatically by the gem release script.
  s.version = "0.0.1"
  s.date    = "2010-10-04"

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
  s.add_runtime_dependency('chunky_png', '~> 0.10.1')

  s.add_development_dependency('rake')
  s.add_development_dependency('rspec', '>= 1.3')

  s.rdoc_options << '--title' << s.name << '--main' << 'README.rdoc' << '--line-numbers' << '--inline-source'
  s.extra_rdoc_files = ['README.rdoc']

  # Do not change the files and test_files fields by hand. This will be done
  # automatically by the gem release script.
  s.files      = %w(.gitignore Gemfile Gemfile.lock LICENSE README.rdoc Rakefile ext/oily_png/decoding.c ext/oily_png/extconf.rb lib/oily_png.rb oily_png.gemspec tasks/github-gem.rake)
  s.test_files = %w()
end
