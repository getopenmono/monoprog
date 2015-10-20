require 'aruba/cucumber'

# Has to be the same as in '../../configuration.sh'
BUILDDIR = 'bin'

PROJECT_ROOT = File.join(File.dirname(__FILE__),'..','..')

module ArubaOverrides
  def detect_ruby(cmd)
    processor, platform, *rest = RUBY_PLATFORM.split("-")
    if platform =~ /w32$/ && cmd =~ /^monoprog/
      ENV['PATH'] = "#{File.expand_path(File.dirname(__FILE__) + '/../../' + BUILDDIR + '/debug')}#{File::PATH_SEPARATOR}#{ENV['PATH']}"
      *parts = cmd.split(" ")
      parts[0] += ".exe"
      wincmd = parts.join(" ")
      "#{wincmd}"
    else
      # The application will be located in the 'build' directory.
      ENV['PATH'] = "#{File.join(PROJECT_ROOT,BUILDDIR)}#{File::PATH_SEPARATOR}#{ENV['PATH']}"
      "#{cmd}"
    end
  end
end

World(ArubaOverrides)
