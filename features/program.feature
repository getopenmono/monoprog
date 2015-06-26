Feature: Application will program Mono device
	To transfer my own app to Mono
	As a maker
	I want the application to program Mono over the USB.

	Scenario Outline: Application will complain about missing files
		When I run `monoprog --mock connected --program` on test data `<file>`
		Then the output should contain "File does not exist"
		And the exit status should be 4

		Examples:
			| file             |
			| doesNotExist.elf |
			| mumbo jumbo      |

	Scenario Outline: Application will complain about wrong file type
		When I run `monoprog --mock connected --program` on test data `<file>`
		Then the output should contain "Unknown program type"
		And the exit status should be 5

		Examples:
			| file  |
			| a.out |
			| b.o   |

	Scenario Outline: Application will complain about broken files
		When I run `monoprog --mock connected --program` on test data `<file>`
		Then the output should contain "Corrupt program"
		And the exit status should be 3

		Examples:
			| file         |
			| broken.cyacd |
			| broken.ELF   |

	Scenario Outline: Application will program device with a valid CYACD file
		When I run `monoprog --mock connected --program` on test data `<cyacd>`
		And the exit status should be 0

		Examples:
			| cyacd                |
			| ledBlinker.cyacd     |
			| ledBlinkerSlow.CYACD |

	Scenario Outline: Application will complain about missing connection when Mono not detected
		When I run `monoprog --mock disconnected --program` on test data `<file>`
		Then the output should contain "device not detected"
		And the exit status should be 2

		Examples:
			| file                 |
			| ledBlinker.cyacd     |
			| ledBlinkerSlow.CYACD |
			| ledBlinker.elf       |
			| ledBlinkerSlow.ELF   |

	Scenario Outline: Application will program device with a valid ELF file
		When I run `monoprog --mock connected --program` on test data `<elf>`
		And the exit status should be 0

		Examples:
			| elf                |
			| ledBlinker.elf     |
			| ledBlinkerSlow.ELF |

	Scenario: Application will complain about missing siliconId in ELF file
		When I run `monoprog --mock connected --program` on test data `noCyMetaSection.elf`
		Then the output should contain "No section .cymeta in ELF program"
		And the exit status should be 6
