Dir['tasks/*.rake'].each { |file| load(file) }

require 'rake/extensiontask'

gem_management_tasks = GithubGem::RakeTasks.new(:gem)
Rake::ExtensionTask.new('oily_png', gem_management_tasks.gemspec) do |ext|
  ext.lib_dir = File.join('lib', 'oily_png')
end

Rake::Task['spec:basic'].prerequisites << :compile
Rake::Task['spec:rcov'].prerequisites << :compile
Rake::Task['spec:specdoc'].prerequisites << :compile

task :default => [:spec]
