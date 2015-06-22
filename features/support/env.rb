require 'aruba/cucumber'

# The application will be located in the 'build' directory.
ENV['PATH'] = "#{File.expand_path(File.dirname(__FILE__) + '/../../build')}#{File::PATH_SEPARATOR}#{ENV['PATH']}"

module ArubaOverrides
  def detect_ruby(cmd)
    processor, platform, *rest = RUBY_PLATFORM.split("-")
    #puts platform
    if platform =~ /w32$/ && cmd =~ /^monoprog /
      *parts = cmd.split(" ")
      parts[0] += ".exe"
      wincmd = parts.join(" ")
      "../../build/debug/#{wincmd}"
    else
      "#{cmd}"
    end
  end
end
 
World(ArubaOverrides)
