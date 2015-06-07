Feature: Application can display help and license
	To know how to use the application
	As a maker
	I want the application to display a usage summary when invoked with no arguments.

	Scenario: Application invoked with no arguments
		When I run `monoprog`
		Then the output should contain "Usage: monoprog"
		And the exit status should be 1

	Scenario Outline: Application invoked with short help argument
		When I run `monoprog <arg>`
		Then the output should contain "Usage: monoprog"
		And the exit status should be 0

		Examples:
			| arg    |
			| -h     |
			| --help |

	Scenario Outline: Application invoked with long version argument
		When I run `monoprog <arg>`
		Then the output should contain "monoprog 0.3"
		And the exit status should be 0

		Examples:
			| arg       |
			| -v        |
			| --version |

	Scenario: Application can display licenses and versions of agregated software
		When I run `monoprog --license`
		Then the output should contain "Monolit ApS"
		And the output should contain "free of charge"
		And the output should contain "use, copy, modify, merge, publish"
		And the output should contain "distribute, sublicense"
		And the output should contain "WITHOUT WARRANTY OF ANY KIND"
		And the output should contain "Signal 11 Software"
		And the output should contain "Cypress Semiconductor Corporation"
		And the output should contain "Serge Lamikhov-Center"
		And the exit status should be 0
