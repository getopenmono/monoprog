require 'aruba/cucumber'

# The application will be located in the 'build' directory.
ENV['PATH'] = "#{File.expand_path(File.dirname(__FILE__) + '/../../build')}#{File::PATH_SEPARATOR}#{ENV['PATH']}"
