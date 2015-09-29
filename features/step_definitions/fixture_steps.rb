FIXTURE_PATH = "features/data"

When /^I run `([^`]*)` on test data `([^`]*)`$/ do |cmd, fixture|
  path = File.expand_path("#{FIXTURE_PATH}/#{fixture}")
  run_simple(unescape(cmd + " " + path), false)
end
