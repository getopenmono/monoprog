Feature: Application will complain about wrong input
	To know how to use the application
	As a maker
	I want the application to tell me exactly what was wrong with wrong input.

	Scenario Outline: Application invoked with wrong arguments
		When I run `monoprog <arg>`
		Then the output should contain "Unknown option '<option>'"
		And the exit status should be 1

		Examples:
			| arg      | option |
			| -x       | x      |
			| --babble | babble |

    Scenario: Application invoked with missing mock device
        When I run `monoprog --mock`
        Then the output should contain "Missing value after '--mock'"
        And the exit status should be 1

    Scenario Outline: Application invoked with wrong mock device
        When I run `monoprog --mock blabla`
        Then the output should contain "Unknown mock device"
        Then the output should contain "<device>"
        And the exit status should be 1

        Examples:
            | device       |
            | connected    |
            | disconnected |
