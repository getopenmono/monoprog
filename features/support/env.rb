require 'aruba/cucumber'

module ArubaOverrides
  def detect_ruby(cmd)
    processor, platform, *rest = RUBY_PLATFORM.split("-")
    if platform =~ /w32$/ && cmd =~ /^monoprog/
      # The application will be located in the 'build/[configuration]' directory.
      ENV['PATH'] = "#{File.expand_path(File.dirname(__FILE__) + '/../../build/debug')}#{File::PATH_SEPARATOR}#{ENV['PATH']}"
      *parts = cmd.split(" ")
      parts[0] += ".exe"
      wincmd = parts.join(" ")
      "#{wincmd}"
    else
      # The application will be located in the 'build' directory.
      ENV['PATH'] = "#{File.expand_path(File.dirname(__FILE__) + '/../../build')}#{File::PATH_SEPARATOR}#{ENV['PATH']}"
      "#{cmd}"
    end
  end
end
 
World(ArubaOverrides)
