Feature: Application should program Mono device
	To transfer my own app to Mono
	As a maker
	I want the application to program Mono over the USB.

	Scenario Outline: Camplain about missing program
		When I run `monoprog <arg>`
		Then the output should contain "Missing value after '<arg>'"
		And the exit status should be 1

		Examples:
			| arg       |
			| -p        |
			| --program |

	Scenario Outline: Complain about missing files
		When I run `monoprog --mock devboard --program` on test data `<file>`
		Then the output should contain "File does not exist"
		And the exit status should be 4

		Examples:
			| file             |
			| doesNotExist.elf |
			| mumbo jumbo      |

	Scenario Outline: Complain about wrong file type
		When I run `monoprog --mock monoboard --program` on test data `<file>`
		Then the output should contain "Unknown program type"
		And the exit status should be 5

		Examples:
			| file  |
			| a.out |
			| b.o   |

	Scenario Outline: Complain about broken files
		When I run `monoprog --mock devboard --program` on test data `<file>`
		Then the output should contain "Corrupt program"
		And the exit status should be 3

		Examples:
			| file         |
			| broken.cyacd |
			| broken.ELF   |

	Scenario Outline: Valid CYACD file
		When I run `monoprog --mock <board> --program` on test data `<cyacd>`
		And the exit status should be 0

		Examples:
			| cyacd                 | board      |
			| ledBlinker.cyacd      | devboard   |
			| ledBlinkerSlow.CYACD  | devboard   |
			| wayDisplayTest.cyacd  | monoboard  |
			| usbuart_test.cyacd    | monoboard  |
			| mono_i2c_test.cyacd   | monoboard  |
			| wayDisplayTest.cyacd  | responsive |
			| usbuart_test.cyacd    | responsive |
			| mono_i2c_test.cyacd   | responsive |

	Scenario Outline: Complain about missing connection when Mono not detected
		When I run `monoprog --mock <board> --program` on test data `<file>`
		Then the output should contain "device not detected"
		And the exit status should be 2

		Examples:
			| file                 | board        |
			| ledBlinker.cyacd     | disconnected |
			| ledBlinkerSlow.CYACD | disconnected |
			| ledBlinker.elf       | disconnected |
			| ledBlinkerSlow.ELF   | disconnected |
			| wayDisplayTest2.elf  | disconnected |
			| ledBlinker.cyacd     | unresponsive |
			| wayDisplayTest2.elf  | unresponsive |

	Scenario Outline: Valid ELF file
		When I run `monoprog --mock <board> --program` on test data `<elf>`
		Then the exit status should be 0

		Examples:
			| elf                 | board      |
			| ledBlinker.elf      | devboard   |
			| ledBlinkerSlow.ELF  | devboard   |
			| wayDisplayTest.elf  | monoboard  |
			| wayDisplayTest2.elf | monoboard  |
			| usbuart_test.elf    | monoboard  |
			| mono_i2c_test.elf   | monoboard  |
			| mono_project.elf    | responsive |
			| usbuart_test.elf    | responsive |
			| mono_i2c_test.elf   | responsive |
			| mono_project.elf    | responsive |

	Scenario: Complain when silicon ID does not match program
		When I run `monoprog --mock devboard --program` on test data `wayDisplayTest2.elf`
		Then the output should contain "Silicon ID for device does not match program"
		And the exit status should be 7

	Scenario Outline: Output for headless use
		When I run `monoprog --mock monoboard <arg> --program` on test data `usbuart_test.elf`
		Then the exit status should be 0

		Examples:
			| arg        |
			| -H         |
			| --headless |
