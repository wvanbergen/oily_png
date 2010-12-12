require 'rubygems'
require 'bundler'

Bundler.setup

Dir['tasks/*.rake'].each { |file| load(file) }

require 'rake/extensiontask'

def gemspec
  @clean_gemspec ||= eval(File.read(File.expand_path('../oily_png.gemspec', __FILE__)))
end

GithubGem::RakeTasks.new(:gem)

Rake::ExtensionTask.new('oily_png', gemspec) do |ext|
  ext.lib_dir = File.join('lib', 'oily_png')
end

Rake::Task['spec:basic'].prerequisites << :compile
Rake::Task['spec:rcov'].prerequisites << :compile
Rake::Task['spec:specdoc'].prerequisites << :compile

task :default => [:spec]
