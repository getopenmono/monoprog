require 'aruba/cucumber'

# Have to be the same as in '../../configuration.sh'
BUILDDIR = '../bin'
EXE = 'monoprog'
# (TODO: Use something from
# http://stackoverflow.com/questions/1197224/source-shell-script-into-environment-within-a-ruby-script
# to parse the the configuration.sh)

RELEASEDIR = "#{BUILDDIR}/release"
PROJECT_ROOT = File.join(File.dirname(__FILE__),'..','..')

module ArubaOverrides
  def detect_ruby(cmd)
    processor, platform, *rest = RUBY_PLATFORM.split("-")
    if platform =~ /w32$/ && cmd =~ /^#{EXE}/
      # The application will be located in the build directory.
      ENV['PATH'] = "#{File.expand_path(File.dirname(__FILE__)+'/../../'+RELEASEDIR)};#{ENV['PATH']}"
      # The application will Windows extension.
      *parts = cmd.split(" ")
      parts[0] += ".exe"
      wincmd = parts.join(" ")
      "#{wincmd}"
    elsif platform =~ /^darwin/
      # The application will be located in the app bundle directory.
      ENV['PATH'] = "#{File.join(PROJECT_ROOT,BUILDDIR)}/#{EXE}.app/Contents/MacOS:#{ENV['PATH']}"
      "#{cmd}"
    else
      # The application will be located in the build directory.
      ENV['PATH'] = "#{File.join(PROJECT_ROOT,BUILDDIR)}:#{ENV['PATH']}"
      "#{cmd}"
    end
  end
end

World(ArubaOverrides)
