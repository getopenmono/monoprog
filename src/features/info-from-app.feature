Feature: Application can extract meta data from app
	To check whether my application makes proper use of resources
	As a maker
	I want the application to display meta information from an app.

	Scenario Outline: Complain missing value
		When I run `monoprog <arg>`
		Then the output should contain "Missing value after '<arg>'"
		And the exit status should be 1

		Examples:
			| arg    |
			| -i     |
			| --info |

	Scenario Outline: Complain about missing files
		When I run `monoprog --info` on test data `<file>`
		Then the output should contain "File does not exist"
		And the exit status should be 4

		Examples:
			| file             |
			| doesNotExist.elf |
			| mumbo jumbo      |

	Scenario Outline: Complain about wrong file type
		When I run `monoprog --info` on test data `<file>`
		Then the output should contain "Unknown program type"
		And the exit status should be 5

		Examples:
			| file           |
			| a.out          |
			| b.o            |
			| bootloader.hex |

	Scenario Outline: Unsupported file types
		When I run `monoprog --info` on test data `<file>`
		Then the output should contain "File type not supported"
		And the exit status should be 9

		Examples:
			| file             |
			| ledBlinker.cyacd |

	Scenario Outline: Display info
		When I run `monoprog --info` on test data `<file>`
		Then the output should contain "Flash bytes used: <flash>"
		And the exit status should be 0

		Examples:
			| file                | flash  |
			| ledBlinker.elf      | 33408  |
			| wayDisplayTest2.elf | 21600  |

	Scenario: Display info headless
		When I run `monoprog --headless --info` on test data `ledBlinker.elf`
		Then the output should contain exactly "33408\n"
		And the exit status should be 0
