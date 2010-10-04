# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# paths = ['/opt/local', '/usr/local']
# lib_paths = paths.map { |p| p + '/lib' }
# inc_paths = paths.map { |p| p + '/include' }

# exit(1) unless find_library('png', 'png_get_io_ptr', *lib_paths) && find_header('png.h', *inc_paths)

# Create a makefile
create_makefile('oily_png/decoding')
