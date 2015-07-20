Feature: Application will program Mono device
	To transfer my own app to Mono
	As a maker
	I want the application to program Mono over the USB.

	Scenario Outline: Application will complain about missing files
		When I run `monoprog --mock devboard --program` on test data `<file>`
		Then the output should contain "File does not exist"
		And the exit status should be 4

		Examples:
			| file             |
			| doesNotExist.elf |
			| mumbo jumbo      |

	Scenario Outline: Application will complain about wrong file type
		When I run `monoprog --mock monoboard --program` on test data `<file>`
		Then the output should contain "Unknown program type"
		And the exit status should be 5

		Examples:
			| file  |
			| a.out |
			| b.o   |

	Scenario Outline: Application will complain about broken files
		When I run `monoprog --mock devboard --program` on test data `<file>`
		Then the output should contain "Corrupt program"
		And the exit status should be 3

		Examples:
			| file         |
			| broken.cyacd |
			| broken.ELF   |

	Scenario Outline: Application will program device with a valid CYACD file
		When I run `monoprog --mock <board> --program` on test data `<cyacd>`
		And the exit status should be 0

		Examples:
			| cyacd                 | board     |
			| ledBlinker.cyacd      | devboard  |
			| ledBlinkerSlow.CYACD  | devboard  |
			| wayDisplayTest.cyacd  | monoboard |
			| usbuart_test.cyacd    | monoboard |
			| mono_i2c_test.cyacd   | monoboard |

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
			| wayDisplayTest2.elf  |

	Scenario Outline: Application will program device with a valid ELF file
		When I run `monoprog --mock <board> --program` on test data `<elf>`
		Then the exit status should be 0

		Examples:
			| elf                 | board     |
			| ledBlinker.elf      | devboard  |
			| ledBlinkerSlow.ELF  | devboard  |
			| wayDisplayTest.elf  | monoboard |
			| wayDisplayTest2.elf | monoboard |
			| usbuart_test.elf    | monoboard |
			| mono_i2c_test.elf   | monoboard |

	Scenario: Application will complain when silicon ID does not match program.
		When I run `monoprog --mock devboard --program` on test data `wayDisplayTest2.elf`
		Then the output should contain "Silicon ID for device does not match program"
		And the exit status should be 7
