Feature: Application will detect Mono device
	To know if Mono is connected correctly to computer
	As a maker
	I want the application to indicate whether or not it can connect to Mono.

	Scenario Outline: Should not detect unresposive device
		When I run `monoprog --timeout 10 --mock <board> <arg>`
		Then the output should contain "Mono device not detected on USB port"
		And the exit status should be 2

		Examples:
			| arg      | board        |
			| -d       | disconnected |
			| --detect | unresponsive |

	Scenario Outline: Should detect bootloader
		When I run `monoprog --detect --mock <board>`
		Then the output should contain "Mono device in bootloader detected"
		And the exit status should be 0

		Examples:
			| board        |
			| devboard     |
			| monoboard    |

	Scenario Outline: Should detect bootloader headless
		When I run `monoprog --detect --headless --mock <board>`
		Then the output should contain exactly "bootloader\n"
		And the exit status should be 0

		Examples:
			| board        |
			| devboard     |
			| monoboard    |

	Scenario: Should detect resposive device
		When I run `monoprog --detect --mock responsive`
		Then the output should contain "Mono device running app detected"
		And the exit status should be 0

	Scenario: Should detect resposive device headless
		When I run `monoprog --detect --headless --mock responsive`
		Then the output should contain exactly "app\n"
		And the exit status should be 0

	Scenario Outline: Should change timeout
		When I run `monoprog --detect --mock disconnected <switch> <ms>`
		And the output should contain "Mono device not detected on USB port"
		And the exit status should be 2

		Examples:
			| switch    | ms   |
			| -t        | 100  |
			| --timeout | 1000 |
