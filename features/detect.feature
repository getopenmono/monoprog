Feature: Application will detect Mono device
	To know if Mono is connected correctly to computer
	As a maker
	I want the application to indicate whether or not it can connect to Mono.

	Scenario Outline: Should not detect unresposive device
		When I run `monoprog --mock <board> <arg>`
		Then the output should contain "Mono device not detected on USB port"
		And the exit status should be 2

		Examples:
			| arg      | board        |
			| -d       | disconnected |
			| --detect | unresponsive |

	Scenario Outline: Should detect resposive device
		When I run `monoprog --detect --mock <board>`
		Then the output should contain "Mono device detected"
		And the exit status should be 0

		Examples:
			| board        |
			| devboard     |
			| monoboard    |
			| responsive   |
